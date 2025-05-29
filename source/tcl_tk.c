#include <stdlib.h>
#include <pthread.h>
#include <tcl.h>
#include <tk.h>
#include "middleware.h"

#define TCL_ARGS \
    [[maybe_unused]] ClientData clientData, \
    [[maybe_unused]] Tcl_Interp *interp, \
    [[maybe_unused]] int argc, \
    [[maybe_unused]] const char **argv \

#define TCL_EASY_CREATE_COMMAND(c) \
    Tcl_CreateCommand(interp, #c, Tcl_ ## c, (ClientData)NULL, (void (*)(void*))NULL);

static
int Tcl_cPutRegex(TCL_ARGS) {
    put_regex(argv[1]);
    return TCL_OK;
}

static
int Tcl_cPutInput(TCL_ARGS) {
    put_input(argv[1]);
    return TCL_OK;
}

static
int Tcl_cPutEngine(TCL_ARGS) {
    put_engine(argv[1]);
    return TCL_OK;
}

static
int Tcl_cGetCorrectness(TCL_ARGS) {
    char * r = pull_results();
    Tcl_SetResult(interp, r, TCL_VOLATILE);
    free(r);

    return TCL_OK;
}

static
int Tcl_cGetEngines(TCL_ARGS) {
    char * r = pull_engines();
    Tcl_SetResult(interp, r, TCL_VOLATILE);
    free(r);

    return TCL_OK;
}

static
int Tcl_cGetCompileError(TCL_ARGS) {
    char * e = pull_compile_error();
    Tcl_SetResult(interp, e, TCL_VOLATILE);
    free(e);

    return TCL_OK;
}

static
void tcl_run(void) {
    Tcl_Interp * interp = Tcl_CreateInterp();
    if (interp == NULL) {
        fprintf(stderr, "Can't create Tcl interpreter\n");
        exit(1);
    }

    Tcl_Init(interp);
    Tk_Init(interp);

    // Disable our testing trick
    Tcl_SetVar(interp, "WRAPPED", "true", 0); 

    TCL_EASY_CREATE_COMMAND(cPutRegex);
    TCL_EASY_CREATE_COMMAND(cPutInput);
    TCL_EASY_CREATE_COMMAND(cPutEngine);
    TCL_EASY_CREATE_COMMAND(cGetCorrectness);
    TCL_EASY_CREATE_COMMAND(cGetEngines);
    TCL_EASY_CREATE_COMMAND(cGetCompileError);

    int result;
  #if DEBUG
    result = Tcl_EvalFile(interp, "source/gui.tcl");
  #else
    const char script[] = {
        #embed "gui.tcl"
    };
    result = Tcl_Eval(interp, script);
  #endif

    if (result == TCL_ERROR) {
        fprintf(stderr, "Tcl script execution failed: %s\n", Tcl_GetStringResult(interp));
        exit(1);
    }

    Tk_MainLoop();
}

void tcl_loop(void) {
    pthread_t tcl_thread;
    pthread_create(&tcl_thread, NULL, (void* (*)(void*))tcl_run, (void*)NULL);
}
