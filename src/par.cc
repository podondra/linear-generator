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

#define BF 16

using namespace std;

void par_computation(
        const size_t num,
        const uint32_t k,
        const uint32_t c,
        const uint32_t d,
        const uint32_t e,
        uint32_t *__restrict__ a,
        uint32_t *__restrict__ b,
        uint32_t *__restrict__ n,
        uint32_t *__restrict__ x,
        uint32_t *__restrict__ min,
        uint32_t *__restrict__ max,
        uint32_t *__restrict__ count
        ) {
    uint32_t *__restrict__ p_a;
    uint32_t *__restrict__ p_b;
    uint32_t *__restrict__ p_n;
    uint32_t *__restrict__ p_x;
    uint32_t *__restrict__ p_min;
    uint32_t *__restrict__ p_max;
    uint32_t *__restrict__ p_count;
    p_a = (uint32_t *__restrict__)__builtin_assume_aligned(a, 32);
    p_b = (uint32_t *__restrict__)__builtin_assume_aligned(b, 32);
    p_n = (uint32_t *__restrict__)__builtin_assume_aligned(n, 32);
    p_x = (uint32_t *__restrict__)__builtin_assume_aligned(x, 32);
    p_min = (uint32_t *__restrict__)__builtin_assume_aligned(min, 32);
    p_max = (uint32_t *__restrict__)__builtin_assume_aligned(max, 32);
    p_count = (uint32_t *__restrict__)__builtin_assume_aligned(count, 32);

    for (size_t j = 0; j < num; ++j)
        p_n[j] = (1 << p_n[j]) - 1;

    uint32_t dist;
    /* loop tiling - main */
#pragma omp parallel for default(shared) num_threads(24) \
    private(dist) schedule(static)
    for (size_t j1 = 0; j1 < BF * (num / BF); j1 += BF) {
        for (size_t i = 0; i < k; ++i) {
            for (size_t j = j1; j < j1 + BF; ++j) {
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
    for (size_t i = 0; i < k; ++i) {
        for (size_t j = BF * (num / BF); j < num; ++j) {
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

    par_computation(num, k, c, d, e, a, b, n, x, min, max, count);

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
