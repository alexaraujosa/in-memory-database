#include "util/error.h"

void rt_assert(int condition, const char *message) {
    if (!condition) {
        fprintf(stderr, "Assertion failed: %s\n", message);
        exit(EXIT_FAILURE);
    }
}