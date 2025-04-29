#ifndef REGCHECK
#define REGCHECK

/* For every regex dialect our builder can check,
 *  this interface must be implemented.
 */
class Regcheck {
  public:
    virtual ~Regcheck() = 0;
    virtual vector<int> eval(vector<string> ss) = 0;

    /* Configures the regex to use.
     *  Subsequent eval() calls shall respect it.
     */
    virtual int compile(const char * s) = 0;

    /* NOTE: this function might be called even
     *        if there was no compile error,
     *        in which case it shall return an empty string
     */
    virtual const char * get_compile_error(void) = 0;
};

Regcheck::~Regcheck() { ; }

#endif
