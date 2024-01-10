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

void rt_assert(int condition, const char *message);
// char* trace_msg(const char* scope, const char* message);

#define trace_msg(scope, message) "ERROR [" scope "] " message

#endif