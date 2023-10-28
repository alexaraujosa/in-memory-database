#include "parser/parser.h"

#define DELIMITER ';'
#define DELIMITER_S LITERALIZE(DELIMITER)

Tokens tokenize_csv(char* line, ssize_t len) {
    char* ptr = strndup(line, len + 1);
    if (ptr == NULL) exit(EXIT_FAILURE);

    int seps = 1;
    for (int i = 0; line[i]; i++) seps += (line[i] == DELIMITER);

    char** arr = (char**)malloc(seps * sizeof(char*));

    char* token;
    int i = 0;
    while ((token = strsep(&ptr, DELIMITER_S)) != NULL) {
        int tokenLen = strlen(token);
        // TODO: Memory usage reduction: Keep the original string in memory the entire time and pass pointers
        // instead of reduplicating the entire string using various mallocs?
        char* tokenData = strndup(token, tokenLen + 1);
        if (tokenData[tokenLen - 1] == '\n') {
            tokenData[tokenLen - 1] = '\0';
        }

        arr[i++] = tokenData;
    }

    Tokens ret = (Tokens)malloc(sizeof(TOKENS));
    ret->data = arr;
    ret->len = seps;

    free(ptr);
    return ret;
}

void parse(
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

    if (skip_n_lines(stream, 1) == 1) {
        exit(EXIT_FAILURE);
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    
    while ((read = getline(&line, &len, stream)) != -1) {
        // SEE Todo in tokenize_csv
        volatile Tokens tokens = tokenize_csv(line, read);

        int valid = verifier(tokens);
        if (!valid) {
            printf("INVALID LINE.\n");
            discarder(tokens);
            continue;
        }

        void* data = parser(tokens);
        if (data == NULL) {
            printf("Error while parsing: Unable to transform tokens.\n");
            continue;
        }

        writer(data);
    }

    CLOSE_FILE(stream);
}