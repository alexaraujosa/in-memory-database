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
    // char* ptr = line;
    char* ptr = strdup(line);
    char* ptr_root = ptr;

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

    free(ptr_root);
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
    ParserStore store = g_array_new(FALSE, FALSE, sizeof(void*));

    return store;
}

void discard_to_file(Tokens tokens, ParserStore store) {
    if (store == NULL) {
        printf("Could not discard tokens: Store does not exist.");
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

    FILE** discard_file = (FILE**)&g_array_index(store, void*, 0);
    fputs(joint, *discard_file);

    free(joint);
}

void cvs_preprocessor_helper(FILE* stream, ParserStore store) {
    char* line = NULL;
    size_t len = 0;
    ssize_t _ = getline(&line, &len, stream);
    IGNORE_ARG(_);

    char** file_header = (char**)&g_array_index(store, void*, 1);
    *file_header = line;
}

void default_csv_preprocessor(FILE* stream, ParserStore store, va_list args) {
    IGNORE_ARG(args);

    gpointer null_element = NULL;
    g_array_append_vals(store, &null_element, 1);
    cvs_preprocessor_helper(stream, store);
}

void default_csv_destructor(FILE* stream, ParserStore store) {
    IGNORE_ARG(stream);

    FILE* discarder = g_array_index(store, FILE*, 0);
    if(discarder != NULL)  CLOSE_FILE(discarder);

    void** file_header = g_array_index(store, void**, 1);
    free(file_header);



    for (guint i = 2; i < store->len - 1; ++i) {
        void* element = g_array_index(store, void*, i);
        free(element);
    }

    g_array_free(store, TRUE);
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

    int valid = verifier(tokens, store);
    if (!valid) {
        discarder(tokens, store);

        // Pass cleanup control to discarder?
        // Answer: FUCK NO
        destroy_tokens(tokens);
        return;
    }

    void* data = parser(tokens);
    if (data == NULL) {
        printf("Error while parsing: Unable to transform tokens.\n");
        return;
    }

    writer(data, store);

    destroy_tokens(tokens);
}

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

    ParserStore store = makeStore();

    char* path;
    
    if (is_path_absolute(filename)) {
        path = filename;
    } else {
        path = join_paths(2, get_cwd()->str, filename);
    }

    FILE* stream = OPEN_FILE(path, "r");

    // Preprocess store
    va_list args;
    va_start(args, destructor);

    preprocess(stream, store, args);

    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    
    while ((read = getline(&line, &len, stream)) != -1) {
        parse(line, read, tokenizer, verifier, parser, writer, discarder, store);
    }

    free(line);

    destructor(stream, store);

    CLOSE_FILE(stream);

    if (!is_path_absolute(filename)) free(path);

    va_end(args);
}
