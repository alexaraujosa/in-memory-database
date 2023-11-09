#include <stdio.h>

#include "collections/user.h"
#include "collections/reservation.h"
#include "collections/flight.h"
#include "util/io.h"
#include "parser/parser.h"
#include "queries/queries.h"

typedef struct test {
    char* id;
} TEST, *Test;

int ind = 0;

void testPreprocessor(FILE* stream, ParserStore store) {
    // size_t len = 0;
    // ssize_t read;
    // char* line =  getline(&line, &len, stream);

    // store->file_header = line;

    gpointer null_element = NULL;
    g_array_append_vals(store, &null_element, 1); // Discard file
    // default_cvs_preprocessor(stream, store);   // File header
    cvs_preprocessor_helper(stream, store);
}

int testVerifier(Tokens tokens) {
    // for (int i = 0; i < tokens->len; i++) printf("VERIFIER TOKENS %d: '%s'\n", i, tokens->data[i]);
    // if (++ind == 3) return 0; // FYIP
    // return 1;

    return 0;
}

void* testParser(Tokens tokens) {
    Test test = (Test)malloc(sizeof(TEST));
    test->id = strdup(tokens->data[0]);

    return test;
}

void testWriter(void* raw_data, FILE** store) {
    IGNORE_ARG(store);
    Test data = (Test)raw_data;
    printf("WRITE: %s\n", data->id);
    fflush(stdout);
}

// void testDiscarder(void* raw_data, FILE** store) {
//     Tokens tokens = (Tokens)raw_data;

//     if (*store == NULL) {
//         printf("DISTORE SET\n");
//         *store = (FILE*)0x1;
//     }

//     printf("DISTORE: '%p'\n", *store);

//     int totalLen = tokens->len;
//     for (int i = 0; i < tokens->len; i++) totalLen += strlen(tokens->data[i]);

//     char* joint = (char*)malloc(totalLen * sizeof(char));
//     memset(joint, 0, totalLen * sizeof(char));

//     for (int i = 0; i < tokens->len - 1; i++) {
//         strcat(joint, tokens->data[i]);
//         if (i != tokens->len - 1) strcat(joint, ";");
//     }

//     printf("DISCARD: '%s'\n", joint);
//     fflush(stdout);

//     free(joint);
// }

void testDiscarder(void* raw_data, ParserStore store) {// , FILE** store
    void** discard_file = &g_array_index(store, void*, 0);
    if (*discard_file == NULL) {
        char* parts[2] = {get_cwd()->str, "../Resultados/errors.csv"};
        char* full_path = join_paths(parts, 2);
        *discard_file = OPEN_FILE(full_path, "w");

        void** file_header = &g_array_index(store, void*, 1);
        rt_assert(
            *file_header != NULL,
            "Could not open discard file: Dataset header missing."
        );

        fputs(*file_header, *discard_file);
    }
    // if (store->discard_file == NULL) {
    //     char* parts[2] = {get_cwd()->str, "../Resultados/errors.csv"};
    //     char* full_path = join_paths(parts, 2);
    //     store->discard_file = OPEN_FILE(full_path, "w");

    //     fputs(store->file_header, store->discard_file);
    // }

    Tokens tokens = (Tokens)raw_data;

    discard_to_file(tokens, store);
}

void testDestructor(FILE* stream, ParserStore store) {
    default_csv_destructor(stream, store);
}

int main(int argc, char const *argv[]) {
    printf("Hello World!\n");

    RESERVATION res = makeReservation(
        1, 
        "123456789012345678901234567890123456789012345678901234567890",
        100,
        "098765432109876543210987654321098765432109876543210987654321",
        5,
        4,
        123456,
        4321,
        196,
        false,
        1
    );

    USER user = makeUser(
        "123456789012345678901234567890123456789012345678901234567890",
        "Manuel Maria Filipe Carlos Amélio Luís Miguel Rafael Gabriel Gonzaga Xavier "
            "Francisco de Assis Eugénio de Bragança Orleães Sabóia e Saxe-Coburgo-Gotha",
        false,
        "PT",
        12345,
        true
    );

    FLIGHT flight = makeFlight(
        1,
        "Fuckery Airlines",
        "Fuckonov 69420",
        "PQP",
        "CDC",
        12345,
        23456,
        34567
    );

    GString* bin = get_cwd();
    printf("BIN1: %s\n", bin->str);

    GString* bin2 = get_cwd();
    printf("BIN2: %s\n", bin2->str);

    char* parts1[2] = {bin->str, "../obj"};
    char* full_path1 = join_paths(parts1, 2);
    GArray* files = get_files(full_path1);
    if (files == NULL || files->len == 0) {
        printf("ALLAHU AKBAR: No files.\n");
        exit(EXIT_FAILURE);
    }
    for (guint i = 0; i < files->len; i++) {
        char *dirp = g_array_index(files, char*, i);
        g_print("FILE: '%s'\n", dirp);
    }

    char* parts2[2] = {bin->str, "../obj"};
    char* full_path2 = join_paths(parts2, 2);
    GArray* subdirs = get_subdirs(full_path2);
    if (subdirs == NULL || subdirs->len == 0) {
        printf("ALLAHU AKBAR: No subdirs.\n");
        exit(EXIT_FAILURE);
    }

    for (guint i = 0; i < subdirs->len; i++) {
        char *dirp = g_array_index(subdirs, char*, i);
        g_print("SUBDIR: '%s'\n", dirp);
    }

    // parse("../test2.txt", &tokenize_csv, &testVerifier, &testParser, &testWriter, &testDiscarder);
    parse_file(
        "../test2.txt", 
        // "/home/rafaelsf/Desktop/Cadeiras/2ano/LI3/Project/project/trabalho-pratico/test2 copy.txt",
        &tokenize_csv,
        &testPreprocessor,
        &testVerifier, 
        &testParser, 
        &testWriter, 
        &testDiscarder,
        &testDestructor
    );

    // query_run_single("2 U000000001", 12);
    query_run_bulk("../testq.txt", "ignore");
    
    return 0;
}

// #include "executers/batch.h"

// int main(int argc, char** argv) {
//     if (argc > 3) {
//         init_batch(argv[1], argv[2]);
//     } else {
//         printf("Interactive mode not implemented.\n");
//         exit(EXIT_FAILURE);
//     }

//     return 0;
// }
