#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include "seq.h"
#include "opt.h"

using namespace std;

void usage(FILE *output) {
    fprintf(output,
            "Usage: ./lg [VARIANT] -k NUM -n NUM 2> /dev/null\n"
            "Use VARIANT of solution to compute k iteration of n linear"
            " generators.\n"
            "\n"
            "Variant is one of:\n"
            "  --seq\n"
            "  --opt\n"
            "  --par\n"
           );
}

int main(int argc, char *argv[]) {
    int variant_flag = 0;
    int k = 0, num = 0;

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

        int ch = getopt_long(argc, argv, "hk:n:", long_options, &option_index);

        /* detect the end of the options */
        if (ch == -1)
            break;

        switch (ch) {
            case 'h':
                usage(stderr);
                return EXIT_SUCCESS;
            case 'k':
                k = std::strtol(optarg, nullptr, 10);
                break;
            case 'n':
                num = std::strtol(optarg, nullptr, 10);
                break;
            default:
                break;
        }
    }

    /* check getopt values */
    if (k == 0 || num == 0 || optind != argc) {
        usage(stderr);
        return EXIT_FAILURE;
    }

    /* create random engine for input generation */
    std::random_device r;
    std::default_random_engine engine(r());

    double time = 0;
    switch (variant_flag) {
        case 0:
            time = seq(&engine, num, k);
            break;
        case 1:
            time = opt(&engine, num, k);
            break;
        case 2:
            fprintf(stderr, "Not implemented yet.\n");
            break;
    }

    if (time != 0)
        fprintf(stderr, "%lf\n", time);

    return EXIT_SUCCESS;
}
