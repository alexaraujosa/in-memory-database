#include "executers/batch.h"

void test_preprocessor(FILE* stream, ParserStore store, va_list args) {
    gpointer null_element = NULL;
    g_array_append_vals(store, &null_element, 1); // Discard file
    default_csv_preprocessor(stream, store, args);   // File header
    cvs_preprocessor_helper(stream, store);

    Catalog* catalogo = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo, 1);
}

void test_parser(Tokens tokens) {
    return;
}

void test_writer(void* raw_data, FILE** store) {
    fflush(stdout);
}

void batch(char* arg1, char* arg2) {

    Catalog* user_catalog = catalog_init(&user_tree_compare_func);

    // char* userdata_path_parts[2] = {arg1, "users.csv"};
    // char* userdata_path = join_paths(userdata_path_parts, 2);
    char* userdata_path = join_paths(2, arg1, "users.csv");

    parse_file(
        userdata_path,     
        &tokenize_csv,
        &test_preprocessor,
        &verify_user_tokens, 
        &parse_user, 
        &write_user, 
        &discard_user,
        &default_csv_destructor,
        user_catalog
    );  

    free(userdata_path);

    // char* passengersdata_path_parts[2] = {arg1, "passengers.csv"};
    // char* passengersdata_path = join_paths(passengersdata_path_parts, 2);
    char* passengersdata_path = join_paths(2, arg1, "passengers.csv");

    parse_file(
        passengersdata_path,
        &tokenize_csv,
        &test_preprocessor,
        &verify_passenger_tokens, 
        &test_parser, 
        &test_writer, 
        &discard_passenger,
        &default_csv_destructor,
        "AAAAAAAAAAAAAA"
    );

    free(passengersdata_path);

    // char* reservationsdata_path_parts[2] = {arg1, "reservations.csv"};
    // char* reservationsdata_path = join_paths(reservationsdata_path_parts, 2);
    char* reservationsdata_path = join_paths(2, arg1, "reservations.csv");

    parse_file(
        reservationsdata_path,
        &tokenize_csv,
        &test_preprocessor,
        &verify_reservation_tokens, 
        &test_parser, 
        &test_writer, 
        &discard_reservation,
        &default_csv_destructor,
        "Lorem ipsum dolore sit amet"
    );

    free(reservationsdata_path);

    // char* flightsdata_path_parts[2] = {arg1, "flights.csv"};
    // char* flightsdata_path = join_paths(flightsdata_path_parts, 2);
    char* flightsdata_path = join_paths(2, arg1, "flights.csv");

    parse_file(
        flightsdata_path,
        &tokenize_csv,
        &test_preprocessor,
        &verify_flight_tokens, 
        &test_parser, 
        &test_writer, 
        &discard_flight,
        &default_csv_destructor,
        "consequitur adipisicing elit."
    );

    free(flightsdata_path);

    // Cleanup cwd
    g_free(get_cwd()->str);
    g_free(get_cwd());

    catalog_destroy(user_catalog);

    return;
}