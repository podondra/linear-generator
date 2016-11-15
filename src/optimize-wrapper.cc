#include <cinttypes>
#include <climits>
#include <cstdint>
#include <cstdio>
#include <chrono>
#include <random>

#include "optimize.h"
#include "random.h"

/* parse input */
uint32_t gen_optimize(
        std::default_random_engine *engine,
        uint32_t num,
        uint32_t **a,
        uint32_t **b,
        float **n,
        uint32_t **x,
        uint32_t **count,
        uint32_t **min,
        uint32_t **max
        ) {
    *a = (uint32_t *)aligned_alloc(32, num * sizeof(*a));
    *b = (uint32_t *)aligned_alloc(32, num * sizeof(*b));
    *n = (float *)aligned_alloc(32, num * sizeof(*n));
    *x = (uint32_t *)aligned_alloc(32, num * sizeof(*x));
    *count = (uint32_t *)aligned_alloc(32, num * sizeof(*count));
    *min = (uint32_t *)aligned_alloc(32, num * sizeof(*min));
    *max = (uint32_t *)aligned_alloc(32, num * sizeof(*max));


    if (!a || !b || !n || !x || !count || !min || !max)
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

double optimize(std::default_random_engine *engine, uint32_t k, uint32_t num) {
    /* read linear generators */
    uint32_t *a, *b, *x, *count, *min, *max;
    float *n;
    if (gen_optimize(engine, num, &a, &b, &n, &x, &count, &min, &max) != true) {
        fprintf(stdout, "Not enough memory.\n");
        return 0;
    }

    uint32_t c = random_num(engine);
    uint32_t d = random_num(engine);
    uint32_t e = random_num(engine);

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
        fprintf(stderr,
                "%" PRIu32 "%" PRIu32 "%" PRIu32,
                count[i], min[i], max[i]);

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
