#ifndef TP_UTIL_ERROR_H
#define TP_UTIL_ERROR_H

#include "common.h"
// #include "util/string.h"
#include <assert.h>
#include <errno.h>

// Define only if assert is also defined.
#ifndef NDEBUG
    #define m_assert(expr, msg) assert(( (void)(msg), (expr) ))
#else
    #define m_assert(expr, msg) ;
#endif

// TODO: Turn into function
#define rt_assert_dyn(cond, msg, dstr) {\
    if (!(cond)) {\
        void* dat = (msg);\
        (dstr(dat));\
        exit(EXIT_FAILURE);\
    }\
}

#define trace_msg(scope, message) "ERROR [" scope "] " message

void rt_assert(int condition, const char *message);

#endif