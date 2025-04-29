#ifndef REGCHECK_PERL
#define REGCHECK_PERL

#include "Regcheck.hpp"

#include <qx.h>

class Perlcheck : public Regcheck {
    string pattern;
    static const int compile_error_message_len = 256;
    char compile_error_message[compile_error_message_len];

  public:
    Perlcheck() { ; }
    ~Perlcheck() { ; }

    int compile(const char * s) {
        static char cmd_tmpl[] =
            "perl -E '/%s/' 2>&1"
        ;
        char * cmd;
        int r = 0;

        asprintf(&cmd, cmd_tmpl, s);

        char * output = qx(cmd);

        if (output[0] != '\0') {
            r = 1;
            strncpy(
                compile_error_message,
                output,
                compile_error_message_len
            );
        } else {
            compile_error_message[0] = '\0';
        }

        free(output);
        free(cmd);

        pattern = s;

        return r;
    }

    const char * get_compile_error(void) override {
        return compile_error_message;
    }

    vector<int> eval(vector<string> ss) override {
        static char cmd_tmpl[] =
            "perl -E 'say for map { /%s/ ? 1 : 0 } (%s)' 2>&1"
        ;
        char * cmd;
        vector<int> r;

        if (compile_error_message[0] != '\0') {
            return r;
        }
        
        string lines = "";
        for (const auto &s : ss) {
            lines += "\"";
            lines += s;
            lines += "\", ";
        }

        asprintf(
            &cmd,
            cmd_tmpl,
            pattern.c_str(),
            lines.c_str()
        );

        char * output = qx(cmd);

        for (char * s = output; *s != '\0'; s++) {
            switch (*s) {
                case '0': {
                    r.push_back(false);
                } break;
                case '1': {
                    r.push_back(true);
                } break;
                case '\n': { ; } break;
                default: { abort(); }
            }
        }

        free(output);
        free(cmd);

        return r;
    }
};

REGISTER_CHECKER(Perl);

#endif
