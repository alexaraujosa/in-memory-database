#ifndef TP_UTIL_STRING_H
#define TP_UTIL_STRING_H

#include "common.h"
#include <string.h>
#include <stdarg.h>

#define xstr(s) str(s)
#define str(s) #s

char* isnprintf(const char *format, ...);
char* strdup_to(char* dest, char* src);

#endif