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

// typedef struct parser_store {
//     // FILE* discard_file;
//     // char* file_header;
//     GArray* data;
// } PARSER_STORE, *ParserStore;

#define ParserStore GArray*

#define Tokenizer(name) Tokens(*name)(char* line, ssize_t len)
#define PreprocessFunction(name) void(*name)(FILE*, ParserStore, va_list)
#define VerifyFunction(name) int(*name)(Tokens, ParserStore)
#define ParseFunction(name) void*(*name)(Tokens)
#define WriteFunction(name) void(*name)(void*, ParserStore)
#define DestructFunction(name) void(*name)(FILE*, ParserStore)

/*
 * Tokenizes a CSV line.
 */
Tokens tokenize_csv(char* line, ssize_t len);

void destroy_tokens(Tokens tokens);

/*
 * Generates a store for a parser.
 */
ParserStore makeStore();

/*
 * Discard helper that discards tokens to a file stream.
 */
void discard_to_file(Tokens tokens, ParserStore store);

/*
 * Fetches the header of a CSV file and stores it.
 */
void cvs_preprocessor_helper(FILE* stream, ParserStore store);

/*
 * Default preprocessor strategy for a parser for CSV files.
 * 
 * Initializes the store with the discard value as it's first value.
 */
void default_csv_preprocessor(FILE* stream, ParserStore store, va_list args);

/*
 * Default destructor strategy for a parser for CSV files.
 * 
 * Frees the parser store as initialized by the default_csv_preprocessor.
 */
void default_csv_destructor(FILE* stream, ParserStore store);

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
    WriteFunction(discarder),
    ParserStore store
);

/*
 * Parses a file line by line, passing control to each module in sequence.
 */
void parse_file(
    char* filename,
    Tokenizer(tokenizer),
    PreprocessFunction(preprocess),
    VerifyFunction(verifier), 
    ParseFunction(parser), 
    WriteFunction(writer), 
    WriteFunction(discarder),
    DestructFunction(destructor),
    ...
);

#endif