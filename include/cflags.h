#ifndef CFLAGS_H
#define CFLAGS_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum cflags_type
{
    CFLAGS_TYPE_UNDEFINED = -1,
    CFLAGS_TYPE_STRING,
    CFLAGS_TYPE_BOOL,
    CFLAGS_TYPE_INT,
    CFLAGS_TYPE_FLOAT,
    CFLAGS_TYPE_STRING_CALLBACK,
    CFLAGS_TYPE_BOOL_CALLBACK,
    CFLAGS_TYPE_INT_CALLBACK,
    CFLAGS_TYPE_FLOAT_CALLBACK,
};

typedef enum cflags_type cflags_type_t;

struct cflags_flag
{
    char            short_name;
    const char *    long_name;
    const char *    description;

    cflags_type_t   type;
    unsigned        count;

    struct cflags_flag * next;

    union {
        const char **   string_value;
        bool *          bool_value;
        int *           int_value;
        float *         float_value;
        void (*string_callback)(const char *);
        void (*bool_callback)(bool);
        void (*int_callback)(int);
        void (*float_callback)(float);
    };
};

typedef struct cflags_flag cflags_flag_t;

struct cflags
{
    const char * program;

    int     argc;
    char ** argv;

    cflags_flag_t * first_flag;
};

typedef struct cflags cflags_t;

static cflags_t * cflags_init()
{
    cflags_t * flags = malloc(sizeof(cflags_t));
    flags->program = NULL;
    flags->argc = 0;
    flags->argv = NULL;
    flags->first_flag = NULL;
    return flags;
}

cflags_flag_t * _cflags_add_flag(cflags_t * flags)
{
    cflags_flag_t * new_flag = NULL;

    if (flags->first_flag) {
        cflags_flag_t * flag = flags->first_flag;
        while (flag->next) {
            flag = flag->next;
        }
        flag->next = malloc(sizeof(cflags_flag_t));
        new_flag = flag->next;
    }
    else {
        flags->first_flag = malloc(sizeof(cflags_flag_t));
        new_flag = flags->first_flag;
    }

    new_flag->short_name = '\0';
    new_flag->long_name = NULL;
    new_flag->type = CFLAGS_TYPE_UNDEFINED;
    new_flag->count = 0;
    new_flag->description = NULL;
    new_flag->next = NULL;

    return new_flag;
}

static cflags_flag_t * cflags_add_string(cflags_t * flags, char short_name, const char * long_name, const char ** value, const char * description)
{
    cflags_flag_t * flag = _cflags_add_flag(flags);
    flag->short_name = short_name;
    flag->long_name = long_name;
    flag->type = CFLAGS_TYPE_STRING;
    flag->string_value = value;
    flag->description = description;
    return flag;
}

static cflags_flag_t * cflags_add_bool(cflags_t * flags, char short_name, const char * long_name, bool * value, const char * description)
{
    cflags_flag_t * flag = _cflags_add_flag(flags);
    flag->short_name = short_name;
    flag->long_name = long_name;
    flag->type = CFLAGS_TYPE_BOOL;
    flag->bool_value = value;
    flag->description = description;
    return flag;
}

static cflags_flag_t * cflags_add_int(cflags_t * flags, char short_name, const char * long_name, int * value, const char * description)
{
    cflags_flag_t * flag = _cflags_add_flag(flags);
    flag->short_name = short_name;
    flag->long_name = long_name;
    flag->type = CFLAGS_TYPE_INT;
    flag->int_value = value;
    flag->description = description;
    return flag;
}

static cflags_flag_t * cflags_add_float(cflags_t * flags, char short_name, const char * long_name, float * value, const char * description)
{
    cflags_flag_t * flag = _cflags_add_flag(flags);
    flag->short_name = short_name;
    flag->long_name = long_name;
    flag->type = CFLAGS_TYPE_FLOAT;
    flag->float_value = value;
    flag->description = description;
    return flag;
}

static cflags_flag_t * cflags_add_string_callback(cflags_t * flags, char short_name, const char * long_name, void (*func)(const char *), const char * description)
{
    cflags_flag_t * flag = _cflags_add_flag(flags);
    flag->short_name = short_name;
    flag->long_name = long_name;
    flag->type = CFLAGS_TYPE_STRING_CALLBACK;
    flag->string_callback = func;
    flag->description = description;
    return flag;
}

static cflags_flag_t * cflags_add_bool_callback(cflags_t * flags, char short_name, const char * long_name, void (*func)(bool), const char * description)
{
    cflags_flag_t * flag = _cflags_add_flag(flags);
    flag->short_name = short_name;
    flag->long_name = long_name;
    flag->type = CFLAGS_TYPE_BOOL_CALLBACK;
    flag->bool_callback = func;
    flag->description = description;
    return flag;
}

static cflags_flag_t * cflags_add_int_callback(cflags_t * flags, char short_name, const char * long_name, void (*func)(int), const char * description)
{
    cflags_flag_t * flag = _cflags_add_flag(flags);
    flag->short_name = short_name;
    flag->long_name = long_name;
    flag->type = CFLAGS_TYPE_INT_CALLBACK;
    flag->int_callback = func;
    flag->description = description;
    return flag;
}

static cflags_flag_t * cflags_add_float_callback(cflags_t * flags, char short_name, const char * long_name, void (*func)(float), const char * description)
{
    cflags_flag_t * flag = _cflags_add_flag(flags);
    flag->short_name = short_name;
    flag->long_name = long_name;
    flag->type = CFLAGS_TYPE_FLOAT_CALLBACK;
    flag->float_callback = func;
    flag->description = description;
    return flag;
}

static bool _cflags_parse_bool(const char * str)
{
    return !(strcmp(str, "false") == 0 || 
            strcmp(str, "FALSE") == 0 ||
            strcmp(str, "0") == 0);
}

static void _cflags_process_flag(cflags_flag_t * flag, const char * value) 
{
    ++flag->count;

    switch (flag->type) {
    case CFLAGS_TYPE_STRING:
        if (flag->string_value) {
            if (value) {
                *flag->string_value = value;
            }
        }
        break;
    case CFLAGS_TYPE_STRING_CALLBACK:
        if (flag->string_callback) {
            flag->string_callback(value);
        }
        break;
    case CFLAGS_TYPE_BOOL:
        if (flag->bool_value) {
            if (value) {
                *flag->bool_value = _cflags_parse_bool(value);
            }
            else {
                *flag->bool_value = true;
            }
        }
        break;
    case CFLAGS_TYPE_BOOL_CALLBACK:
        if (flag->bool_callback) {
            if (value) {
                flag->bool_callback(_cflags_parse_bool(value));
            }
            else {
                flag->bool_callback(true);
            }
        }
        break;
    case CFLAGS_TYPE_INT:
        if (flag->int_value) {
            if (value) {
                sscanf(value, "%d", flag->int_value);
            }
        }
        break;
    case CFLAGS_TYPE_INT_CALLBACK:
        if (flag->int_callback) {
            if (value) {
                int tmp = 0;
                sscanf(value, "%d", &tmp);
                flag->int_callback(tmp);
            }
        }
        break;
    case CFLAGS_TYPE_FLOAT:
        if (flag->float_value) {
            if (value) {
                sscanf(value, "%f", flag->float_value);
            }
        }
        break;
    case CFLAGS_TYPE_FLOAT_CALLBACK:
        if (flag->float_callback) {
            if (value) {
                float tmp = 0.f;
                sscanf(value, "%f", &tmp);
                flag->float_callback(tmp);
            }
        }
        break;
    default: ;
    }
}

static void cflags_parse(cflags_t * flags, int argc, char ** argv)
{
    flags->program = argv[0];

    for (int i = 1; i < argc; ++i) {
        char * pch = argv[i];
        if (*pch == '-') {
            ++pch;
            if (*pch == '-') {
                ++pch;
                
                // Long
                char * key = pch;
                char * value = NULL;

                char * divider = strchr(pch, '=');
                if (divider) {
                    *divider = '\0';
                    value = divider + 1;    
                }

                cflags_flag_t * flag = flags->first_flag;
                while (flag) {
                    if (strcmp(flag->long_name, key) == 0) {
                        if (value) {
                            _cflags_process_flag(flag, value);
                        }
                        else {
                            if (i + 1 >= argc) {
                                _cflags_process_flag(flag, NULL);
                            }
                            else {
                                if (argv[i + 1][0] == '-') {
                                    _cflags_process_flag(flag, NULL);
                                }
                                else {
                                    _cflags_process_flag(flag, argv[i + 1]);
                                    ++i;
                                }
                            }
                        }
                        break;
                    }
                    flag = flag->next;
                }
            }
            else {
                while (*pch) {
                    // Short
                    cflags_flag_t * flag = flags->first_flag;
                    while (flag) {
                        if (flag->short_name == *pch) {
                            if (i + 1 >= argc) {
                                _cflags_process_flag(flag, NULL);
                            }
                            else {
                                if (argv[i + 1][0] == '-') {
                                    _cflags_process_flag(flag, NULL);
                                }
                                else {
                                    _cflags_process_flag(flag, argv[i + 1]);
                                    ++i;
                                }
                            }
                            break;
                        }
                        flag = flag->next;
                    }
                    ++pch;
                }
            }
        }
        else {
            ++flags->argc;
            flags->argv = realloc(flags->argv, flags->argc * sizeof(char *));
            flags->argv[flags->argc - 1] = pch;
        }
    }
}

static void cflags_free(cflags_t * flags)
{
    free(flags->argv);

    cflags_flag_t * tmp = NULL;
    cflags_flag_t * flag = flags->first_flag;
    while (flag) {
        tmp = flag;
        flag = flag->next;
        free(tmp);
    }

    free(flags);
}

static void cflags_print_usage(cflags_t * flags, const char * args, const char * above, const char * below)
{
    printf("%s %s\n", flags->program, args);
    printf("%s\n\n", above);
    
    cflags_flag_t * flag = flags->first_flag;
    while (flag) {
        // print flag
        printf("  ");
        if (flag->short_name != '\0') {
            printf("-%c, ", flag->short_name);
        }
        else {
            printf("    ");
        }
        size_t long_name_len = 0;
        if (flag->long_name) {
            printf("--%s", flag->long_name);
            long_name_len = strlen(flag->long_name);
        }
        if (long_name_len > 20) {
            printf("\n                            ");
        }
        else {
            for (size_t i = 0; i < 20 - long_name_len; ++i) {
                printf(" ");
            }
        }
        printf("%s\n", flag->description);
        flag = flag->next;
    }

    printf("\n%s\n", below);
}

#endif // CFLAGS_H