/* 
 * The functions defined on this file refuse to follow any kind of law except those of chaos.
 * By proceeding further, you forfeit your soul.
 * 
 * If you change anything, I will hunt you down and kill you in your sleep. 
 * 
 * 
 * Note to Paulo: IF YOU STEAL MY COMMITS AGAIN, I WILL BREAK YOUR KNEECAPS!
 */

#include "parser/parser.h"

#define CSV_DELIMITER ';'
// #define CSV_DELIMITER_S LITERALIZE(CSV_DELIMITER)   // BUG: This thing is "';'", yet somehow still works, wtf?
#define CSV_DELIMITER_S MAKE_CHAR_ARRAY(CSV_DELIMITER) // This one makes sense.
// Post-mortem: Turns out, IT'S BECAUSE IT FUCKING WRECKS HAVOC ON THIS SHIT.

Tokens tokenize_csv(char* line, ssize_t len) {
    char* ptr = strdup(line);
    if (ptr == NULL) exit(EXIT_FAILURE);

    if (ptr[len - 1] == '\n') {
        ptr[len - 1] = '\0';
    }

    int seps = 1;
    for (int i = 0; line[i]; i++) seps += (line[i] == CSV_DELIMITER);

    char** arr = (char**)malloc(seps * sizeof(char*));
    memset(arr, 0, seps * sizeof(char*));

    char* token;
    int i = 0;
    while ((token = strsep(&ptr, CSV_DELIMITER_S)) != NULL) {
        char* tokenData = strdup(token);

        arr[i++] = tokenData;
    }

    Tokens ret = (Tokens)malloc(sizeof(TOKENS));
    ret->data = arr;
    ret->len = seps;

    free(ptr);
    return ret;
}

Tokens duplicate_tokens(Tokens orig) {
    Tokens dup = (Tokens)malloc(sizeof(TOKENS));

    dup->data = (char**)malloc(orig->len * sizeof(char*));
    dup->len = orig->len;

    for (int i = 0; i < orig->len; i++) dup->data[i] = strdup(orig->data[i]);
    return dup;
}

void destroy_tokens(Tokens tokens) {
    for (int i = 0; i < tokens->len; i++) free(tokens->data[i]);
    free(tokens->data);
    free(tokens);
}

ParserStore makeStore() {
    ParserStore store = (ParserStore)malloc(sizeof(PARSER_STORE));
    store->discard_file = NULL;

    return store;
}

void discard_to_file(Tokens tokens, FILE* store) {
    if (store == NULL) {
        printf("Could not discard tokens: Output file does not exist.");
        exit(EXIT_FAILURE);
    }

    int totalLen = tokens->len + 1;
    for (int i = 0; i < tokens->len; i++) totalLen += strlen(tokens->data[i]);

    char* joint = (char*)malloc(totalLen * sizeof(char));
    memset(joint, 0, totalLen * sizeof(char));

    for (int i = 0; i < tokens->len; i++) {
        strcat(joint, tokens->data[i]);
        if (i != tokens->len - 1) strcat(joint, ";");
    }
    joint[totalLen - 2] = '\n';

    fputs(joint, store);

    free(joint);
}

void parse(
    char* input,
    ssize_t input_len,
    Tokenizer(tokenizer),
    VerifyFunction(verifier), 
    ParseFunction(parser), 
    WriteFunction(writer), 
    WriteFunction(discarder),
    ParserStore store
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

    Tokens vertoks = duplicate_tokens(tokens);

    int valid = verifier(vertoks);
    if (!valid) {
        printf("INVALID LINE.\n");
        discarder(tokens, &store->discard_file);

        // TODO: Pass cleanup control to discarder?
        // Answer: FUCK NO
        destroy_tokens(vertoks);
        destroy_tokens(tokens);
        return;
    }

    Tokens partoks = duplicate_tokens(tokens);

    void* data = parser(partoks);
    if (data == NULL) {
        printf("Error while parsing: Unable to transform tokens.\n");
        return;
    }

    writer(data, NULL);

    destroy_tokens(vertoks);
    destroy_tokens(partoks);
    destroy_tokens(tokens);
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

    ParserStore store = makeStore();

    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    
    while ((read = getline(&line, &len, stream)) != -1) {
        parse(line, read, tokenizer, verifier, parser, writer, discarder, store);
    }

    CLOSE_FILE(stream);
}
