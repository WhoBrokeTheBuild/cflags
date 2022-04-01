#include "cflags.h"
#include <stdio.h>

void parse_file(const char * filename)
{
    printf("parsing %s\n", filename);
}

int main(int argc, char** argv) 
{
    cflags_t * flags = cflags_init();

    bool help = false;
    cflags_add_bool(flags, '\0', "help", &help, "display this help and exit");

    bool debug = false;
    cflags_add_bool(flags, 'd', "debug", &debug, "enable debug mode");

    int count = 0;
    cflags_add_int(flags, 'c', "count", &count, "enter a number");

    float amount = 0.f;
    cflags_add_float(flags, 'a', "amount", &amount, "enter a float");

    cflags_add_bool(flags, 'q', "really-long-argument-name", NULL, "testing really long argument names");

    cflags_add_string_callback(flags, 'f', "file", &parse_file, "process a file");

    cflags_flag_t * verbose = cflags_add_bool(flags, 'v', "verbose", NULL, "enables verbose output, repeat up to 4 times for more verbosity");

    // Parse the command arguments
    if (!cflags_parse(flags, argc, argv) || help || argc == 1) {
        cflags_print_usage(flags,
            "[OPTION]... [ARG]...", 
            "Tests the cflags library.", 
            "Additional information about this library can be found by at:\n"
            "  https://github.com/WhoBrokeTheBuild/cflags");
    }

    printf("help: %d\n", help);
    printf("debug: %d\n", debug);

    printf("count: %d\n", count);
    printf("amount: %f\n", amount);

    printf("verbosity: %d\n", verbose->count);

    printf("argc/argv:\n");
    for (int i = 0; i < flags->argc; ++i) {
        printf("positional %d: %s\n", i, flags->argv[i]);
    }

    cflags_free(flags);

    return 0;
}
