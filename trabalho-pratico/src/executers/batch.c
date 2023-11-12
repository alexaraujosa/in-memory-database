#include "executers/batch.h"

void test_preprocessor(FILE* stream, ParserStore store) {
    gpointer null_element = NULL;
    g_array_append_vals(store, &null_element, 1); // Discard file
    // default_cvs_preprocessor(stream, store);   // File header
    cvs_preprocessor_helper(stream, store);
}

void test_parser(Tokens tokens) {
    return;
}

void test_writer(void* raw_data, FILE** store) {
    fflush(stdout);
}

void batch(char* arg1, char* arg2) {

    char* userdata_path_parts[2] = {arg1, "users.csv"};
    char* userdata_path = join_paths(userdata_path_parts, 2);

    parse_file(
        userdata_path,     
        &tokenize_csv,
        &test_preprocessor,
        &verify_user_tokens, 
        &test_parser, 
        &test_writer, 
        &discard_user,
        &default_csv_destructor
    );  

    free(userdata_path);

    char* passengersdata_path_parts[2] = {arg1, "passengers.csv"};
    char* passengersdata_path = join_paths(passengersdata_path_parts, 2);

    parse_file(
        passengersdata_path,
        &tokenize_csv,
        &test_preprocessor,
        &verify_passenger_tokens, 
        &test_parser, 
        &test_writer, 
        &discard_passenger,
        &default_csv_destructor
    );

    free(passengersdata_path);

    char* reservationsdata_path_parts[2] = {arg1, "reservations.csv"};
    char* reservationsdata_path = join_paths(reservationsdata_path_parts, 2);

    parse_file(
        reservationsdata_path,
        &tokenize_csv,
        &test_preprocessor,
        &verify_reservation_tokens, 
        &test_parser, 
        &test_writer, 
        &discard_reservation,
        &default_csv_destructor
    );

    free(reservationsdata_path);

    char* flightsdata_path_parts[2] = {arg1, "flights.csv"};
    char* flightsdata_path = join_paths(flightsdata_path_parts, 2);

    parse_file(
        flightsdata_path,
        &tokenize_csv,
        &test_preprocessor,
        &verify_flight_tokens, 
        &test_parser, 
        &test_writer, 
        &discard_flight,
        &default_csv_destructor
    );

    free(flightsdata_path);

    g_free(get_cwd()->str);
    g_free(get_cwd());

    return;
}