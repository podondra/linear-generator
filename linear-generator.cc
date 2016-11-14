#include <getopt.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "sequential.h"
#include "optimize-wrapper.h"

/* display better usage */
void usage(FILE *output) {
    fprintf(output,
            "Usage: ./linear-generator [VARIANT] -k NUM -n NUM\n"
            "Use VARIANT of solution to compute k iteration of n linear"
            " generators.\n"
            "\n"
            "Variant of solution is one of:\n"
            "  --seq\n"
            "  --opt\n"
            "  --par\n"
            );
}

int main(int argc, char *argv[]) {
    int variant_flag = 0;
    unsigned int k = 0, n = 0;

    /* getopt */
    while (true) {
        static struct option long_options[] = {
            /* these options set a flag */
            {"seq", no_argument, &variant_flag, 0},
            {"opt", no_argument, &variant_flag, 1},
            {"par", no_argument, &variant_flag, 2},
            {"help", no_argument, nullptr, 'h'},
            {0, 0, 0, 0}
        };

        /* getopt_long stores the option index here */
        int option_index = 0;

        int ch = getopt_long(
                argc,
                argv,
                "hk:n:",
                long_options,
                &option_index
                );

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
            case 'n':
                n = strtol(optarg, nullptr, 10);
                break;
            default:
                break;
        }
    }

    /* check getopt values */
    if (k == 0 || n == 0) {
        usage(stdout);
        return EXIT_FAILURE;
    }
    if (optind != argc) {
        usage(stdout);
        return EXIT_FAILURE;
    }

    std::random_device r;
    std::default_random_engine engine(r());

    double time = 0;
    switch (variant_flag) {
        case 0:
            time = sequential(&engine, n, k);
            break;
        case 1:
            time = optimize(&engine, n, k);
            break;
        case 2:
            fprintf(stdout, "not implemented yet\n");
            break;
    }
    if (time != 0)
        fprintf(stdout, "%lf\n", time);

    return EXIT_SUCCESS;
}
