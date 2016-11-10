#include <inttypes.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

#include <chrono>

#include "optimize.h"

/* parse input */
uint32_t read_optimize(
        FILE *file,
        uint32_t **a,
        uint32_t **b,
        uint32_t **n,
        uint32_t **x,
        uint32_t **count,
        uint32_t **min,
        uint32_t **max
        ) {
    /* first in file is number of linear generators */
    uint32_t num;
    if (fscanf(file, "%" SCNu32, &num) != 1)
        return 0;

    /*
     * then there in num of lines each containing three numbers
     * a, b, n separated by spaces
     */
    *a = new uint32_t[num];
    *b = new uint32_t[num];
    *n = new uint32_t[num];
    *x = new uint32_t[num];
    *count = new uint32_t[num];
    *min = new uint32_t[num];
    *max = new uint32_t[num];
    if (!a || !b || !n || !x || !count || !min || !max)
        return 0;
    for (uint32_t i = 0; i < num; ++i) {
        if (fscanf(file, "%" SCNu32 "%" SCNu32 "%" SCNu32,
                    (*a) + i, (*b) + i, (*n) + i) != 3)
            return 0;
        (*x)[i] = 0;
        (*count)[i] = 0;
        (*min)[i] = UINT32_MAX;
        (*max)[i] = 0;
    }

    return num;
}

double optimize(FILE *input, uint32_t k, uint32_t c, uint32_t d, uint32_t e) {
    /* read linear generators */
    uint32_t *a, *b, *n, *x, *count, *min, *max;
    uint32_t num = read_optimize(input, &a, &b, &n, &x, &count, &min, &max);
    if (num == 0) {
        fprintf(stdout, "Input file reading error.\n");
        return 0;
    }

    std::chrono::high_resolution_clock::time_point start, end;
    /* start time measurement */
    start = std::chrono::high_resolution_clock::now();

    optimized_loop(num, k, c, d, e, a, b, n, x, count, min, max);

    /* end time measurement */
    end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> time_span;
    time_span = std::chrono::duration_cast<std::chrono::duration<double>>(
            end - start
            );

    /* use computed values so compiler does not exclude them */
    for (size_t i = 0; i < num; ++i)
        fprintf(stderr, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
                count[i], min[i], max[i]);

    /* free arrays */
    delete [] a;
    delete [] b;
    delete [] n;
    delete [] x;
    delete [] count;
    delete [] min;
    delete [] max;

    return time_span.count();
}
