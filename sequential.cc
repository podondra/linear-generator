#include <cinttypes>
#include <climits>
#include <cstdio>
#include <chrono>
#include <random>

#include "random.h"

/*
 * linear generator x[i] = (a * x[i - 1] + b) mod (2 ^ n)
 * a and b are odd numbers
 * 10 < n < 32
 * x[0] = 0, (seed)
 */
uint32_t linear_generator(uint32_t a, uint32_t x, uint32_t b, uint32_t n) {
    return ((a * x + b) % (2 << (n - 1)));
}

bool is_in_interval(uint32_t x, uint32_t start, uint32_t end) {
    return start <= x && x <= end;
}

/*
 * source: https://en.wikipedia.org/wiki/Hamming_distance
 * here the number of while loop iteration depends on e
 */
uint32_t hamming_distance(uint32_t x, uint32_t y) {
    uint32_t distance = 0;
    uint32_t xor_val = x ^ y;

    while (xor_val) { /* count the number of bits set */
        ++distance; /* a bit is set increment the counter */ 
        xor_val &= xor_val - 1; /* remove the counted bit */
    }

    return distance;
}

/* parse input */
uint32_t **gen_sequential(std::default_random_engine *engine, uint32_t num) {
    uint32_t **arr = new uint32_t*[num];
    if (arr == nullptr)
        return nullptr;
    for (size_t i = 0; i < num; ++i) {
        arr[i] = new uint32_t[3];
        if (arr[i] == nullptr)
            return nullptr;
        arr[i][0] = random_odd_num(engine);
        arr[i][1] = random_odd_num(engine);
        arr[i][2] = random_n(engine);
    }
    return arr;
}

double sequential(
        std::default_random_engine *engine,
        uint32_t k,
        uint32_t num
        ) {
    /* read linear generators */
    uint32_t **linear_generators = gen_sequential(engine, num);
    if (linear_generators == nullptr) {
        fprintf(stdout, "Not enough memory.\n");
        return 0;
    }

    uint32_t c = random_num(engine);
    uint32_t d = random_num(engine);
    uint32_t e = random_num(engine);

    uint32_t a, b, n, x, count, min, max, dist;

    std::chrono::high_resolution_clock::time_point start, end;
    /* start time measurement */
    start = std::chrono::high_resolution_clock::now();

    /* for each linear generator */
    for (size_t i = 0; i < num; ++i) {
        a = linear_generators[i][0];
        b = linear_generators[i][1];
        n = linear_generators[i][2];
        x = 0;
        count = 0;
        min = UINT_MAX;
        max = 0;
    
        for (size_t j = 0; j < k; ++j) {
            /* compute next value */
            x = linear_generator(a, x, b, n);

            /* check if x is in interval */
            if (is_in_interval(x, c, d))
                ++count;

            /* compute hamming distance */
            dist = hamming_distance(x, e);
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
    end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> time_span;
    time_span = std::chrono::duration_cast<std::chrono::duration<double>>(
            end - start
            );

    /* free array with linear generators */
    for (size_t i = 0; i < num; ++i)
        delete [] linear_generators[i];
    delete [] linear_generators;

    return time_span.count();
}
