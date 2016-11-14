#include <stdint.h>

void optimized_loop(
        uint32_t num,
        uint32_t k,
        uint32_t c,
        uint32_t d,
        uint32_t e,
        uint32_t *__restrict a,
        uint32_t *__restrict b,
        uint32_t *__restrict n, 
        float *__restrict n_inv,
        uint32_t *__restrict x,
        uint32_t *__restrict count,
        uint32_t *__restrict min,
        uint32_t *__restrict max
        );
