#include <cmath>
#include <cstdint>
#include <cstdlib>

void optimized_loop(
        uint32_t num,
        uint32_t k,
        uint32_t c,
        uint32_t d,
        uint32_t e,
        uint32_t *__restrict a,
        uint32_t *__restrict b,
        float *__restrict n,
        uint32_t *__restrict x,
        uint32_t *__restrict count,
        uint32_t *__restrict min,
        uint32_t *__restrict max
        ) {
    /* TODO add comments */
    a = (uint32_t *)__builtin_assume_aligned(a, 32);
    b = (uint32_t *)__builtin_assume_aligned(b, 32);
    n = (float *)__builtin_assume_aligned(n, 32);
    x = (uint32_t *)__builtin_assume_aligned(x, 32);
    count = (uint32_t *)__builtin_assume_aligned(count, 32);
    min = (uint32_t *)__builtin_assume_aligned(min, 32);
    max = (uint32_t *)__builtin_assume_aligned(max, 32);

    for (size_t j = 0; j < num; ++j) {
        n[j] = 1.f / std::exp2(n[j]);
    }

    uint32_t dist;
    for (size_t i = 0; i < k; ++i) {
        for (size_t j = 0; j < num; ++j) {
            x[j] = a[j] * x[j] + b[j];
            x[j] -= ((uint32_t)(x[j] * n[j])) * n[j];

            count[j] += ((c <= x[j] && x[j] <= d) ? 1 : 0);

            dist = x[j] ^ e;
            dist = dist - ((dist >> 1) & 0x55555555);
            dist = (dist & 0x33333333) + ((dist >> 2) & 0x33333333);
            dist = (((dist + (dist >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;

            min[j] = (min[j] < dist) ? min[j] : dist;
            max[j] = (max[j] > dist) ? max[j] : dist;
        }
    }
}
