#include <inttypes.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

#include <algorithm>
#include <chrono>

/* parse input */
uint32_t **read_optimize(FILE *file, uint32_t &num) {
    /* first in file is number of linear generators */
    if (fscanf(file, "%" SCNu32, &num) != 1)
        return nullptr;

    /*
     * then there in num of lines each containing three numbers
     * a, b, c separated by spaces
     */
    uint32_t **lgs = new uint32_t*[num];
    if (lgs == nullptr)
        return nullptr;
    for (uint32_t i = 0; i < num; ++i) {
        lgs[i] = new uint32_t[7];
        if (lgs[i] == nullptr)
            return nullptr;
        if (fscanf(file, "%" SCNu32 "%" SCNu32 "%" SCNu32,
                    lgs[i], lgs[i] + 1, lgs[i] + 2) != 3)
            return nullptr;
        /* x */
        lgs[i][3] = 0;
        /* count in interval */
        lgs[i][4] = 0;
        /* minimal distance */
        lgs[i][5] = UINT32_MAX;
        /* maximal distance */
        lgs[i][6] = 0;
    }

    return lgs;
}

double optimize(FILE *input, uint32_t k, uint32_t c, uint32_t d, uint32_t e) {
    /* read linear generators */
    uint32_t num;
    uint32_t **lgs = read_optimize(input, num);
    if (lgs == nullptr) {
        fprintf(stdout, "Input file reading error.\n");
        return 0;
    }

    uint32_t dist;

    std::chrono::high_resolution_clock::time_point start, end;
    /* start time measurement */
    start = std::chrono::high_resolution_clock::now();

    for (uint32_t i = 0; i < k; ++i) {
        for (uint32_t j = 0; j < num; ++j) {
            lgs[j][3] = (
                    lgs[j][0] * lgs[j][3] + lgs[j][1]) % (2 << (lgs[j][2] - 1)
                        );

            lgs[j][4] += (c <= lgs[j][3] && lgs[j][3] <= d) ? 1 : 0;

            /* population count */
            dist = lgs[j][3] ^ e;
            dist = dist - ((dist >> 1) & 0x55555555);
            dist = (dist & 0x33333333) + ((dist >> 2) & 0x33333333);
            dist = (((dist + (dist >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;

            lgs[j][5] = std::min(lgs[j][5], dist);
            lgs[j][6] = std::max(lgs[j][6], dist);
        }

    }

    /* end time measurement */
    end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> time_span;
    time_span = std::chrono::duration_cast<std::chrono::duration<double>>(
            end - start
            );

    /* use computed values so compiler does not exclude them */
    for (uint32_t i = 0; i < num; ++i)
        fprintf(stderr,
                "%" PRIu32 "%" PRIu32 "%" PRIu32,
                lgs[i][4], lgs[i][5], lgs[i][6]);

    /* free array with linear generators */
    for (uint32_t i = 0; i < num; ++i)
        delete [] lgs[i];
    delete [] lgs;

    return time_span.count();
}
