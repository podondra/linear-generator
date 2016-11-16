#include <cstdint>
#include <random>

using namespace std;

uint32_t random_num(default_random_engine *e) {
    return (uint32_t)(*e)();
}

uint32_t random_odd_num(default_random_engine *e) {
    return random_num(e) | 0x00000001;
}

/* generate uint32_t 10 < n < 32 */
uint32_t random_n(default_random_engine *e) {
    return (random_num(e) % 21) + 11;
}
