#include <inttypes.h>
#include <limits.h>
#include <stdio.h>

#include <chrono>

/*
 * linear generator x[i] = (a * x[i - 1] + b) mod (2 ^ n)
 *
 * a and b are odd numbers
 * 10 < n < 32
 * 0 < a, b < 2 ^ n, (b cannot be 0)
 * x[0] = 0, (seed)
 */
uint32_t linear_generator(uint64_t a, uint64_t prev, uint64_t b, uint64_t n) {
    /*
     * unsigned long long int because of the (a * prev + b)
     * max value of a, prev and b is 2 ^ (31 - 1)
     * (2 ^ 31 - 1) ^ 2 + 2 ^ 31 - 1 = 4611686016279904256
     * this fits into unsigned long long int
     *
     * 2 ^ n = 2 * 2 ^ (n - 1) = 2 << (n - 1)
     */
    return ((a * prev + b) % (2 << (n - 1)));
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
uint32_t **read_sequential(FILE *file, uint32_t &num) {
    /* first in file is number of linear generators */
    if (fscanf(file, "%" SCNu32, &num) != 1)
        return nullptr;

    /*
     * then there in num of lines each containing three numbers
     * a, b, c separated by spaces
     */
    uint32_t **arr = new uint32_t*[num];
    if (arr == nullptr)
        return nullptr;
    for (uint32_t i = 0; i < num; ++i) {
        arr[i] = new uint32_t[3];
        if (arr[i] == nullptr)
            return nullptr;
        if (fscanf(file, "%" SCNu32 "%" SCNu32 "%" SCNu32,
                    arr[i], arr[i] + 1, arr[i] + 2) != 3)
            return nullptr;
    }

    return arr;
}

double sequential(FILE *input, uint32_t k, uint32_t c, uint32_t d, uint32_t e) {
    /* read linear generators */
    uint32_t num;
    uint32_t **linear_generators = read_sequential(input, num);
    if (linear_generators == nullptr) {
        fprintf(stdout, "Input file reading error.\n");
        return 0;
    }

    std::chrono::high_resolution_clock::time_point start, end;
    /* start time measurement */
    start = std::chrono::high_resolution_clock::now();

    uint32_t a, b, n;
    uint32_t x;
    uint32_t count_in_interval;
    uint32_t min_distance, max_distance;
    uint32_t distance;
    /* for each linear generator */
    for (uint32_t i = 0; i < num; ++i) {
        a = linear_generators[i][0];
        b = linear_generators[i][1];
        n = linear_generators[i][2];
        x = 0;
        count_in_interval = 0;
        min_distance = UINT_MAX;
        max_distance = 0;
    
        for (uint32_t j = 0; j < k; ++j) {
            /* compute next value */
            x = linear_generator(a, x, b, n);

            /* check if x is in interval */
            if (is_in_interval(x, c, d))
                ++count_in_interval;

            /* compute hamming distance */
            distance = hamming_distance(x, e);
            /* check minimal hamming distance */
            if (min_distance > distance)
                min_distance = distance;
            /* check maximal hamming distance */
            if (max_distance < distance)
                max_distance = distance;
        }

        /* use computed values so compiler does not exclude them */
        fprintf(stderr,
                "%" PRIu32 "%" PRIu32 "%" PRIu32,
                count_in_interval, min_distance, max_distance);
    }

    /* end time measurement */
    end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> time_span;
    time_span = std::chrono::duration_cast<std::chrono::duration<double>>(
            end - start
            );

    /* free array with linear generators */
    for (uint32_t i = 0; i < num; ++i)
        delete [] linear_generators[i];
    delete [] linear_generators;

    return time_span.count();
}
