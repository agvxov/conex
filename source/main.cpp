#include <dictate.hpp>

#include "middleware.h"

extern "C" {
    #include "tcl_tk.h"
}

signed main(void) {
    regcheck = regcheck_factory("");

    tcl_loop();

    while (true) { ; }
}
