#ifndef REGCHECK_PCRE2
#define REGCHECK_PCRE2

#include "Regcheck.hpp"

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

class PCRE2check : public Regcheck {
    pcre2_code * re = NULL;
    static const int compile_error_message_len = 256;
    PCRE2_UCHAR8 compile_error_message[compile_error_message_len];

  public:
    PCRE2check() { ; }
    ~PCRE2check() override {
        pcre2_code_free(re);
    }

    int compile(const char * s) {
        free(re);

        int errornumber;
        PCRE2_SIZE erroroffset;

        re = pcre2_compile(
            (PCRE2_SPTR)s,
            PCRE2_ZERO_TERMINATED,
            0,
            &errornumber,
            &erroroffset,
            NULL
        );

        if (!re) {
            pcre2_get_error_message(
                errornumber,
                compile_error_message,
                compile_error_message_len
            );
        } else {
            compile_error_message[0] = '\0';
        }

        return 0;
    }

    const char * get_compile_error(void) override {
        return (const char *)compile_error_message;
    }

    vector<int> eval(vector<string> ss) override {
        if (not re) { return vector<int>(); }

        vector<int> r;

        pcre2_match_data * match_data = pcre2_match_data_create_from_pattern(re, NULL);

        for (const auto &s : ss) {
            int rc = pcre2_match(
                this->re,
                (PCRE2_SPTR)s.c_str(),
                s.size(),
                0,
                0,
                match_data,
                NULL
            );

            r.push_back(rc >= 0);
        }
        return r;
    }
};

REGISTER_CHECKER(pcre2);

#endif
