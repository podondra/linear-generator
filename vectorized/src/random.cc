#include <cstdint>
#include <random>

uint32_t random_num(std::default_random_engine *e) {
    return (uint32_t)(*e)();
}

uint32_t random_odd_num(std::default_random_engine *e) {
    return random_num(e) | 0x00000001;
}

uint32_t random_n(std::default_random_engine *e) {
    return (random_num(e) % 21) + 11;
}
