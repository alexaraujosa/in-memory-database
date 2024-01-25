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

#define ParserStore GArray*

#define Tokenizer(name) Tokens(*name)(char* line, ssize_t len)
#define PreprocessFunction(name) void(*name)(FILE*, ParserStore, va_list)
#define VerifyFunction(name) int(*name)(Tokens, ParserStore)
#define ParseFunction(name) void*(*name)(Tokens)
#define WriteFunction(name) void(*name)(void*, ParserStore)
#define DestructFunction(name) void(*name)(FILE*, ParserStore)

/**
 * @brief Tokenizes a string given a one-character delimiter.
 * 
 * @param line A string.
 * @param len The length of the line.
 * @param delim The delimiter to split the line.
 * 
 * @return The tokens extracted from the line.
 */
Tokens tokenize_char_delim(char* line, ssize_t len, char delim[1]);

/**
 * @brief Tokenizes a CSV line.
 * 
 * @param line A CSV line.
 * @param len The length of the line.
 * 
 * @return The tokens extracted from the line.
 */
Tokens tokenize_csv(char* line, ssize_t len);

/**
 * @brief Destroys a Tokens structure.
 * 
 * @param tokens The tokens to be destroyed.
 */
void destroy_tokens(Tokens tokens);

/**
 * @brief Generates an empty store for a parser.
 * 
 * @return An empty store for a parser.
 */
ParserStore makeStore();

/**
 * @brief Discard helper that discards tokens to a file stream.
 * 
 * @param tokens The tokens to be discarded.
 * @param store The store for the current parser.
 */
void discard_to_file(Tokens tokens, ParserStore store);

// /**
//  * @brief Fetches the header of a CSV file and stores it.
//  * 
//  * @param stream The file stream
//  */
// void cvs_preprocessor_helper(FILE* stream, ParserStore store);

/**
 * @brief Default preprocessor strategy for a parser for CSV files.  
 * Initializes the store with the discard value as it's first value.
 * 
 * @param stream A CSV file stream.
 * @param store The store for the current parser.
 * @param args Extra arguments passed to the @see ::parse_file.
 * 
 * @return void
 */
void default_csv_preprocessor(FILE* stream, ParserStore store, va_list args);

/**
 * @brief Default destructor strategy for a parser for CSV files.
 * Frees the parser store as initialized by the default_csv_preprocessor.
 * 
 * @param stream FUCKING USELESS
 * @param store The store for the current parser.
 * 
 * @return void
 */
void default_csv_destructor(FILE* stream, ParserStore store); // TODO: Remove stream

/**
 * @brief Parses a string.
 * 
 * @param input A data string to be parsed.
 * @param input_len The length of the input.
 * @param tokenizer The tokenizer stragegy to be used.
 * @param verifier The verifier stragegy to be used.
 * @param parser The parser stragegy to be used.
 * @param writer The writer stragegy to be used.
 * @param discarder The discarder stragegy to be used.
 * @param store The store for the current parser.
 * 
 * @return void
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

/**
 * @brief Parses a file line by line, passing control to each module in sequence.
 * 
 * @param filename The path (absolute or relative) to the input file.
 * @param tokenizer The tokenizer stragegy to be used.
 * @param preprocess The preprocessor stragegy to be used.
 * @param verifier The verifier stragegy to be used.
 * @param parser The parser tragegy to be used.
 * @param writer The writer stragegy to be used.
 * @param discarder The discarder stragegy to be used.
 * @param destructor The destructor stragegy to be used.
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