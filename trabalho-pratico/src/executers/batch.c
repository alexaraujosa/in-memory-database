#include "locale.h"
#include "executers/batch.h"
#include "tests/test.h"

void default_preprocessor(FILE* stream, ParserStore store, va_list args) {
    gpointer null_element = NULL;
    g_array_append_vals(store, &null_element, 1); // Discard file
    default_csv_preprocessor(stream, store, args);   // File header
    //cvs_preprocessor_helper(stream, store);

    Catalog* catalogo = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo, 1);
}

void preprocessor_reservation(FILE* stream, ParserStore store, va_list args) {
    gpointer null_element = NULL;
    g_array_append_vals(store, &null_element, 1); // Discard file
    default_csv_preprocessor(stream, store, args);   // File header
    //cvs_preprocessor_helper(stream, store);

    Catalog* catalogo = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo, 1);

    Catalog* catalogo2 = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo2, 1);
}

void preprocessor_passenger(FILE* stream, ParserStore store, va_list args) {
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

// TODO Stream unused value
void default_csv_destructor_reservation(FILE* stream, ParserStore store) {
    IGNORE_ARG(stream);

    FILE* discarder = g_array_index(store, FILE*, 0);
    if(discarder != NULL)  CLOSE_FILE(discarder);

    void** file_header = g_array_index(store, void**, 1);
    free(file_header);

    g_array_free(store, TRUE);
}

void batch(const char* arg1, const char* arg2) {
    setlocale(LC_COLLATE, "en_US.UTF-8");
    //setenv("TZ", "", 1);
    #ifdef MAKE_TEST
        double sorting_time = 0;
        double parsing_time = 0;
        printf("\n----===[  CATALOGS SETUP METRICS  ]===----\n\n");
        clock_t start_time = clock();
    #endif

    Catalog* user_catalog = catalog_init(g_str_hash, g_str_equal, free);
    char* userdata_path = join_paths(2, arg1, "users.csv");
    parse_file(
        userdata_path,     
        &tokenize_csv,
        &default_preprocessor,
        &verify_user_tokens, 
        &parse_user, 
        &usersCatalog_write_to_catalog, 
        &discard_user,
        &default_csv_destructor,
        user_catalog
    );  
    
    #ifdef MAKE_TEST
        clock_t end_time = clock();
        double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        parsing_time += elapsed_time;
        printf(" - Execution time for parsing users: %.4f seconds\n", elapsed_time);
    #endif
    free(userdata_path);
    
    TEST_EXPR(start_time = clock();)
    catalog_sort(user_catalog, (GCompareFunc)&usersCatalog_full_compare_func);
    #ifdef MAKE_TEST
        end_time = clock();
        elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        sorting_time += elapsed_time;
        printf(" - Execution time for sorting users: %.4f seconds\n\n", elapsed_time);

        start_time = clock();
    #endif

    Catalog* flight_catalog = catalog_init(g_direct_hash, g_direct_equal, NULL);
    char* flightsdata_path = join_paths(2, arg1, "flights.csv");
    parse_file(
        flightsdata_path,
        &tokenize_csv,
        &default_preprocessor,
        &verify_flight_tokens, 
        &parse_flight, 
        &flightsCatalog_write_to_catalog, 
        &discard_flight,
        &default_csv_destructor,
        flight_catalog
    );
    
    #ifdef MAKE_TEST
        end_time = clock();
        elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        parsing_time += elapsed_time;
        printf(" - Execution time for parsing flights: %.4f seconds\n", elapsed_time);
    #endif
    free(flightsdata_path);

    TEST_EXPR(start_time = clock();)
    catalog_sort(flight_catalog, (GCompareFunc)&flightsCatalog_full_compare_func);
    
    #ifdef MAKE_TEST
        end_time = clock();
        elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        sorting_time += elapsed_time;
        printf(" - Execution time for sorting flights: %.4f seconds\n\n", elapsed_time);

        start_time = clock();
    #endif

    Catalog* passengers_catalog = catalog_init(NULL, NULL, NULL);
    char* passengersdata_path = join_paths(2, arg1, "passengers.csv");
    parse_file(
        passengersdata_path,
        &tokenize_csv,
        &preprocessor_passenger,
        &verify_passenger_tokens,
        &parse_passenger, 
        &passengersCatalog_write_to_catalog, 
        &discard_passenger,
        &default_csv_destructor_reservation,
        user_catalog,
        flight_catalog,
        passengers_catalog
    );
    
    #ifdef MAKE_TEST
        end_time = clock();
        elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        parsing_time += elapsed_time;
        printf(" - Execution time for parsing passengers: %.4f seconds\n", elapsed_time);
    #endif
    free(passengersdata_path);

    TEST_EXPR(start_time = clock();)
    catalog_sort(passengers_catalog, (GCompareFunc)&passengersCatalog_full_compare_func);
    
    #ifdef MAKE_TEST
        end_time = clock();
        elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        sorting_time += elapsed_time;
        printf(" - Execution time for sorting passengers: %.4f seconds\n\n", elapsed_time);

        start_time = clock();
    #endif

    Catalog* reservation_catalog = catalog_init(g_direct_hash, g_direct_equal, NULL);
    char* reservationsdata_path = join_paths(2, arg1, "reservations.csv");
    parse_file(
        reservationsdata_path,
        &tokenize_csv,
        &preprocessor_reservation,
        &verify_reservation_tokens, 
        &parse_reservation, 
        &reservationsCatalog_write_to_catalog, 
        &discard_reservation,
        &default_csv_destructor_reservation,
        user_catalog,
        reservation_catalog
    );
    
    #ifdef MAKE_TEST
        end_time = clock();
        elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        parsing_time += elapsed_time;
        printf(" - Execution time for parsing reservations: %.4f seconds\n", elapsed_time);
    #endif
    free(reservationsdata_path);
    
    TEST_EXPR(start_time = clock();)
    catalog_sort(reservation_catalog, (GCompareFunc)&reservationsCatalog_full_compare_func);
    
    #ifdef MAKE_TEST
        end_time = clock();
        elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
        sorting_time += elapsed_time;
        printf(" - Execution time for sorting reservations: %.4f seconds\n", elapsed_time);
    #endif
    //catalog_print_array(user_catalog, &print_user);
    //catalog_print_array(passengers_catalog, &print_passenger);
    
    //catalog_print_array(reservation_catalog, &print_reservation);

    /*PAULO IS TESTING WITH THIS...DONT TOUCH
    
    int n_flights = calculate_user_n_flights(passengers_catalog, "JéssiTavares910");
    printf("n_flights = %d\n", n_flights);

    int n_reservations = 10;
    double total_spent = calculate_user_total_spent(reservation_catalog, "JéssiTavares910", &n_reservations);
    printf("total_spent = %.3f && n_reservations:%d\n", total_spent, n_reservations);

    int n_passengers = calculate_flight_total_passengers(passengers_catalog, 444);
    printf("n_passengers = %d\n", n_passengers);

    for(int i = 0; i < catalog_get_item_count(flight_catalog); i++){
        void* flight = catalog_search_in_array(flight_catalog, i);
        if(strcmp(get_flight_origin(flight),"WAW") == 0){
            print_flight(flight);
            printf("n_passengers = %d\n", calculate_flight_total_passengers(passengers_catalog, get_flight_id(flight)));
        }
    }

    int year = 2023;
    int n_passengers = calculate_aeroport_n_passengers(flight_catalog, passengers_catalog, "MAD", &year);
    catalog_sort(flight_catalog, (GCompareFunc)&flight_destination_compare_func);
    int n_passengers2 = calculate_aeroport_n_passengers2(flight_catalog, passengers_catalog, "MAD", &year);
    printf("n_passengers = %d\n", n_passengers2+n_passengers);
    */


    // Run queries
    Catalog** catalogues = (Catalog**)malloc(4 * sizeof(Catalog*));
    catalogues[0] = user_catalog;
    catalogues[1] = flight_catalog;
    catalogues[2] = passengers_catalog;
    catalogues[3] = reservation_catalog;
    
    TEST_EXPR(printf("\n----===[  QUERY EXECUTION METRICS  ]===----\n\n");)

    query_run_bulk((char* )arg2, "Resultados", catalogues);

    TEST_EXPR(printf(" -> Execution time for parsing all collections datasets: %.4f seconds.\n", parsing_time);)
    TEST_EXPR(printf(" -> Execution time for sorting all collections datasets: %.4f seconds.\n", sorting_time);)


    // Cleanup cwd
    // g_free(get_cwd()->str);
    // g_free(get_cwd());

    catalog_destroy(user_catalog);
    catalog_destroy(flight_catalog);
    catalog_destroy(passengers_catalog);
    catalog_destroy(reservation_catalog);

    return;
}