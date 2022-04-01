#include "cflags.hpp"

void parse_file(const char * filename)
{
    printf("parsing %s\n", filename);
}

int main(int argc, char * argv[])
{
    cflags::cflags flags;

    bool help = false;
    flags.add_bool('\0', "help", &help, "display this help and exit");

    bool debug = false;
    flags.add_bool('d', "debug", &debug, "enable debug mode");

    int count = 0;
    flags.add_int('c', "count", &count, "enter a number");

    float amount = 0.f;
    flags.add_float('a', "amount", &amount, "enter a float");

    flags.add_bool('q', "really-long-argument-name", nullptr, "testing really long argument names");

    flags.add_cstring_callback('f', "file", &parse_file, "process a file");

    flags.add_string_callback('n', "name", 
        [](std::string name) {
            printf("Hello %s\n", name.c_str());
        },
        "say hello to name");

    auto verbose = flags.add_bool('v', "verbose", NULL, "enables verbose output, repeat up to 4 times for more verbosity");

    if (!flags.parse(argc, argv) || help || argc == 1) {
        flags.print_usage(
            "[OPTION]... [ARG]...", 
            "Tests the cflags library.", 
            "Additional information about this library can be found by contacting:\n"
            "  sdl.slane@gmail.com");
        return 1;
    }

    printf("help: %d\n", help);
    printf("debug: %d\n", debug);

    printf("count: %d\n", count);
    printf("amount: %f\n", amount);

    printf("verbosity: %d\n", verbose->count);

    printf("args:\n");
    for (auto& arg : flags.args) {
        printf("Positional %s\n", arg.data());
    }

    printf("argc/argv:\n");
    for (int i = 0; i < flags.argc; ++i) {
        printf("positional %d: %s\n", i, flags.argv[i]);
    }

    return 0;
}