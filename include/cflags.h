//
// cflags version 3.0.3
//
// MIT License
// 
// Copyright (c) 2022 Stephen Lane-Walsh
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef CFLAGS_H
#define CFLAGS_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define CFLAGS_ERROR_OOM "cflags: out of memory"

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
        const char **   string_ptr;
        bool *          bool_ptr;
        int *           int_ptr;
        float *         float_ptr;
    };
    
    void (*string_callback)(const char *);
    void (*bool_callback)(bool);
    void (*int_callback)(int);
    void (*float_callback)(float);
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
    cflags_t * flags = (cflags_t *)malloc(sizeof(cflags_t));
    if (!flags) {
        fprintf(stderr, CFLAGS_ERROR_OOM);
        return NULL;
    }

    flags->program = NULL;
    flags->argc = 0;
    flags->argv = NULL;
    flags->first_flag = NULL;
    return flags;
}

cflags_flag_t * _cflags_add_flag(cflags_t * flags)
{
    cflags_flag_t ** next_flag = &flags->first_flag;

    while (*next_flag) {
        next_flag = &(*next_flag)->next;
    }

    *next_flag = (cflags_flag_t *)malloc(sizeof(cflags_flag_t));
    if (!*next_flag) {
        fprintf(stderr, CFLAGS_ERROR_OOM);
        return NULL;
    }

    (*next_flag)->short_name = '\0';
    (*next_flag)->long_name = NULL;
    (*next_flag)->type = CFLAGS_TYPE_UNDEFINED;
    (*next_flag)->count = 0;
    (*next_flag)->description = NULL;
    (*next_flag)->next = NULL;

    return *next_flag;
}

static cflags_flag_t * cflags_add_string(cflags_t * flags, char short_name, const char * long_name, const char ** value, const char * description)
{
    cflags_flag_t * flag = _cflags_add_flag(flags);
    if (!flag) {
        return NULL;
    }

    flag->short_name = short_name;
    flag->long_name = long_name;
    flag->type = CFLAGS_TYPE_STRING;
    flag->string_ptr = value;
    flag->description = description;
    return flag;
}

static cflags_flag_t * cflags_add_bool(cflags_t * flags, char short_name, const char * long_name, bool * value, const char * description)
{
    cflags_flag_t * flag = _cflags_add_flag(flags);
    if (!flag) {
        return NULL;
    }

    flag->short_name = short_name;
    flag->long_name = long_name;
    flag->type = CFLAGS_TYPE_BOOL;
    flag->bool_ptr = value;
    flag->description = description;
    return flag;
}

static cflags_flag_t * cflags_add_int(cflags_t * flags, char short_name, const char * long_name, int * value, const char * description)
{
    cflags_flag_t * flag = _cflags_add_flag(flags);
    if (!flag) {
        return NULL;
    }

    flag->short_name = short_name;
    flag->long_name = long_name;
    flag->type = CFLAGS_TYPE_INT;
    flag->int_ptr = value;
    flag->description = description;
    return flag;
}

static cflags_flag_t * cflags_add_float(cflags_t * flags, char short_name, const char * long_name, float * value, const char * description)
{
    cflags_flag_t * flag = _cflags_add_flag(flags);
    if (!flag) {
        return NULL;
    }

    flag->short_name = short_name;
    flag->long_name = long_name;
    flag->type = CFLAGS_TYPE_FLOAT;
    flag->float_ptr = value;
    flag->description = description;
    return flag;
}

static cflags_flag_t * cflags_add_string_callback(cflags_t * flags, char short_name, const char * long_name, void (*func)(const char *), const char * description)
{
    cflags_flag_t * flag = _cflags_add_flag(flags);
    if (!flag) {
        return NULL;
    }

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
    if (!flag) {
        return NULL;
    }

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
    if (!flag) {
        return NULL;
    }

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
    if (!flag) {
        return NULL;
    }

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
        if (flag->string_ptr) {
            *flag->string_ptr = value;
        }
        break;
    case CFLAGS_TYPE_STRING_CALLBACK:
        if (flag->string_callback) {
            flag->string_callback(value);
        }
        break;
    case CFLAGS_TYPE_BOOL:
        if (flag->bool_ptr) {
            if (value) {
                *flag->bool_ptr = _cflags_parse_bool(value);
            }
            else {
                *flag->bool_ptr = true;
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
        if (flag->int_ptr) {
            if (value) {
                *flag->int_ptr = strtol(value, NULL, 10);
            }
        }
        break;
    case CFLAGS_TYPE_INT_CALLBACK:
        if (flag->int_callback) {
            if (value) {
                flag->int_callback(strtol(value, NULL, 10));
            }
        }
        break;
    case CFLAGS_TYPE_FLOAT:
        if (flag->float_ptr) {
            if (value) {
                *flag->float_ptr = strtof(value, NULL);
            }
        }
        break;
    case CFLAGS_TYPE_FLOAT_CALLBACK:
        if (flag->float_callback) {
            if (value) {
                flag->float_callback(strtof(value, NULL));
            }
        }
        break;
    default: ;
    }
}

static bool cflags_parse(cflags_t * flags, int argc, char ** argv)
{
    flags->argc = 1;
    flags->argv = (char **)malloc(flags->argc * sizeof(char *));
    if (!flags->argv) {
        fprintf(stderr, CFLAGS_ERROR_OOM);
        return false;
    }
    flags->argv[0] = argv[0];
    flags->program = flags->argv[0];

    bool passthrough = false;
    for (int i = 1; i < argc; ++i) {
        char * pch = argv[i];
        if (!passthrough && *pch == '-') {
            ++pch;
            if (*pch == '-') {
                ++pch;

                if (*pch == '\0') {
                    // All following flags are not to be processed
                    passthrough = true;
                    continue;
                }
                
                // Long
                char * key = pch;
                char * value = NULL;

                char * divider = strchr(pch, '=');
                if (divider) {
                    *divider = '\0';
                    value = divider + 1;
                }

                bool next_arg_is_value = (i + 1 < argc && argv[i + 1][0] != '-');

                bool found = false;
                cflags_flag_t * flag = flags->first_flag;
                while (flag) {
                    if (strcmp(flag->long_name, key) == 0) {
                        found = true;

                        if (value) {
                            _cflags_process_flag(flag, value);
                        }
                        else if (next_arg_is_value) {
                            _cflags_process_flag(flag, argv[i + 1]);
                            ++i;
                        }
                        else if (flag->type == CFLAGS_TYPE_BOOL || flag->type == CFLAGS_TYPE_BOOL_CALLBACK) {
                            _cflags_process_flag(flag, NULL);
                        }
                        else {
                            fprintf(stderr, "%s: option '--%s' requires an value\n", flags->program, key);
                            return false;
                        }

                        break;
                    }

                    flag = flag->next;
                }

                if (!found) {
                    fprintf(stderr, "%s: unrecognized option '--%s'\n", flags->program, key);
                    return false;
                }
            }
            else {
                // Short
                while (*pch) {
                    bool is_last_short_flag = (*(pch + 1) == '\0');
                    bool next_arg_is_value = (i + 1 < argc && argv[i + 1][0] != '-');
                    
                    bool found = false;
                    cflags_flag_t * flag = flags->first_flag;
                    while (flag) {
                        if (flag->short_name == *pch) {
                            found = true;

                            if (is_last_short_flag && next_arg_is_value) {
                                _cflags_process_flag(flag, argv[i + 1]);
                                ++i;
                            }
                            else if (flag->type == CFLAGS_TYPE_BOOL || flag->type == CFLAGS_TYPE_BOOL_CALLBACK) {
                                _cflags_process_flag(flag, NULL);
                            }
                            else {
                                fprintf(stderr, "%s: option '-%c' requires an value\n", flags->program, *pch);
                                return false;
                            }

                            break;
                        }

                        flag = flag->next;
                    }

                    if (!found) {
                        fprintf(stderr, "%s: unrecognized option '-%c'\n", flags->program, *pch);
                        return false;
                    }
                    
                    ++pch;
                }
            }
        }
        else {
            ++flags->argc;
            char ** tmp = (char **)realloc(flags->argv, flags->argc * sizeof(char *));
            if (!tmp) {
                fprintf(stderr, CFLAGS_ERROR_OOM);
                return false;
            }
            flags->argv = tmp;
            flags->argv[flags->argc - 1] = pch;
        }
    }

    return true;
}

static void cflags_free(cflags_t * flags)
{
    free(flags->argv);
    flags->argv = NULL;

    cflags_flag_t * tmp = NULL;
    cflags_flag_t * flag = flags->first_flag;
    while (flag) {
        tmp = flag;
        flag = flag->next;
        free(tmp);
    }

    free(flags);
    flags = NULL;
}

static void cflags_print_usage(cflags_t * flags, const char * usage, const char * above, const char * below)
{
    printf("%s %s\n", flags->program, usage);
    printf("%s\n\n", above);

    cflags_flag_t * flag = flags->first_flag;
    while (flag) {
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

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // CFLAGS_H