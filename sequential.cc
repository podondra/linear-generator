#include <stdio.h>
#include <limits.h>
#include <chrono>

/*
 * linear generator x[i] = (a * x[i - 1] + b) mod (2 ^ n)
 *
 * a and b are odd numbers
 * 10 < n < 32
 * 0 < a, b < 2 ^ n, (b cannot be 0)
 * x[0] = 0, (seed)
 */
unsigned int linear_generator(
        unsigned long long int a,
        unsigned long long int prev,
        unsigned long long int b,
        unsigned int n
        ) {
    /*
     * unsigned long long int because of the (a * prev + b)
     * max value of a, prev and b is 2 ^ 31 - 1
     * (2 ^ 31 - 1) ^ 2 + 2 ^ 31 - 1 = 4611686016279904256
     * this fits into unsigned long long int
     *
     * 2 ^ n = 2 * 2 ^ (n - 1) = 2 << (n - 1)
     */
    return ((a * prev + b) % (2 << (n - 1)));
}

bool is_in_interval(unsigned int x, unsigned int start, unsigned int end) {
    return start <= x && x <= end;
}

/*
 * source: https://en.wikipedia.org/wiki/Hamming_distance
 * here the number of while loop iteration depends on e
 */
unsigned int hamming_distance(unsigned int x, unsigned int y) {
    unsigned int distance = 0;
    unsigned int xor_val = x ^ y;

    while (xor_val) { /* count the number of bits set */
        ++distance; /* a bit is set increment the counter */ 
        xor_val &= xor_val - 1; /* remove the counted bit */
    }

    return distance;
}

int main(int argc, char *argv[]) {
    /* TODO input parameters and input data */
    unsigned int a = 9;
    unsigned int b = 15;
    unsigned int n = 31;

    unsigned int k = 10000000;
    unsigned int c = 41832;
    unsigned int d = 321832;
    unsigned int e = 94321;

    /* input checks */
    if (a % 2 == 0 && b % 2 == 0) {
        printf("a and b must be odd numbers\n");
        return 1;
    }
    if (c > d) {
        printf("c must be less then or equal to d\n");
        return 1;
    }

    unsigned int x = 0;
    unsigned int count_in_interval = 0;
    unsigned int min_distance = UINT_MAX, max_distance = 0;
    unsigned int distance;
    
    /* start time measurement */
    std::chrono::high_resolution_clock::time_point start, end;
    start = std::chrono::high_resolution_clock::now();

    for (unsigned int i = 0; i < k; ++i) {
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

    /* end time measurement */
    end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> time_span;
    time_span = std::chrono::duration_cast<std::chrono::duration<double>>(
            end - start
            );

    printf("%lf\n", time_span.count());

    return 0;
}
