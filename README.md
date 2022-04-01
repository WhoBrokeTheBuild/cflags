
# cflags

Command line flag parsing library in C

Heavily inspired by Go's `flag` package
https://golang.org/pkg/flag/

## Building

cflags is a header only library. To use it, simply copy `cflags.h` or `cflags.hpp` to your project, or add it to your include path.

You may also install it using cmake, like so:

```
cmake path/to/source
sudo make install
```

This will install both CMake and pkg-config configuration files.

## Argument Parsing Logic

* The first argument is stored in `program`
* The following arguments are parsed left to right
* If an argument does not start with `-`, it is placed in the additional arguments list stored in `args/argv`
* If the special `--` argument appears, all following arguments are treated as positional  
  e.g. `-c 4 -- --name hello` would parse the `-c`, but place `--name` and `hello` into `args/argv`
* Arguments starting with `--` are long name flags, e.g. `--example`
  * The list of flags is searched for one with `long_name` equal to the argument name (after the `--`), e.g. `long_name == example`
  * If a flag is not found with that name, an error is printed and `parse()` returns false
* Arguments starting with just `-` are short name flags, e.g. `-xvf`
  * These can be grouped together, so they are searched one at a time from left to right, e.g. `x`, `v`, then `f`
  * If any of these fail to match a flag, an error is printed and `parse()` returns false
* Once a flag is found, it attempts to find a value
  * Arguments with long names can also come in the forms `--name`, `--name=value`, or `--name value`
  * Arguemnts with short names can come in the forms `-n`, or `-n value`
    * Note: Only the last short flag of a group can have a value, e.g. `-xvf file` will work, but `-xfv file` will fail
  * If the flag is of type `[c]string`, `int`, or `float` then a value is required, and if one is not found an error is printed and `parse()` returns false
    * Arguments of type `bool` can have a value, e.g. `--debug=false`, but one is not required
  * Each time a flag is encountered, the `count` member is incremented
  * The value for a flag is overwritten each time the flag is processed, the last argument parsed wins, e.g. `-c 4 -c 10` will result in `-c` being 10
    * If you want to capture each argument separately, use `add_*_callback` instead

## Usage (C)

```cpp
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
    cflags_add_bool(flags, 'd', "debug", &debug, "enable debug mode");

    // Add a similar help flag, which will be callable with just --help
    bool help = false;
    cflags_add_bool(flags, '\0', "help", &help, "print this text and exit");

    // Add a string flag
    const char * string = NULL;
    cflags_add_string(flags, 's', "string", &string, "enter a string");

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
    if (!cflags_parse(flags, argc, argv) || help || flags->argc == 0) {
        cflags_print_usage(flags,
            "[OPTION]... [ARG]...", 
            "Tests the cflags library.", 
            "Additional information about this library can be found by at:\n"
            "  https://github.com/WhoBrokeTheBuild/cflags");
    }

    printf("debug: %d\n", debug);
    printf("string: %s\n", string);
    printf("count: %d\n", count);
    printf("amount: %f\n", amount);

    // Print the number of times verbose was added
    printf("verbosity: %d\n", verbose->count);

    // Print any additional arguments, in the order they were parsed
    for (int i = 1; i < flags->argc; ++i) {
        printf("positional arg %d: %s\n", i, flags->argv[i]);
    }

    // Cleanup
    cflags_free(flags);

    return 0;
}
```

## Usage (C++)

```cpp
#include <cflags.hpp>

void process_string(std::string str)
{
    printf("processing %s\n", str.c_str());
}

void process_cstring(const char * str)
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

int main(int argc, char * argv[])
{
    // Create a cflags object
    cflags::cflags flags;

    // Add a bool flag, which will be callable with -d or --debug
    // The value will be true if it exists, and can bet set to false
    // by saying -d false or --debug=false
    bool debug = false;
    flags.add_bool('d', "debug", &debug, "enable debug mode");

    // Add a similar help flag, which will be callable with just --help
    bool help = false;
    flags.add_bool('\0', "help", &help, "print this text and exit");

    // Add a string flag
    std::string string;
    flags.add_string('s', "string", &string, "enter a string");

    // Add a cstring flag
    const char * cstring = NULL;
    flags.add_cstring('\0', "cstring", &cstring, "enter a string (cstring)");

    // Add an int flag
    int count = 0;
    flags.add_int('c', "count", &count, "enter a number");

    // Add a float flag
    float amount = 0.f;
    flags.add_float('a', "amount", &amount, "enter a float");

    // Add a string callback flag. This will call the supplied function with the value
    // when it is parsed
    flags.add_string_callback('f', "file", &process_string, "process a file");

    flags.add_cstring_callback('\0', "cfile", &process_cstring, "process a file (cstring)");

    flags.add_bool_callback('q', "bool-flag", &process_bool, "process a bool");

    flags.add_int_callback('w', "int-flag", &process_int, "process a int");

    flags.add_float_callback('e', "float-flag", &process_float, "process a float");

    // You can also use lambdas
    flags.add_string_callback('l', "lambda",
        [](std::string value) {
            printf("Hello %s\n", value.c_str());
        },
        "use a lambda function"
    );

    // Add a flag that can be called multiple times
    auto verbose = flags.add_bool('v', "verbose", NULL, "enables verbose output, repeat up to 4 times for more verbosity");

    // Parse the command arguments
    if (!flags.parse(argc, argv) || help || flags.argc == 0) {
        flags.print_usage(
            "[OPTION]... [ARG]...", 
            "Tests the cflags library.", 
            "Additional information about this library can be found by at:\n"
            "  https://github.com/WhoBrokeTheBuild/cflags");
    }

    printf("debug: %d\n", debug);
    printf("string: %s\n", string.c_str());
    printf("cstring: %s\n", cstring);
    printf("count: %d\n", count);
    printf("amount: %f\n", amount);

    // Print the number of times verbose was added
    printf("verbosity: %d\n", verbose->count);

    // Print any additional arguments, in the order they were parsed
    for (auto& arg : flags.args) {
        printf("positional arg %s\n", arg.data());
    }

    // For backwards compatability, the additional arguments are also exposed in argc/argv
    for (int i = 0; i < flags.argc; ++i) {
        printf("positional arg %d: %s\n", i, flags.argv[i]);
    }

    return 0;
}
```

## Quirks

### 1. Only the last short-name argument in a group may have a value.

For example:
```cpp
flags.add_string('f', "file", parse_filename, "parse a filename");
flags.add_bool('d', "debug", &debug, "enable debug mode");

// These will work
test -df test.txt
test -df test.txt -f test2.txt

// And these will fail
test -fd test.txt
test -ff test.txt test2.txt
```

### 2. Any `char *` strings point to the original memory from `argv`

Call the following code as `program -s hello`
```cpp
int main(int argc, char ** argv)
{
    for (int i = 1; i < argc; ++i) {
        printf("argv[%d] @ %p: %s\n", i, argv[i], argv[i]);
    }

    cflags_t * flags = cflags_init();

    const char * string = NULL;
    cflags_add_string(flags, 's', "string", &string, "enter a string");

    cflags_parse(flags, argc, argv);

    // The address pointed to by `string` is the same memory pointed to by argv
    printf("string @ %p: %s\n", string, string);
    
    cflags_free(flags);

    return 0;
}
```

Example output
```
argv[1] @ 0000024F6BDD665F: -s
argv[2] @ 0000024F6BDD6662: hello
string @ 0000024F6BDD6662: hello
```

When parsing arguments in the form `--name=value`, the memory pointed to by `argv` is altered and the `=` is replaced by a `\0`.

When using the C++ version, arguments as `std::string` do not point at `argv` as their memory gets copied. 


