#ifndef TP_UTIL_MATH_H
#define TP_UTIL_MATH_H

#include <math.h>

inline static int imax(int a, int b) {
    if (a > b) return a;
    else return b;
}

inline static int imin(int a, int b) {
    if (a < b) return a;
    else return b;
}

#endif