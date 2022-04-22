//
// cflags version 3.0.1
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

#ifndef CFLAGS_HPP
#define CFLAGS_HPP

#include <cstring>
#include <string>
#include <string_view>
#include <vector>
#include <functional>

namespace cflags {

using std::string;
using std::string_view;
using std::vector;
using std::function;

struct flag
{
public:

    enum class type 
    {
        Undefined = -1,
        String,
        CString,
        Bool,
        Int,
        Float,
        StringCallback,
        CStringCallback,
        BoolCallback,
        IntCallback,
        FloatCallback,
    };

    char        short_name;
    string      long_name;
    string      description;

    type        type;
    unsigned    count;

    union {
        string *        string_ptr;
        const char **   cstring_ptr;
        bool *          bool_ptr;
        int *           int_ptr;
        float *         float_ptr;
    };

    function<void(string)>          string_callback;
    function<void(const char *)>    cstring_callback;
    function<void(bool)>            bool_callback;
    function<void(int)>             int_callback;
    function<void(float)>           float_callback;

    flag()
        : short_name('\0')
        , type(type::Undefined)
        , count(0)
        , string_ptr(nullptr) // This will set all of the *_ptr members
    { }

    inline void process(const char * value)
    {
        auto parse_bool = [](string_view str) {
            return !(
                str == "false" ||
                str == "FALSE" ||
                str == "0"
            );
        };

        ++count;

        switch (type) {
        case type::String:
            if (string_ptr) {
                *string_ptr = value;
            }
            break;
        case type::StringCallback:
            if (string_callback) {
                if (value) {
                    string_callback(value);
                }
            }
            break;
        case type::CString:
            if (cstring_ptr) {
                *cstring_ptr = value;
            }
            break;
        case type::CStringCallback:
            if (cstring_callback) {
                cstring_callback(value);
            }
            break;
        case type::Bool:
            if (bool_ptr) {
                if (value) {
                    *bool_ptr = parse_bool(value);
                }
                else {
                    *bool_ptr = true;
                }
            }
            break;
        case type::BoolCallback:
            if (bool_callback) {
                if (value) {
                    bool_callback(parse_bool(value));
                }
                else {
                    bool_callback(true);
                }
            }
            break;
        case type::Int:
            if (int_ptr) {
                if (value) {
                    *int_ptr = strtol(value, nullptr, 10);
                }
            }
            break;
        case type::IntCallback:
            if (int_callback) {
                if (value) {
                    int_callback(strtol(value, nullptr, 10));
                }
            }
            break;
        case type::Float:
            if (float_ptr) {
                if (value) {
                    *float_ptr = strtof(value, nullptr);
                }
            }
            break;
        case type::FloatCallback:
            if (float_callback) {
                if (value) {
                    float_callback(strtof(value, nullptr));
                }
            }
            break;
        default: ;
        }
    }

};

class cflags
{
public:

    string program;
    vector<string_view> args;

    int argc;
    char ** argv;

    cflags()
        : argc(0)
        , argv(nullptr)
    { }

    flag * add_flag(flag && flag)
    {
        _flags.push_back(flag);
        return &_flags.back();
    }

    flag * add_string(char short_name, string long_name, string * value_ptr, string description)
    {
        flag flag;
        flag.short_name = short_name;
        flag.long_name = long_name;
        flag.type = flag::type::String;
        flag.string_ptr = value_ptr;
        flag.description = description;

        _flags.push_back(flag);
        return &_flags.back();
    }
    
    flag * add_cstring(char short_name, string long_name, const char ** value_ptr, string description)
    {
        flag flag;
        flag.short_name = short_name;
        flag.long_name = long_name;
        flag.type = flag::type::CString;
        flag.cstring_ptr = value_ptr;
        flag.description = description;

        _flags.push_back(flag);
        return &_flags.back();
    }
    
    flag * add_bool(char short_name, string long_name, bool * value_ptr, string description)
    {
        flag flag;
        flag.short_name = short_name;
        flag.long_name = long_name;
        flag.type = flag::type::Bool;
        flag.bool_ptr = value_ptr;
        flag.description = description;

        _flags.push_back(flag);
        return &_flags.back();
    }
    
    flag * add_int(char short_name, string long_name, int * value_ptr, string description)
    {
        flag flag;
        flag.short_name = short_name;
        flag.long_name = long_name;
        flag.type = flag::type::Int;
        flag.int_ptr = value_ptr;
        flag.description = description;

        _flags.push_back(flag);
        return &_flags.back();
    }
    
    flag * add_float(char short_name, string long_name, float * value_ptr, string description)
    {
        flag flag;
        flag.short_name = short_name;
        flag.long_name = long_name;
        flag.type = flag::type::Float;
        flag.float_ptr = value_ptr;
        flag.description = description;

        _flags.push_back(flag);
        return &_flags.back();
    }

    flag * add_string_callback(char short_name, string long_name, function<void(string)> callback, string description)
    {
        flag flag;
        flag.short_name = short_name;
        flag.long_name = long_name;
        flag.type = flag::type::StringCallback;
        flag.string_callback = callback;
        flag.description = description;

        _flags.push_back(flag);
        return &_flags.back();
    }

    flag * add_cstring_callback(char short_name, string long_name, function<void(const char *)> callback, string description)
    {
        flag flag;
        flag.short_name = short_name;
        flag.long_name = long_name;
        flag.type = flag::type::CStringCallback;
        flag.cstring_callback = callback;
        flag.description = description;

        _flags.push_back(flag);
        return &_flags.back();
    }

    flag * add_bool_callback(char short_name, string long_name, function<void(bool)> callback, string description)
    {
        flag flag;
        flag.short_name = short_name;
        flag.long_name = long_name;
        flag.type = flag::type::BoolCallback;
        flag.bool_callback = callback;
        flag.description = description;

        _flags.push_back(flag);
        return &_flags.back();
    }

    flag * add_int_callback(char short_name, string long_name, function<void(int)> callback, string description)
    {
        flag flag;
        flag.short_name = short_name;
        flag.long_name = long_name;
        flag.type = flag::type::IntCallback;
        flag.int_callback = callback;
        flag.description = description;

        _flags.push_back(flag);
        return &_flags.back();
    }

    flag * add_float_callback(char short_name, string long_name, function<void(float)> callback, string description)
    {
        flag flag;
        flag.short_name = short_name;
        flag.long_name = long_name;
        flag.type = flag::type::FloatCallback;
        flag.float_callback = callback;
        flag.description = description;

        _flags.push_back(flag);
        return &_flags.back();
    }

    ///
    ///
    ///
    inline bool parse(int main_argc, char * main_argv[])
    {
        argc = main_argc;
        argv = main_argv;

        program = argv[0];

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
                    for (auto& flag : _flags) {
                        if (flag.long_name == key) {
                            found = true;

                            if (value) {
                                flag.process(value);
                            }
                            else if (next_arg_is_value) {
                                flag.process(argv[i + 1]);
                                ++i;
                            }
                            else if (flag.type == flag::type::Bool || flag.type == flag::type::BoolCallback) {
                                flag.process(nullptr);
                            }
                            else {
                                fprintf(stderr, "%s: option '--%s' requires an value\n", program.c_str(), key);
                                return false;
                            }

                            break;
                        }
                    }

                    if (!found) {
                        fprintf(stderr, "%s: unrecognized option '--%s'\n", program.c_str(), key);
                        return false;
                    }
                }
                else {
                    // Short
                    while (*pch) {
                        bool is_last_short_flag = (*(pch + 1) == '\0');
                        bool next_arg_is_value = (i + 1 < argc && argv[i + 1][0] != '-');

                        bool found = false;
                        for (flag& flag : _flags) {
                            if (flag.short_name == *pch) {
                                found = true;

                                if (is_last_short_flag && next_arg_is_value) {
                                    flag.process(argv[i + 1]);
                                    ++i;
                                }
                                else if (flag.type == flag::type::Bool || flag.type == flag::type::BoolCallback) {
                                    flag.process(nullptr);
                                }
                                else {
                                    fprintf(stderr, "%s: option '-%c' requires an value\n", program.c_str(), *pch);
                                    return false;
                                }

                                break;
                            }
                        }

                        if (!found) {
                            fprintf(stderr, "%s: unrecognized option '-%c'\n", program.c_str(), *pch);
                            return false;
                        }

                        ++pch;
                    }
                }
            }
            else {
                args.push_back(pch);
                _argv.push_back(pch);
            }
        }

        argc = static_cast<int>(_argv.size());
        argv = _argv.data();

        return true;
    }

    void print_usage(const string& usage, const string& above, const string& below)
    {
        printf("Usage: %s %s\n", program.c_str(), usage.c_str());
        printf("%s\n\n", above.c_str());

        for (auto& flag : _flags) {
            printf("  ");
            if (flag.short_name != '\0') {
                printf("-%c, ", flag.short_name);
            }
            else {
                printf("    ");
            }

            if (!flag.long_name.empty()) {
                printf("--%s", flag.long_name.c_str());
            }
            if (flag.long_name.size() > 20) {
                printf("\n                            ");
            }
            else {
                for (size_t i = 0; i < 20 - flag.long_name.size(); ++i) {
                    printf(" ");
                }
            }
            printf("%s\n", flag.description.c_str());
        }

        printf("\n%s\n", below.c_str());
    }

private:

    vector<char *> _argv;
    
    vector<flag> _flags;

};

} // namespace cflags

#endif // CFLAGS_HPP