#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

# ifdef __cplusplus
    class Regcheck;

    extern Regcheck * regcheck;

    Regcheck * regcheck_factory(const char * s);

    extern "C" {
# endif

extern void put_regex(const char * const s);
extern void put_input(const char * const s);
extern void put_engine(const char * const s);
extern char * pull_results(void);
extern char * pull_compile_error(void);
extern char * pull_engines(void);

# ifdef __cplusplus
    }
# endif

#endif
