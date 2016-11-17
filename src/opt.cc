#include <chrono>
#include <cinttypes>
#include <climits>
#include <cstdint>
#include <cstdio>
#include <random>
#include "random.h"

using namespace std;

uint32_t **gen_opt(default_random_engine *engine, uint32_t num) {
    uint32_t **lgs = (uint32_t **)malloc(num * sizeof(uint32_t *));
    if (lgs == nullptr)
        return nullptr;
    for (size_t i = 0; i < num; ++i) {
        lgs[i] = (uint32_t *)malloc(3 * sizeof(uint32_t));
        if (lgs[i] == nullptr)
            return nullptr;
        lgs[i][0] = random_odd_num(engine);
        lgs[i][1] = random_odd_num(engine);
        lgs[i][2] = random_n(engine);
    }
    return lgs;
}

double opt(default_random_engine *engine, uint32_t k, uint32_t num) {
    /* read linear generators */
    uint32_t **linear_generators = gen_opt(engine, num);
    if (linear_generators == nullptr) {
        fprintf(stdout, "Not enough memory.\n");
        return 0;
    }

    uint32_t c = random_num(engine);
    uint32_t d = random_num(engine);
    uint32_t e = random_num(engine);

    uint32_t a, b, n, x, count, min, max, dist;

    chrono::high_resolution_clock::time_point start, end;
    /* start time measurement */
    start = chrono::high_resolution_clock::now();

    /* for each linear generator */
    for (size_t i = 0; i < num; ++i) {
        a = linear_generators[i][0];
        b = linear_generators[i][1];
        n = linear_generators[i][2];
        x = 0;
        count = 0;
        min = UINT32_MAX;
        max = 0;

        for (size_t j = 0; j < k; ++j) {
            /* compute next value */
            x = ((a * x + b) % (2 << (n - 1)));

            /* check if x is in interval */
            if (c <= x && x <= d)
                ++count;

            /* compute hamming distance */
            dist = x ^ e;
            dist = dist - ((dist >> 1) & 0x55555555);
            dist = (dist & 0x33333333) + ((dist >> 2) & 0x33333333);
            dist = (((dist + (dist >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;

            /* check minimal hamming distance */
            if (min > dist)
                min = dist;
            /* check maximal hamming distance */
            if (max < dist)
                max = dist;
        }

        /* use computed values so compiler does not exclude them */
        fprintf(stderr, "%" PRIu32 "%" PRIu32 "%" PRIu32, count, min, max);
    }

    /* end time measurement */
    end = chrono::high_resolution_clock::now();

    chrono::duration<double> time_span;
    time_span = chrono::duration_cast<chrono::duration<double>>(end - start);

    /* free array with linear generators */
    for (size_t i = 0; i < num; ++i)
        free(linear_generators[i]);
    free(linear_generators);

    return time_span.count();
}
