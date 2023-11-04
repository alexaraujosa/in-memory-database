#ifndef TP_PARSER_H
#define TP_PARSER_H

#include "common.h"
#include "util/io.h"
#include "util/misc.h"
#include "util/error.h"

typedef struct tokens {
    char** data;
    int len;
} TOKENS, *Tokens;

#define Tokenizer(name) Tokens(*name)(char* line, ssize_t len)
#define VerifyFunction(name) int(*name)(Tokens)
#define ParseFunction(name) void*(*name)(Tokens)
#define WriteFunction(name) void(*name)(void*)

/*
 * Tokenizes a CSV line.
 */
Tokens tokenize_csv(char* line, ssize_t len);

/*
 * Parses a string.
 */
void parse(
    char* input,
    ssize_t input_len,
    Tokenizer(tokenizer),
    VerifyFunction(verifier), 
    ParseFunction(parser), 
    WriteFunction(writer), 
    WriteFunction(discarder)
);

/*
 * Parses a file line by line, passing control to each module in sequence.
 */
void parse_file(
    char* filename,
    Tokenizer(tokenizer),
    VerifyFunction(verifier), 
    ParseFunction(parser), 
    WriteFunction(writer), 
    WriteFunction(discarder)
);

#endif