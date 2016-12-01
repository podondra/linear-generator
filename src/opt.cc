#include <algorithm>
#include <chrono>
#include <cinttypes>
#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <random>
#include "random.h"

#ifdef PAPI
#include <papi.h>
#define NUM_EVENTS 5
#endif

#define BF 16

using namespace std;

void opt_computation(
        size_t num,
        uint32_t k,
        uint32_t c,
        uint32_t d,
        uint32_t e,
        uint32_t *__restrict__ a,
        uint32_t *__restrict__ b,
        uint32_t *__restrict__ n,
        uint32_t *__restrict__ x,
        uint32_t *__restrict__ min,
        uint32_t *__restrict__ max,
        uint32_t *__restrict__ count
        ) {
    n = (uint32_t *)__builtin_assume_aligned(n, 32);
    for (size_t j = 0; j < num; ++j)
        n[j] = (1 << n[j]) - 1;

    a = (uint32_t *)__builtin_assume_aligned(a, 32);
    b = (uint32_t *)__builtin_assume_aligned(b, 32);
    x = (uint32_t *)__builtin_assume_aligned(x, 32);
    min = (uint32_t *)__builtin_assume_aligned(min, 32);
    max = (uint32_t *)__builtin_assume_aligned(max, 32);
    count = (uint32_t *)__builtin_assume_aligned(count, 32);

    uint32_t dist;
    /* loop tiling - main */
    for (size_t j1 = 0; j1 < num - BF; j1 += BF) {
        for (size_t i = 0; i < k; ++i) {
            for (size_t j = 0; j < BF; ++j) {
                /* compute next value */
                x[j] = (a[j] * x[j] + b[j]) & n[j];

                /* check if x is in interval */
                count[j] += (c <= x[j] && x[j] <= d) ? 1 : 0;

                /* compute hamming distance */
                dist = x[j] ^ e;
                dist = dist - ((dist >> 1) & 0x55555555);
                dist = (dist & 0x33333333) + ((dist >> 2) & 0x33333333);
                dist = (((dist + (dist >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;

                /* check minimal hamming distance */
                min[j] = (min[j] < dist) ? min[j] : dist;
                max[j] = (max[j] > dist) ? max[j] : dist;
            }
        }
        a += BF;
        b += BF;
        x += BF;
        n += BF;
        min += BF;
        max += BF;
        count += BF;
    }

    /* loop tiling - the rest */
    for (size_t i = 0; i < k; ++i) {
        for (size_t j = 0; j < num % BF; ++j) {
            /* compute next value */
            x[j] = (a[j] * x[j] + b[j]) & n[j];

            /* check if x is in interval */
            count[j] += (c <= x[j] && x[j] <= d) ? 1 : 0;

            /* compute hamming distance */
            dist = x[j] ^ e;
            dist = dist - ((dist >> 1) & 0x55555555);
            dist = (dist & 0x33333333) + ((dist >> 2) & 0x33333333);
            dist = (((dist + (dist >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;

            /* check minimal hamming distance */
            min[j] = (min[j] < dist) ? min[j] : dist;
            max[j] = (max[j] > dist) ? max[j] : dist;
        }
    }
}

bool gen_opt(
        std::default_random_engine *engine,
        uint32_t num,
        uint32_t **a,
        uint32_t **b,
        uint32_t **n,
        uint32_t **x,
        uint32_t **min,
        uint32_t **max,
        uint32_t **count
        ) {
    *a = (uint32_t *)aligned_alloc(32, num * sizeof(uint32_t));
    *b = (uint32_t *)aligned_alloc(32, num * sizeof(uint32_t));
    *n = (uint32_t *)aligned_alloc(32, num * sizeof(uint32_t));
    *x = (uint32_t *)aligned_alloc(32, num * sizeof(uint32_t));
    *min = (uint32_t *)aligned_alloc(32, num * sizeof(uint32_t));
    *max = (uint32_t *)aligned_alloc(32, num * sizeof(uint32_t));
    *count = (uint32_t *)aligned_alloc(32, num * sizeof(uint32_t));


    if (!a || !b || !n || !x || !min || !max || !count)
        return false;

    for (uint32_t i = 0; i < num; ++i) {
        (*a)[i] = random_odd_num(engine);
        (*b)[i] = random_odd_num(engine);
        (*n)[i] = random_n(engine);
        (*x)[i] = 0;
        (*count)[i] = 0;
        (*min)[i] = UINT32_MAX;
        (*max)[i] = 0;
    }

    return true;
}

double opt(default_random_engine *engine, uint32_t num, uint32_t k) {
    /* read linear generators */
    uint32_t *a, *b, *x, *n, *min, *max, *count;
    if (gen_opt(engine, num, &a, &b, &n, &x, &min, &max, &count) == false) {
        fprintf(stderr, "Not enough memory.\n");
        return 0;
    }

    uint32_t c = random_num(engine);
    uint32_t d = random_num(engine);
    uint32_t e = random_num(engine);

    chrono::high_resolution_clock::time_point start, end;
    /* start time measurement */
    start = chrono::high_resolution_clock::now();

#ifdef PAPI
    int Events[NUM_EVENTS] = { PAPI_L1_DCM, PAPI_L2_DCM, PAPI_L2_DCA, PAPI_L3_TCM, PAPI_L3_TCA };
    long_long values[NUM_EVENTS];

    /* start counting events */
    if (PAPI_start_counters(Events, NUM_EVENTS) != PAPI_OK)
        return 0;
#endif

    opt_computation(num, k, c, d, e, a, b, n, x, min, max, count);

#ifdef PAPI
    /* Stop counting events */
    if (PAPI_stop_counters(values, NUM_EVENTS) != PAPI_OK)
        return 0;

    fprintf(stderr, "%lld\t", values[0]);
    fprintf(stderr, "%lld\t", values[1]);
    fprintf(stderr, "%lld\t", values[2]);
    fprintf(stderr, "%lld\t", values[3]);
    fprintf(stderr, "%lld\t", values[4]);
#endif

    /* end time measurement */
    end = chrono::high_resolution_clock::now();

    chrono::duration<double> time_span;
    time_span = chrono::duration_cast<chrono::duration<double>>(end - start);

    /* use computed values so compiler does not exclude them */
    for (size_t i = 0; i < num; ++i)
        fprintf(
                stdout,
                "%" PRIu32 "%" PRIu32 "%" PRIu32,
                min[i], max[i], count[i]
                );

    /* free arrays */
    free(a);
    free(b);
    free(n);
    free(x);
    free(count);
    free(min);
    free(max);

    return time_span.count();
}
