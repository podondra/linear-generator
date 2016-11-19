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
#define NUM_EVENTS 3
#endif

using namespace std;

void opt_computation(
        uint32_t num,
        uint32_t k,
        uint32_t c,
        uint32_t d,
        uint32_t e,
        uint32_t *__restrict__ a,
        uint32_t *__restrict__ b,
        float *__restrict__ n,
        uint32_t *__restrict__ x,
        uint32_t *__restrict__ min,
        uint32_t *__restrict__ max,
        uint32_t *__restrict__ count
        ) {
    /* TODO comment */
    a = (uint32_t *)__builtin_assume_aligned(a, 32);
    b = (uint32_t *)__builtin_assume_aligned(b, 32);
    n = (float *)__builtin_assume_aligned(n, 32);
    x = (uint32_t *)__builtin_assume_aligned(x, 32);
    min = (uint32_t *)__builtin_assume_aligned(min, 32);
    max = (uint32_t *)__builtin_assume_aligned(max, 32);
    count = (uint32_t *)__builtin_assume_aligned(count, 32);

    for (size_t j = 0; j < num; ++j)
        n[j] = 1.f / std::exp2(n[j]);

    uint32_t dist;
    for (size_t i = 0; i < k; ++i) {
        /* for each linear generator */
        for (size_t j = 0; j < num; ++j) {
            /* compute next value */
            x[j] = a[j] * x[j] + b[j];
            x[j] -= ((uint32_t)(x[j] * n[j])) * n[j];

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
        float **n,
        uint32_t **x,
        uint32_t **min,
        uint32_t **max,
        uint32_t **count
        ) {
    *a = (uint32_t *)aligned_alloc(32, num * sizeof(uint32_t));
    *b = (uint32_t *)aligned_alloc(32, num * sizeof(uint32_t));
    *n = (float *)aligned_alloc(32, num * sizeof(float));
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
    uint32_t *a, *b, *x, *min, *max, *count;
    float *n;
    if (gen_opt(engine, num, &a, &b, &n, &x, &min, &max, &count) == false) {
        fprintf(stdout, "Not enough memory.\n");
        return 0;
    }

    uint32_t c = random_num(engine);
    uint32_t d = random_num(engine);
    uint32_t e = random_num(engine);

    chrono::high_resolution_clock::time_point start, end;
    /* start time measurement */
    start = chrono::high_resolution_clock::now();

#ifdef PAPI
    int Events[NUM_EVENTS] = { PAPI_L1_DCM, PAPI_L2_DCM, PAPI_L2_DCA };
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

    fprintf(stdout, "%lld ", values[0]);
    fprintf(stdout, "%lld ", values[1]);
    fprintf(stdout, "%lld ", values[2]);
#endif

    /* end time measurement */
    end = chrono::high_resolution_clock::now();

    chrono::duration<double> time_span;
    time_span = chrono::duration_cast<chrono::duration<double>>(end - start);

    /* use computed values so compiler does not exclude them */
    for (size_t i = 0; i < num; ++i)
        fprintf(
                stderr,
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
