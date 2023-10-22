#ifndef TP_UTIL_MISC_H
#define TP_UTIL_MISC_H

#define MAX_BIT_VALUE(bits) ((1ULL << (bits)) - 1)

#define LITERALIZE_HELPER(token) #token
#define LITERALIZE(token) LITERALIZE_HELPER(token)

#endif