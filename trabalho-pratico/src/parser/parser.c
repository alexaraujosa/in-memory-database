#include "parser.h"

#define DELIMITER ';'
#define DELIMITER_S LITERALIZE(DELIMITER)

void parse(char* filename, VerifyFunction(verifier), ParseFunction(parser), WriteFunction(writer)) {
    // for each line
    //    tokens = tokenize(line)
    //    verify(tokens)
    //    values = evaluateValues(tokens)
    //    insertValues(values)

    IGNORE_ARG(filename);
    IGNORE_ARG(verifier);
    IGNORE_ARG(parser);
    IGNORE_ARG(writer);
}