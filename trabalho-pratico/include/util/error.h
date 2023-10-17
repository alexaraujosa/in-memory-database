#ifndef TP_UTIL_ERROR_H
#define TP_UTIL_ERROR_H

#include "common.h"

enum Errors {
    MAXIMUM_VALUE_EXCEEDED
};

#ifndef NDEBUG
    #define m_assert(expr, msg) assert(( (void)(msg), (expr) ))
#else
    #define m_Assert(expr, msg) ;
#endif

void rt_assert(int condition, const char *message);

#endif