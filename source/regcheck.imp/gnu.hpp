#ifndef REGCHECK_GNU
#define REGCHECK_GNU

#include "Regcheck.hpp"

#include <regex.h>

class GNUcheck : public Regcheck {
    bool was_set = false;
    regex_t re;
    static const int compile_error_message_len = 256;
    char compile_error_message[compile_error_message_len];

  public:
    GNUcheck() { ; }
    ~GNUcheck() {
        if (was_set) {
            regfree(&re);
        }
    }

    int compile(const char * s) {
        int r;

        if (was_set) {
            regfree(&re);
        }

        r = regcomp(&re, s, REG_EXTENDED);

        if (r) {
            regerror(
                r,
                &re,
                compile_error_message,
                compile_error_message_len
            );
        } else {
            compile_error_message[0] = '\0';
        }

        was_set = true;

        return r;
    }

    const char * get_compile_error(void) override {
        return compile_error_message;
    }

    vector<int> eval(vector<string> ss) override {
        vector<int> r;
        for (const auto &s : ss) {
            int rc = regexec(&re, s.c_str(), 0, NULL, 0);
            r.push_back(rc == 0);
        }
        return r;
    }
};

REGISTER_CHECKER(GNU);

#endif
