
# cFlags

## Building

cFlags is a header only library. To use it, simply copy `cflags.h` to your project, or add it to your include path.

You may also install it using cmake, like so:

```
cmake path/to/source
sudo make install
```

This will install both CMake and pkg-config configuration files.

## Usage

```c
#include <cflags.h>

void process_string(const char * str)
{
    printf("processing %s\n", str);
}

void process_bool(bool b)
{
    printf("processing %d\n", b);
}

void process_int(int i)
{
    printf("processing %d\n", i);
}

void process_float(float f)
{
    printf("processing %f\n", f);
}

int main(int argc, char** argv)
{
    // Create a cflags object
    cflags_t * flags = cflags_init();

    // Add a bool flag, which will be callable with -d or --debug
    // The value will be true if it exists, and can bet set to false
    // by saying -d false or --debug=false
    bool debug = false;
    cflags_add_bool(flags, 'd', "debug", &debug, "enable debug mode")

    // Add a similar help flag, which will be callable with just --help
    bool help = false;
    cflags_add_bool(flags, '\0', "help", &help, "print this text and exit");

    // Add a string flag
    const char * string = NULL;
    cflags_add_string(flags, 's', "string", &string, "enter a string")

    // Add an int flag
    int count = 0;
    cflags_add_int(flags, 'c', "count", &count, "enter a number");

    // Add a float flag
    float amount = 0.f;
    cflags_add_float(flags, 'a', "amount", &amount, "enter a float");

    // Add a string callback flag. This will call the supplied function with the value
    // when it is parsed
    cflags_add_string_callback(flags, 'f', "file", &process_string, "process a file");

    cflags_add_bool_callback(flags, 'q', "bool-flag", &process_bool, "process a bool");

    cflags_add_int_callback(flags, 'w', "int-flag", &process_int, "process a int");

    cflags_add_float_callback(flags, 'e', "float-flag", &process_float, "process a float");

    // Add a flag that can be called multiple times
    cflags_flag_t * verbose = cflags_add_bool(flags, 'v', "verbose", NULL, "enables verbose output, repeat up to 4 times for more verbosity");

    // Parse the command arguments
    cflags_parse(flags, argc, argv);

    // Print the number of times verbose was added
    printf("verbosity: %d\n", verbose->count);

    // Print any additional arguments, in the order they were parsed
    for (int i = 0; i < flags->argc; ++i) {
        printf("positional arg %d: %s\n", i, flags->argv[i]);
    }

    if (help || flags->argc == 0) {
        cflags_print_usage(flags,
            "[OPTION]... [ARG]...", 
            "Tests the cflags library.", 
            "Additional information about this library can be found by at:\n"
            "  https://github.com/WhoBrokeTheBuild/cFlags");
    }

    // Cleanup
    cflags_free(flags);

    return 0;
}
```
