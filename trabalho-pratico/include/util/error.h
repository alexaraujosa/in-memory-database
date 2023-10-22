#ifndef TP_UTIL_ERROR_H
#define TP_UTIL_ERROR_H

#include "common.h"
#include <assert.h>
#include <errno.h>

// Define only if assert is also defined.
#ifndef NDEBUG
    #define m_assert(expr, msg) assert(( (void)(msg), (expr) ))
#else
    #define m_Assert(expr, msg) ;
#endif

void rt_assert(int condition, const char *message);

#endif