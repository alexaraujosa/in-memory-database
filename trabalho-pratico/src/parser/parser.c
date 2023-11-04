// The functions defined on this file refuse to follow any kind of law except those of chaos.

#include "parser/parser.h"

#define CSV_DELIMITER ';'
#define CSV_DELIMITER_S LITERALIZE(CSV_DELIMITER) // BUG: This thing is "';'", yet somehow still works, wtf?
// #define CSV_DELIMITER_S MAKE_CHAR_ARRAY(CSV_DELIMITER) // This one makes sense.

Tokens tokenize_csv(char* line, ssize_t len) {
    char* ptr = strndup(line, len + 1);
    if (ptr == NULL) exit(EXIT_FAILURE);

    if (ptr[len - 1] == '\n') {
        ptr[len - 1] = '\0';
    }

    int seps = 1;
    for (int i = 0; line[i]; i++) seps += (line[i] == CSV_DELIMITER);

    char** arr = (char**)malloc(seps * sizeof(char*));

    volatile char* delim = CSV_DELIMITER_S;

    char* token;
    int i = 0;
    while ((token = strsep(&ptr, CSV_DELIMITER_S)) != NULL) {
        int tokenLen = strlen(token);
        char* tokenData = strndup(token, tokenLen + 1);

        arr[i++] = tokenData;
    }

    Tokens ret = (Tokens)malloc(sizeof(TOKENS));
    ret->data = arr;
    ret->len = seps;

    free(ptr);
    return ret;
}

void parse(
    char* input,
    ssize_t input_len,
    Tokenizer(tokenizer),
    VerifyFunction(verifier), 
    ParseFunction(parser), 
    WriteFunction(writer), 
    WriteFunction(discarder)
) {
    rt_assert(
        input != NULL, 
        "Expected input string, but got NULL."
    );
    rt_assert(
        input_len >= 0, 
        "Expected positive string length, but got negative number."
    );
    rt_assert(
        tokenizer != NULL, 
        "Expected tokenizer function, but got NULL."
    );
    rt_assert(
        verifier != NULL, 
        "Expected verifier function, but got NULL."
    );
    rt_assert(
        parser != NULL, 
        "Expected parser function, but got NULL."
    );
    rt_assert(
        writer != NULL, 
        "Expected writer function, but got NULL."
    );
    rt_assert(
        discarder != NULL, 
        "Expected discarder function, but got NULL."
    );

    volatile Tokens tokens = tokenizer(input, input_len);

    int valid = verifier(tokens);
    if (!valid) {
        printf("INVALID LINE.\n");
        discarder(tokens);
        return;
    }

    void* data = parser(tokens);
    if (data == NULL) {
        printf("Error while parsing: Unable to transform tokens.\n");
        return;
    }

    writer(data);
}

void parse_file(
    char* filename,
    Tokenizer(tokenizer),
    VerifyFunction(verifier), 
    ParseFunction(parser), 
    WriteFunction(writer), 
    WriteFunction(discarder)
) {
    rt_assert(
        tokenizer != NULL, 
        "Expected tokenizer function, but got NULL."
    );
    rt_assert(
        verifier != NULL, 
        "Expected verifier function, but got NULL."
    );
    rt_assert(
        parser != NULL, 
        "Expected parser function, but got NULL."
    );
    rt_assert(
        writer != NULL, 
        "Expected writer function, but got NULL."
    );
    rt_assert(
        discarder != NULL, 
        "Expected discarder function, but got NULL."
    );

    char* paths[2] = { get_cwd()->str, filename };
    char* path = join_paths(paths, 2);

    FILE* stream = OPEN_FILE(path, "r");

    // TODO: Move this to tokenizer or allow caller to control preprocessing of file.
    if (skip_n_lines(stream, 1) == 1) {
        exit(EXIT_FAILURE);
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    
    while ((read = getline(&line, &len, stream)) != -1) {
        parse(line, read, tokenizer, verifier, parser, writer, discarder);
    }

    CLOSE_FILE(stream);
}
