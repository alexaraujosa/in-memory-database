#include "util/string.h"

char* isnprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    // Determine the length of the formatted string
    int length = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (length < 0) return NULL;

    va_start(args, format);

    char *result = (char *)malloc(length + 1);
    if (result == NULL) {
        va_end(args);
        return NULL;
    }

    // Format the string into the allocated memory
    vsnprintf(result, length + 1, format, args);
    va_end(args);

    return result;
}

char* strdup_to(char* dest, char* src) {
    char* str;
    char* p;
    int len = 0;

    while (src[len]) len++;
    str = malloc(len + 1);
    p = str;

    while (*src) *p++ = *src++;
    *p = '\0';

    *dest = *str;

    return str;
}