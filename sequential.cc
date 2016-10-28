#include <stdio.h>
#include <stdlib.h>
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
        unsigned long long int n
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

double compute(
        unsigned int **linear_generators,
        unsigned int num,
        unsigned int k,
        unsigned int c,
        unsigned int d,
        unsigned int e
        ) {
    std::chrono::high_resolution_clock::time_point start, end;
    /* start time measurement */
    start = std::chrono::high_resolution_clock::now();

    unsigned int a, b, n;
    unsigned int x;
    unsigned int count_in_interval;
    unsigned int min_distance, max_distance;
    unsigned int distance;
    /* for each linear generator */
    for (unsigned int i = 0; i < num; ++i) {
        a = linear_generators[i][0];
        b = linear_generators[i][1];
        n = linear_generators[i][2];
        x = 0;
        count_in_interval = 0;
        min_distance = UINT_MAX;
        max_distance = 0;
    
        for (unsigned int j = 0; j < k; ++j) {
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
    }

    /* end time measurement */
    end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> time_span;
    time_span = std::chrono::duration_cast<std::chrono::duration<double>>(
            end - start
            );

    return time_span.count();
}

/*
 * TODO
 * display better usage
 * use getopt
 */
void print_usage() {
    printf("./a.out FILE K C D E\n");
}

/* read input from file */
unsigned int **read_linear_generators(char *filename, unsigned int &num) {
    FILE *fp = fopen(filename, "r");
    if (fp == nullptr)
        return nullptr;

    fscanf(fp, "%u", &num);

    unsigned int **arr = new unsigned int*[num];
    for (unsigned int i = 0; i < num; ++i) {
        arr[i] = new unsigned int[3];
        fscanf(fp, "%u%u%u", &(arr[i][0]), &(arr[i][1]), &(arr[i][2]));
    }

    fclose(fp);
    
    return arr;
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        print_usage();
        return EXIT_FAILURE;
    }

    long int k = strtol(argv[2], nullptr, 10);
    long int c = strtol(argv[3], nullptr, 10);
    long int d = strtol(argv[4], nullptr, 10);
    long int e = strtol(argv[5], nullptr, 10);

    /* read linear generators */
    unsigned int num;
    unsigned int **linear_generators = read_linear_generators(argv[1], num);
    if (linear_generators == nullptr) {
        printf("cannot read the FILE\n");
        return EXIT_FAILURE;
    }

    /* start and print time of computation */
    printf("%lf\n", compute(linear_generators, num, k, c, d, e));

    /* free array with linear generators */
    for (unsigned int i = 0; i < num; ++i)
        delete [] linear_generators[i];
    delete [] linear_generators;

    return 0;
}
