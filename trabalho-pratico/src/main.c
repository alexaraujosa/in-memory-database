#include <stdio.h>

#include "collections/user.h"
#include "collections/reservation.h"
#include "collections/flight.h"
#include "util/io.h"
#include "parser/parser.h"
#include "queries/queries.h"

#include "parser/verifier.h"

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
    // Test test = (Test)malloc(sizeof(TEST));
    // test->id = strdup(tokens->data[0]);

    // return test;
    return;
}

void testWriter(void* raw_data, FILE** store) {
    // IGNORE_ARG(store);
    // Test data = (Test)raw_data;
    //printf("WRITE: %s\n", data->id);
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

void discard_user(void* raw_data, ParserStore store) {
    void** discard_file = &g_array_index(store, void*, 0);
    if (*discard_file == NULL) {
        char* parts[2] = {get_cwd()->str, "Resultados/users_errors.csv"};
        char* full_path = join_paths(parts, 2);
        *discard_file = OPEN_FILE(full_path, "w");

        void** file_header = &g_array_index(store, void*, 1);
        rt_assert(
            *file_header != NULL,
            "Could not open discard file: Dataset header missing."
        );

        fputs(*file_header, *discard_file);
        free(full_path);
    }
    Tokens tokens = (Tokens)raw_data;

    discard_to_file(tokens, store);
}

void discard_passenger(void* raw_data, ParserStore store) {
    void** discard_file = &g_array_index(store, void*, 0);
    if (*discard_file == NULL) {
        char* parts[2] = {get_cwd()->str, "Resultados/passengers_errors.csv"};
        char* full_path = join_paths(parts, 2);
        *discard_file = OPEN_FILE(full_path, "w");

        void** file_header = &g_array_index(store, void*, 1);
        rt_assert(
            *file_header != NULL,
            "Could not open discard file: Dataset header missing."
        );

        fputs(*file_header, *discard_file);
        free(full_path);
    }

    Tokens tokens = (Tokens)raw_data;

    discard_to_file(tokens, store);
}

void discard_flight(void* raw_data, ParserStore store) {
    void** discard_file = &g_array_index(store, void*, 0);
    if (*discard_file == NULL) {
        char* parts[2] = {get_cwd()->str, "Resultados/flights_errors.csv"};
        char* full_path = join_paths(parts, 2);
        *discard_file = OPEN_FILE(full_path, "w");

        void** file_header = &g_array_index(store, void*, 1);
        rt_assert(
            *file_header != NULL,
            "Could not open discard file: Dataset header missing."
        );

        fputs(*file_header, *discard_file);
        free(full_path);
    }

    Tokens tokens = (Tokens)raw_data;

    discard_to_file(tokens, store);
}

void discard_reservation(void* raw_data, ParserStore store) {
    void** discard_file = &g_array_index(store, void*, 0);
    if (*discard_file == NULL) {
        char* parts[2] = {get_cwd()->str, "Resultados/reservations_errors.csv"};
        char* full_path = join_paths(parts, 2);
        *discard_file = OPEN_FILE(full_path, "w");

        void** file_header = &g_array_index(store, void*, 1);
        rt_assert(
            *file_header != NULL,
            "Could not open discard file: Dataset header missing."
        );

        fputs(*file_header, *discard_file);
        free(full_path);
    }

    Tokens tokens = (Tokens)raw_data;

    discard_to_file(tokens, store);
}

void testDestructor(FILE* stream, ParserStore store) {
    default_csv_destructor(stream, store);
}

int main(int argc, char const *argv[]) {


    // init_batch(recebe o argv[0] como argumento)

    char* userdata_path_parts[2] = {"/mnt/data", "users.csv"};
    char* userdata_path = join_paths(userdata_path_parts, 2);

    char* flightsdata_path_parts[2] = {"/mnt/data", "flights.csv"};
    char* flightsdata_path = join_paths(flightsdata_path_parts, 2);

    char* passengersdata_path_parts[2] = {"/mnt/data", "passengers.csv"};
    char* passengersdata_path = join_paths(passengersdata_path_parts, 2);

    char* reservationsdata_path_parts[2] = {"/mnt/data", "reservations.csv"};
    char* reservationsdata_path = join_paths(reservationsdata_path_parts, 2);

    parse_file(
        userdata_path,     
        &tokenize_csv,
        &testPreprocessor,
        &verify_user_tokens, 
        &testParser, 
        &testWriter, 
        &discard_user,
        &testDestructor
    );

    parse_file(
        passengersdata_path,
        &tokenize_csv,
        &testPreprocessor,
        &verify_passenger_tokens, 
        &testParser, 
        &testWriter, 
        &discard_passenger,
        &testDestructor
    );

    parse_file(
        reservationsdata_path,
        &tokenize_csv,
        &testPreprocessor,
        &verify_reservation_tokens, 
        &testParser, 
        &testWriter, 
        &discard_reservation,
        &testDestructor
    );

    parse_file(
        flightsdata_path,
        &tokenize_csv,
        &testPreprocessor,
        &verify_flight_tokens, 
        &testParser, 
        &testWriter, 
        &discard_flight,
        &testDestructor
    );

    free(userdata_path);
    free(passengersdata_path);
    free(flightsdata_path);
    free(reservationsdata_path);

    g_free(get_cwd()->str);
    g_free(get_cwd());


    // query_run_single("2 U000000001", 12);
    // query_run_bulk("../testq.txt", "ignore");

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
