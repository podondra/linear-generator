#include <stdint.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "sequential.h"
#include "optimize.h"

/* display better usage */
void usage(FILE *output) {
    fprintf(output,
            "Usage: ./linear-generator [VARIANT] PARAMS FILE 2> /dev/null\n"
            "Use VARIANT of solution to compute input from FILE with PARAMS.\n"
            "\n"
            "Variant of solution is one of:\n"
            "  --sequential\n"
            "  --optimize\n"
            "  --parallel\n"
            "\n"
            "Parameters for computation:\n"
            "  -k NUM\n"
            "  -c NUM\n"
            "  -d NUM\n"
            "  -e NUM\n"
            );
}

int main(int argc, char *argv[]) {
    int variant_flag = 0;
    int32_t k = -1;
    int32_t c = -1;
    int32_t d = -1;
    int32_t e = -1;

    /* getopt */
    while (true) {
        static struct option long_options[] = {
            /* these options set a flag */
            {"sequential", no_argument, &variant_flag, 0},
            {"optimize", no_argument, &variant_flag, 1},
            {"parallel", no_argument, &variant_flag, 2},
            {"help", no_argument, nullptr, 'h'},
            {0, 0, 0, 0}
        };

        /* getopt_long stores the option index here */
        int option_index = 0;

        int ch = getopt_long(argc, argv, "hk:c:d:e:", long_options,
                &option_index);

        /* detect the end of the options */
        if (ch == -1)
            break;

        switch (ch) {
            case 'h':
                usage(stdout);
                return EXIT_SUCCESS;
            case 'k':
                k = strtol(optarg, nullptr, 10);
                break;
            case 'c':
                c = strtol(optarg, nullptr, 10);
                break;
            case 'd':
                d = strtol(optarg, nullptr, 10);
                break;
            case 'e':
                e = strtol(optarg, nullptr, 10);
                break;
            default:
                break;
        }
    }

    /* check getopt values */
    if (k == -1 || c == -1 || d == -1 || e == -1) {
        usage(stderr);
        return EXIT_FAILURE;
    }
    if (optind != argc - 1) {
        usage(stderr);
        return EXIT_FAILURE;
    }

    /* open input file */
    FILE *input = fopen(argv[argc - 1], "r");
    if (input == nullptr) {
        fprintf(stdout, "Cannot open input file.\n");
        usage(stderr);
        return EXIT_FAILURE;
    }

    double time = 0;
    switch (variant_flag) {
        case 0:
            time = sequential(input, k, c, d, e);
            break;
        case 1:
            time = optimize(input, k, c, d, e);
            break;
        case 2:
            fprintf(stdout, "not implemented yet\n");
            break;
    }
    if (time != 0)
        fprintf(stdout, "%lf\n", time);

    /* close input file */
    fclose(input);

    return EXIT_SUCCESS;
}
