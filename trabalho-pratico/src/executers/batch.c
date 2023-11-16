#include "executers/batch.h"

#include "stats/stats.h"

void test_preprocessor(FILE* stream, ParserStore store, va_list args) {
    gpointer null_element = NULL;
    g_array_append_vals(store, &null_element, 1); // Discard file
    default_csv_preprocessor(stream, store, args);   // File header
    //cvs_preprocessor_helper(stream, store);

    Catalog* catalogo = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo, 1);
}

void test_preprocessor_reservation(FILE* stream, ParserStore store, va_list args) {
    gpointer null_element = NULL;
    g_array_append_vals(store, &null_element, 1); // Discard file
    default_csv_preprocessor(stream, store, args);   // File header
    //cvs_preprocessor_helper(stream, store);

    Catalog* catalogo = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo, 1);

    Catalog* catalogo2 = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo2, 1);
}

void test_preprocessor_passenger(FILE* stream, ParserStore store, va_list args) {
    gpointer null_element = NULL;
    g_array_append_vals(store, &null_element, 1); // Discard file
    default_csv_preprocessor(stream, store, args);   // File header
    //cvs_preprocessor_helper(stream, store);

    Catalog* catalogo = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo, 1);

    Catalog* catalogo2 = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo2, 1);

    Catalog* catalogo3 = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo3, 1);
}
void default_csv_destructor_reservation(FILE* stream, ParserStore store) {
    FILE* discarder = g_array_index(store, FILE*, 0);
    if(discarder != NULL)  CLOSE_FILE(discarder);

    void** file_header = g_array_index(store, void**, 1);
    free(file_header);

    g_array_free(store, TRUE);
}

void test_parser(Tokens tokens) {
    return;
}

void test_writer(void* raw_data, FILE** store) {
    fflush(stdout);
}

void batch(const char* arg1, const char* arg2) {

    Catalog* user_catalog = catalog_init(g_str_hash, g_str_equal, free);
    char* userdata_path = join_paths(2, arg1, "users.csv");
    parse_file(
        userdata_path,     
        &tokenize_csv,
        &test_preprocessor,
        &verify_user_tokens, 
        &parse_user, 
        &usersCatalog_write_to_catalog, 
        &discard_user,
        &default_csv_destructor,
        user_catalog
    );  
    free(userdata_path);
    catalog_sort(user_catalog, &usersCatalog_full_compare_func);
    //catalog_print_array(user_catalog, &print_user);

    Catalog* flight_catalog = catalog_init(g_direct_hash, g_direct_equal, NULL);
    char* flightsdata_path = join_paths(2, arg1, "flights.csv");
    parse_file(
        flightsdata_path,
        &tokenize_csv,
        &test_preprocessor,
        &verify_flight_tokens, 
        &parse_flight, 
        &flightsCatalog_write_to_catalog, 
        &discard_flight,
        &default_csv_destructor,
        flight_catalog
    );
    free(flightsdata_path);
    catalog_sort(flight_catalog, &flightsCatalog_full_compare_func);
    //catalog_print_array(flight_catalog, &print_flight);

    Catalog* passengers_catalog = catalog_init(NULL, NULL, NULL);
    char* passengersdata_path = join_paths(2, arg1, "passengers.csv");
    parse_file(
        passengersdata_path,
        &tokenize_csv,
        &test_preprocessor_passenger,
        &verify_passenger_tokens,
        &parse_passenger, 
        &passengersCatalog_write_to_catalog, 
        &discard_passenger,
        &default_csv_destructor_reservation,
        user_catalog,
        flight_catalog,
        passengers_catalog
    );
    free(passengersdata_path);
    catalog_sort(passengers_catalog, &passengersCatalog_full_compare_func);
    //catalog_print_array(passengers_catalog, &passengersCatalog_print_array);

    Catalog* reservation_catalog = catalog_init(g_direct_hash, g_direct_equal, NULL);
    char* reservationsdata_path = join_paths(2, arg1, "reservations.csv");
    parse_file(
        reservationsdata_path,
        &tokenize_csv,
        &test_preprocessor_reservation,
        &verify_reservation_tokens, 
        &parse_reservation, 
        &reservationsCatalog_write_to_catalog, 
        &discard_reservation,
        &default_csv_destructor_reservation,
        user_catalog,
        reservation_catalog
    );//TODO: passar o reservation_catalog, pq preciso de guardar os valores das reservas nele
    free(reservationsdata_path);
    
    catalog_sort(reservation_catalog, &reservationsCatalog_full_compare_func);
    
    //catalog_print_array(reservation_catalog, &print_reservation);

    double rating = 0;
    rating = calculate_hotel_average_rating(reservation_catalog, 1001);
    printf("Rating = %.3f\n", rating);
    rating = calculate_hotel_average_rating(reservation_catalog, 1002);
    printf("Rating = %.3f\n", rating);
    rating = calculate_hotel_average_rating(reservation_catalog, 1003);
    printf("Rating = %.3f\n", rating);
    
    int n_flights = calculate_user_n_flights(passengers_catalog, "JéssiTavares910");
    printf("n_flights = %d\n", n_flights);

    double total_spent = calculate_user_total_spent(reservation_catalog, "JéssiTavares910");
    printf("total_spent = %.3f\n", total_spent);

    // Run queries
    Catalog** catalogues = (Catalog*)malloc(4 * sizeof(Catalog*));
    catalogues[0] = user_catalog;
    catalogues[1] = flight_catalog;
    catalogues[2] = passengers_catalog;
    catalogues[3] = reservation_catalog;

    query_run_bulk((char* )arg2, "Resultados", catalogues);


    // Cleanup cwd
    g_free(get_cwd()->str);
    g_free(get_cwd());

    catalog_destroy(user_catalog);
    catalog_destroy(flight_catalog);
    catalog_destroy(passengers_catalog);
    catalog_destroy(reservation_catalog);

    return;
}