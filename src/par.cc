#include <algorithm>
#include <chrono>
#include <cinttypes>
#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <omp.h>
#include <random>
#include "random.h"

#ifdef PAPI
#include <papi.h>
#define NUM_EVENTS 5
#endif

#define BF 16

using namespace std;

void par_computation(
        const size_t num,
        const uint32_t k,
        const uint32_t c,
        const uint32_t d,
        const uint32_t e,
        const uint32_t *__restrict__ a,
        const uint32_t *__restrict__ b,
        uint32_t *__restrict__ n,
        uint32_t *__restrict__ x,
        uint32_t *__restrict__ min,
        uint32_t *__restrict__ max,
        uint32_t *__restrict__ count
        ) {
    for (size_t j = 0; j < num; ++j)
        n[j] = (1 << n[j]) - 1;

    const uint32_t *__restrict__ p_a;
    const uint32_t *__restrict__ p_b;
    const uint32_t *__restrict__ p_n;
    uint32_t *__restrict__ p_x;
    uint32_t *__restrict__ p_min;
    uint32_t *__restrict__ p_max;
    uint32_t *__restrict__ p_count;

    uint32_t dist;
    /* loop tiling - main */
#pragma omp parallel for default(shared) num_threads(12) \
    private(dist, p_a, p_b, p_x, p_n, p_min, p_max, p_count)
    for (size_t j1 = 0; j1 < num - BF; j1 += BF) {
        p_a     = a + j1;
        p_b     = b + j1;
        p_x     = x + j1;
        p_n     = n + j1;
        p_min   = min + j1;
        p_max   = max + j1;
        p_count = count + j1;

        for (size_t i = 0; i < k; ++i) {
            for (size_t j = 0; j < BF; ++j) {
                /* compute next value */
                p_x[j] = (p_a[j] * p_x[j] + p_b[j]) & p_n[j];

                /* check if x is in interval */
                p_count[j] += (c <= p_x[j] && p_x[j] <= d) ? 1 : 0;

                /* compute hamming distance */
                dist = p_x[j] ^ e;
                dist = dist - ((dist >> 1) & 0x55555555);
                dist = (dist & 0x33333333) + ((dist >> 2) & 0x33333333);
                dist = (((dist + (dist >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;

                /* check minimal hamming distance */
                p_min[j] = (p_min[j] < dist) ? p_min[j] : dist;
                p_max[j] = (p_max[j] > dist) ? p_max[j] : dist;
            }
        }
    }

    /* loop tiling - the rest */
    size_t shift = BF * (num / BF);
    p_a     = a + shift;
    p_b     = b + shift;
    p_x     = x + shift;
    p_n     = n + shift;
    p_min   = min + shift;
    p_max   = max + shift;
    p_count = count + shift;
    for (size_t i = 0; i < k; ++i) {
        for (size_t j = 0; j < num % BF; ++j) {
            /* compute next value */
            p_x[j] = (p_a[j] * p_x[j] + p_b[j]) & p_n[j];

            /* check if x is in interval */
            p_count[j] += (c <= p_x[j] && p_x[j] <= d) ? 1 : 0;

            /* compute hamming distance */
            dist = p_x[j] ^ e;
            dist = dist - ((dist >> 1) & 0x55555555);
            dist = (dist & 0x33333333) + ((dist >> 2) & 0x33333333);
            dist = (((dist + (dist >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;

            /* check minimal hamming distance */
            p_min[j] = (p_min[j] < dist) ? p_min[j] : dist;
            p_max[j] = (p_max[j] > dist) ? p_max[j] : dist;
        }
    }
}

bool gen_par(
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

double par(default_random_engine *engine, uint32_t num, uint32_t k) {
    /* read linear generators */
    uint32_t *a, *b, *x, *n, *min, *max, *count;
    if (gen_par(engine, num, &a, &b, &n, &x, &min, &max, &count) == false) {
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

    par_computation(num, k, c, d, e, a, b, n, x, min, max, count);

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
