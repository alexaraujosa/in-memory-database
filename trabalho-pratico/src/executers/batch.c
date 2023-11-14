#include "executers/batch.h"

void test_preprocessor(FILE* stream, ParserStore store, va_list args) {
    gpointer null_element = NULL;
    g_array_append_vals(store, &null_element, 1); // Discard file
    default_csv_preprocessor(stream, store, args);   // File header
    //cvs_preprocessor_helper(stream, store);

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

    Catalog* user_catalog = catalog_init(&user_tree_compare_func, g_str_hash, g_str_equal);
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

    //TODO: Passenger catalog init
    char* passengersdata_path = join_paths(2, arg1, "passengers.csv");
    //TODO: Add flight id validation (more one catalog as parameter)
    parse_file(
        passengersdata_path,
        &tokenize_csv,
        &test_preprocessor,
        &verify_passenger_tokens,   //TODO: Change verify_passenger_tokens to receive store
        &test_parser, 
        &test_writer, 
        &discard_passenger,
        &default_csv_destructor,
        user_catalog
    );

    free(passengersdata_path);

    // TODO: Reservation catalog init
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
        user_catalog
    );

    free(reservationsdata_path);

    // TODO: Flight catalog init
    char* flightsdata_path = join_paths(2, arg1, "flights.csv");

    parse_file(
        flightsdata_path,
        &tokenize_csv,
        &test_preprocessor,
        &verify_flight_tokens, 
        &parse_flight, 
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
    // TODO: Catalogs destroy

    return;
}