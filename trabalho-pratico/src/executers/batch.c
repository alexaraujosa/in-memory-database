#include "executers/batch.h"

#include "locale.h"
#include "tests/test.h"

void default_preprocessor(FILE* stream, ParserStore store, va_list args) {
    gpointer null_element = NULL;
    g_array_append_vals(store, &null_element, 1);   // Discard file
    default_csv_preprocessor(stream, store, args);  // File header
    // cvs_preprocessor_helper(stream, store);

    Catalog* catalogo = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo, 1);
}

void preprocessor_reservation(FILE* stream, ParserStore store, va_list args) {
    gpointer null_element = NULL;
    g_array_append_vals(store, &null_element, 1);   // Discard file
    default_csv_preprocessor(stream, store, args);  // File header
    // cvs_preprocessor_helper(stream, store);

    Catalog* catalogo = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo, 1);

    Catalog* catalogo2 = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo2, 1);
}

void preprocessor_passenger(FILE* stream, ParserStore store, va_list args) {
    gpointer null_element = NULL;
    g_array_append_vals(store, &null_element, 1);   // Discard file
    default_csv_preprocessor(stream, store, args);  // File header
    // cvs_preprocessor_helper(stream, store);

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
    if (discarder != NULL) CLOSE_FILE(discarder);

    void** file_header = g_array_index(store, void**, 1);
    free(file_header);

    g_array_free(store, TRUE);
}

void batch(const char* arg1, const char* arg2) {
#ifdef MAKE_TEST
    char* output_path = join_paths(2, get_cwd()->str, "Resultados/test_report.txt");
    FILE* test_report = OPEN_FILE(output_path, "w");
    double sorting_time = 0;
    double parsing_time = 0;
    printf("\n----===[  CATALOGS SETUP METRICS  ]===----\n\n");
    fprintf(test_report, "\n----===[  CATALOGS SETUP METRICS  ]===----\n\n");
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
        user_catalog);

#ifdef MAKE_TEST
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    parsing_time += elapsed_time;
    printf(" - Execution time for parsing users: %.4f seconds\n", elapsed_time);
    fprintf(test_report, " - Execution time for parsing users: %.4f seconds\n", elapsed_time);
#endif
    free(userdata_path);

    TEST_EXPR(start_time = clock();)
    catalog_sort(user_catalog, (GCompareFunc)&usersCatalog_full_compare_func);
#ifdef MAKE_TEST
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    sorting_time += elapsed_time;
    printf(" - Execution time for sorting users: %.4f seconds\n\n", elapsed_time);
    fprintf(test_report, " - Execution time for sorting users: %.4f seconds\n\n", elapsed_time);

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
        flight_catalog);

#ifdef MAKE_TEST
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    parsing_time += elapsed_time;
    printf(" - Execution time for parsing flights: %.4f seconds\n", elapsed_time);
    fprintf(test_report, " - Execution time for parsing flights: %.4f seconds\n", elapsed_time);
#endif
    free(flightsdata_path);

    TEST_EXPR(start_time = clock();)
    catalog_sort(flight_catalog, (GCompareFunc)&flightsCatalog_full_compare_func);

#ifdef MAKE_TEST
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    sorting_time += elapsed_time;
    printf(" - Execution time for sorting flights: %.4f seconds\n\n", elapsed_time);
    fprintf(test_report, " - Execution time for sorting flights: %.4f seconds\n\n", elapsed_time);

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
        passengers_catalog);

#ifdef MAKE_TEST
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    parsing_time += elapsed_time;
    printf(" - Execution time for parsing passengers: %.4f seconds\n", elapsed_time);
    fprintf(test_report, " - Execution time for parsing passengers: %.4f seconds\n", elapsed_time);
#endif
    free(passengersdata_path);

    TEST_EXPR(start_time = clock();)
    catalog_sort(passengers_catalog, (GCompareFunc)&passengersCatalog_full_compare_func);

#ifdef MAKE_TEST
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    sorting_time += elapsed_time;
    printf(" - Execution time for sorting passengers: %.4f seconds\n\n", elapsed_time);
    fprintf(test_report, " - Execution time for sorting passengers: %.4f seconds\n\n", elapsed_time);

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
        reservation_catalog);

#ifdef MAKE_TEST
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    parsing_time += elapsed_time;
    printf(" - Execution time for parsing reservations: %.4f seconds\n", elapsed_time);
    fprintf(test_report, " - Execution time for parsing reservations: %.4f seconds\n", elapsed_time);
#endif
    free(reservationsdata_path);

    TEST_EXPR(start_time = clock();)
    catalog_sort(reservation_catalog, (GCompareFunc)&reservationsCatalog_full_compare_func);

#ifdef MAKE_TEST
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    sorting_time += elapsed_time;
    printf(" - Execution time for sorting reservations: %.4f seconds\n\n", elapsed_time);
    fprintf(test_report, " - Execution time for sorting reservations: %.4f seconds\n\n", elapsed_time);
#endif

    TEST_EXPR(start_time = clock();)
    Stats_info stats_info = create_stats_info(flight_catalog);

#ifdef MAKE_TEST
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    sorting_time += elapsed_time;
    printf(" - Execution time for inicializing statistics: %.4f seconds\n", elapsed_time);
    fprintf(test_report, " - Execution time for inicializing statistics: %.4f seconds\n", elapsed_time);
#endif

    // Run queries
    void** catalogues = (void**)malloc(5 * sizeof(void*));
    catalogues[0] = (Catalog*)user_catalog;
    catalogues[1] = (Catalog*)flight_catalog;
    catalogues[2] = (Catalog*)passengers_catalog;
    catalogues[3] = (Catalog*)reservation_catalog;
    catalogues[4] = (Stats_info)stats_info;

    TEST_EXPR(printf("\n----===[  QUERY EXECUTION METRICS  ]===----\n\n");)
    TEST_EXPR(fprintf(test_report, "\n----===[  QUERY EXECUTION METRICS  ]===----\n\n");)
    TEST_EXPR(CLOSE_FILE(test_report);)
    query_run_bulk((char*)arg2, "Resultados", catalogues);

    TEST_EXPR(test_report = OPEN_FILE(output_path, "a");)
    TEST_EXPR(printf(" -> Execution time for parsing all collections datasets: %.4f seconds.\n", parsing_time);)
    TEST_EXPR(fprintf(test_report, " -> Execution time for parsing all collections datasets: %.4f seconds.\n", parsing_time);)
    TEST_EXPR(printf(" -> Execution time for sorting all collections datasets: %.4f seconds.\n", sorting_time);)
    TEST_EXPR(fprintf(test_report, " -> Execution time for sorting all collections datasets: %.4f seconds.\n", sorting_time);)
    TEST_EXPR(CLOSE_FILE(test_report);)

    // Cleanup cwd
    // g_free(get_cwd()->str);
    // g_free(get_cwd());

    catalog_destroy(user_catalog);
    catalog_destroy(flight_catalog);
    catalog_destroy(passengers_catalog);
    catalog_destroy(reservation_catalog);
    stats_destroy(stats_info);

    return;
}