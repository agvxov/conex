#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <string.h>

#include <dictate.hpp>

extern "C" {
    #include "tcl_tk.h"
}

#define REGISTER_CHECKER(R)\
    struct _ ## R ## registerer {           \
        _ ## R ## registerer () {           \
            regcheck_engines.push_back(#R); \
        }                                   \
    } _ ## R ## registerer_i;

#define DEFAULT_CHECKER regcheck_engines[0].c_str()

using namespace std;

static vector<string> regcheck_engines;

#include "Regcheck.hpp"
#include "regcheck.imp/pcre2.hpp"
#include "regcheck.imp/gnu.hpp"
#include "regcheck.imp/perl.hpp"

Regcheck * regcheck_factory(const char * s) {
    if (!strcmp(s, "GNU")) {
        return new GNUcheck();
    } else
    if (!strcmp(s, "pcre2")) {
        return new PCRE2check();
    } else
    if (!strcmp(s, "Perl")) {
        return new Perlcheck();
    } else {
        abort();
    }
}

static Regcheck * regcheck;

static vector<string> inputs;
static vector<int> results;
static string last_pattern;

extern "C"
void put_regex(const char * const s) {
    last_pattern = string(s, strlen(s)-1);
    regcheck->compile(last_pattern.c_str());

    results = regcheck->eval(inputs);
}

extern "C"
void put_input(const char * const s) {
    inputs.clear();

    istringstream stream(s);
    string line;
    while (getline(stream, line)) {
        inputs.push_back(line);
    }

    results = regcheck->eval(inputs);
}

extern "C"
void put_engine(const char * const s) {
    delete regcheck;
    regcheck = regcheck_factory(s);
    regcheck->compile(last_pattern.c_str());
    regcheck->eval(inputs);
}

extern "C"
char * pull_results(void) {
    string r = "";
    for (const auto &i : results) {
        r += to_string(i);
        r += " ";
    }

    return strdup(r.c_str());
}

extern "C"
char * pull_engines(void) {
    string r = "";
    for (const auto &i : regcheck_engines) {
        r += i;
        r += " ";
    }
    r = r.substr(0, r.size()-1);

    return strdup(r.c_str());
}

extern "C"
char * pull_compile_error(void) {
    return strdup(regcheck->get_compile_error());
}

signed main(void) {
    regcheck = regcheck_factory(DEFAULT_CHECKER);

    tcl_loop();

    while (true) { ; }
}
