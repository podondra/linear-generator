#include <chrono>
#include <cinttypes>
#include <climits>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <random>
#include "random.h"

using namespace std;

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
    *a = (uint32_t *)malloc(num * sizeof(uint32_t));
    *b = (uint32_t *)malloc(num * sizeof(uint32_t));
    *n = (uint32_t *)malloc(num * sizeof(uint32_t));
    *x = (uint32_t *)malloc(num * sizeof(uint32_t));
    *min = (uint32_t *)malloc(num * sizeof(uint32_t));
    *max = (uint32_t *)malloc(num * sizeof(uint32_t));
    *count = (uint32_t *)malloc(num * sizeof(uint32_t));

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

double opt(default_random_engine *engine, uint32_t k, uint32_t num) {
    /* read linear generators */
    uint32_t *a, *b, *n, *x, *min, *max, *count;
    if (gen_opt(engine, num, &a, &b, &n, &x, &min, &max, &count) == false) {
        fprintf(stdout, "Not enough memory.\n");
        return 0;
    }

    uint32_t c = random_num(engine);
    uint32_t d = random_num(engine);
    uint32_t e = random_num(engine);
    uint32_t dist;

    chrono::high_resolution_clock::time_point start, end;
    /* start time measurement */
    start = chrono::high_resolution_clock::now();

    for (size_t i = 0; i < k; ++i) {
        /* for each linear generator */
        for (size_t j = 0; j < num; ++j) {
            /* compute next value */
            x[j] = ((a[j] * x[j] + b[j]) % (2 << (n[j] - 1)));

            /* check if x is in interval */
            if (c <= x[j] && x[j] <= d)
                count[j] += 1;

            /* compute hamming distance */
            dist = x[j] ^ e;
            dist = dist - ((dist >> 1) & 0x55555555);
            dist = (dist & 0x33333333) + ((dist >> 2) & 0x33333333);
            dist = (((dist + (dist >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;

            /* check minimal hamming distance */
            if (min[j] > dist)
                min[j] = dist;
            /* check maximal hamming distance */
            if (max[j] < dist)
                max[j] = dist;
        }
    }

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
