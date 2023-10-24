#ifndef TP_PARSER_H
#define TP_PARSER_H

#include "common.h"
#include "util/io.h"
#include "util/misc.h"

typedef struct tokens {
    char** data;
    int len;
} TOKENS, *Tokens;

#define VerifyFunction(name) int(*name)(Tokens)
#define ParseFunction(name) void*(*name)(Tokens)
#define WriteFunction(name) void(*name)(void*)

void parse(
    char* filename,
    VerifyFunction(verifier), 
    ParseFunction(parser), 
    WriteFunction(writer), 
    WriteFunction(discarder)
);

#endif