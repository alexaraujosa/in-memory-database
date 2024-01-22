#include "executers/datasets.h"
#include "parser/parser.h"
#include "catalog/usersCatalog.h"
#include "catalog/genericCatalog.h"
#include "catalog/reservationsCatalog.h"
#include "catalog/flightsCatalog.h"
#include "catalog/passengersCatalog.h"
#include "catalog/catalogManager.h"
#include "tests/test.h"

#ifdef MAKE_TEST
// Refer to batch.c
extern double batch_test_parsing_time;
extern double batch_test_sorting_time;
extern char* batch_test_output_path;
extern FILE* batch_test_test_report;
#endif

typedef struct dataset_data {
    char* dataset_dir;
    int datasets_loaded;
    Catalog* users;
    Catalog* reservations;
    Catalog* passengers;
    Catalog* flights;
    Stats_info stats_info;
    GArray* flights2_array;

    // TODO: Kept for backwards compatibility with the current query implementation. 
    // Should be removed aftwerwards.
    void** _catalog_arr; 
} DATASET_DATA, *DatasetData;

// Forward declarations, for ease of reading.
void** make_catalogues(char* dataset_dir);

// ============== DATASET STORE ==============
DatasetData make_dataset_data(char* input) {
    DatasetData dd = (DatasetData)malloc(sizeof(DATASET_DATA));
    dd->datasets_loaded = FALSE;
    dd->users = NULL;
    dd->reservations = NULL;
    dd->passengers = NULL;
    dd->flights = NULL;
    dd->stats_info = NULL;
    dd->flights2_array = NULL;

    if (input != NULL) dd->dataset_dir = strdup(input);
    else dd->dataset_dir = NULL;

    return dd;
}

void dataset_data_set_dir(DatasetData dd, char* dir) {
    dd->dataset_dir = strdup(dir);
}

int dataset_data_load(DatasetData dd) {
    if (dd->dataset_dir == NULL) return 1;

    dd->_catalog_arr = make_catalogues(dd->dataset_dir);
    dd->users          = (Catalog*)(dd->_catalog_arr[0]);
    dd->flights        = (Catalog*)(dd->_catalog_arr[1]);
    dd->passengers     = (Catalog*)(dd->_catalog_arr[2]);
    dd->reservations   = (Catalog*)(dd->_catalog_arr[3]);
    dd->stats_info     = (Stats_info)(dd->_catalog_arr[4]);
    dd->flights2_array = (GArray*)(dd->_catalog_arr[5]);

    dd->datasets_loaded = TRUE;

    return 0;
}

char* dataset_data_get_dir(DatasetData dd) {
    return strdup(dd->dataset_dir);
}

int dataset_data_is_loaded(DatasetData dd) {
    return dd->datasets_loaded;
}

Catalog* dataset_data_get_users_catalog(DatasetData dd) {
    if (!dd->datasets_loaded) return NULL;
    return dd->users;
}

Catalog* dataset_data_get_flights_catalog(DatasetData dd) {
    if (!dd->datasets_loaded) return NULL;
    return dd->flights;
}

Catalog* dataset_data_get_passengers_catalog(DatasetData dd) {
    if (!dd->datasets_loaded) return NULL;
    return dd->passengers;
}

Catalog* dataset_data_get_reservations_catalog(DatasetData dd) {
    if (!dd->datasets_loaded) return NULL;
    return dd->reservations;
}

Stats_info dataset_data_get_stats_info(DatasetData dd) {
    if (!dd->datasets_loaded) return NULL;
    return dd->stats_info;
}

GArray* dataset_data_get_flights2_array(DatasetData dd) {
    if (!dd->datasets_loaded) return NULL;
    return dd->flights2_array;
}

void** dataset_data_get_catalog_array(DatasetData dd) {
    if (!dd->datasets_loaded) return NULL;
    return dd->_catalog_arr;
}

void destroy_dataset_data(DatasetData dd) {
    if (dd->dataset_dir != NULL) free(dd->dataset_dir);

    if (dd->datasets_loaded) {
        catalog_destroy(dd->users);
        catalog_destroy(dd->flights);
        catalog_destroy(dd->passengers);
        catalog_destroy(dd->reservations);
        stats_destroy(dd->stats_info);
        g_array_free(dd->flights2_array, TRUE);

        free(dd->_catalog_arr);
    }

    free(dd);
}

// ============== PARSE DATASETS ==============
gint flights2_compare(gconstpointer flight_A, gconstpointer flight_B) {
    const Flight Flight1 = *(const Flight *)flight_A;
    const Flight Flight2 = *(const Flight *)flight_B;

    int date1 = get_flight_schedule_departure_date(Flight1);
    int date2 = get_flight_schedule_departure_date(Flight2);
    if(date1 > date2) return 1;
    if(date1 < date2) return -1;
    return 0;
}

void _default_catalog_preprocessor(FILE* stream, ParserStore store, va_list args) {
    gpointer null_element = NULL;
    g_array_append_vals(store, &null_element, 1);  // Discard file
    default_csv_preprocessor(stream, store, args); // File header

    Catalog* catalogo = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo, 1);
}

void _reservation_preprocessor(FILE* stream, ParserStore store, va_list args) {
    gpointer null_element = NULL;
    g_array_append_vals(store, &null_element, 1);  // Discard file
    default_csv_preprocessor(stream, store, args); // File header

    Catalog* catalogo = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo, 1);

    Catalog* catalogo2 = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo2, 1);
}

void _passenger_preprocessor(FILE* stream, ParserStore store, va_list args) {
    gpointer null_element = NULL;
    g_array_append_vals(store, &null_element, 1);  // Discard file
    default_csv_preprocessor(stream, store, args); // File header

    Catalog* catalogo = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo, 1);

    Catalog* catalogo2 = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo2, 1);

    Catalog* catalogo3 = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo3, 1);
}

void csv_destructor_extended(FILE* stream, ParserStore store) {
    IGNORE_ARG(stream);

    FILE* discarder = g_array_index(store, FILE*, 0);
    if(discarder != NULL)  CLOSE_FILE(discarder);

    void** file_header = g_array_index(store, void**, 1);
    free(file_header);

    g_array_free(store, TRUE);
}

void** make_catalogues(char* dataset_dir) {
#ifdef MAKE_TEST
    batch_test_output_path = join_paths(2, get_cwd()->str, "Resultados/test_report.txt");
    FILE* batch_test_test_report = OPEN_FILE(batch_test_output_path, "w");

    // double sorting_time = 0;
    // double parsing_time = 0;

    printf("\n----===[  CATALOGS SETUP METRICS  ]===----\n\n");
    fprintf(batch_test_test_report, "\n----===[  CATALOGS SETUP METRICS  ]===----\n\n");

    clock_t start_time = clock();
#endif

    // TODO Verificar onde vai ficar a inicialização desta estrutura
    GArray* pointer_to_generic_catalog = generate_genCat();

    Catalog* user_catalog = catalog_init(g_str_hash, g_str_equal, free);
    char* userdata_path = join_paths(2, dataset_dir, "users.csv");
    parse_file(
        userdata_path,
        &tokenize_csv,
        &preprocessor_user,
        &verify_user_tokens,
        &parse_user,
        &usersCatalog_write_to_catalog,
        &discard_user,
        &csv_destructor_extended,
        user_catalog,
        pointer_to_generic_catalog
    );

#ifdef MAKE_TEST
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    batch_test_parsing_time += elapsed_time;
    printf(" - Execution time for parsing users: %.4f seconds\n", elapsed_time);
    fprintf(batch_test_test_report, " - Execution time for parsing users: %.4f seconds\n", elapsed_time);
#endif

    free(userdata_path);

    TEST_EXPR(start_time = clock();)
    catalog_sort(user_catalog, (GCompareFunc)&usersCatalog_full_compare_func);

#ifdef MAKE_TEST
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    batch_test_sorting_time += elapsed_time;
    printf(" - Execution time for sorting users: %.4f seconds\n\n", elapsed_time);
    fprintf(batch_test_test_report, " - Execution time for sorting users: %.4f seconds\n\n", elapsed_time);

    start_time = clock();
#endif

    Catalog* flight_catalog = catalog_init(g_direct_hash, g_direct_equal, NULL);
    char* flightsdata_path = join_paths(2, dataset_dir, "flights.csv");
    parse_file(
        flightsdata_path,
        &tokenize_csv,
        &preprocessor_flight,
        &verify_flight_tokens,
        &parse_flight,
        &flightsCatalog_write_to_catalog,
        &discard_flight,
        &csv_destructor_extended,
        flight_catalog,
        pointer_to_generic_catalog
    );


    //TODO: COLOCAR O MAKE_TEST E COLOCAR A FUNCAO flights2_compare NO LUGAR CORRETO, SEM SER NO TOPO DA BATCH
    GArray* flights2_array = catalog_get_array_copy(flight_catalog);
    g_array_sort(flights2_array, (GCompareFunc)&flights2_compare);

#ifdef MAKE_TEST
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    batch_test_parsing_time += elapsed_time;
    printf(" - Execution time for parsing flights: %.4f seconds\n", elapsed_time);
    fprintf(batch_test_test_report, " - Execution time for parsing flights: %.4f seconds\n", elapsed_time);
#endif
    free(flightsdata_path);

    TEST_EXPR(start_time = clock();)
    catalog_sort(flight_catalog, (GCompareFunc)&flightsCatalog_full_compare_func);

#ifdef MAKE_TEST
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    batch_test_sorting_time += elapsed_time;
    printf(" - Execution time for sorting flights: %.4f seconds\n\n", elapsed_time);
    fprintf(batch_test_test_report, " - Execution time for sorting flights: %.4f seconds\n\n", elapsed_time);

    start_time = clock();
#endif

    Catalog* passengers_catalog = catalog_init(NULL, NULL, NULL);
    char* passengersdata_path = join_paths(2, dataset_dir, "passengers.csv");
    parse_file(
        passengersdata_path,
        &tokenize_csv,
        &preprocessor_passenger,
        &verify_passenger_tokens,
        &parse_passenger,
        &passengersCatalog_write_to_catalog,
        &discard_passenger,
        &csv_destructor_extended,
        user_catalog,
        flight_catalog,
        passengers_catalog,
        pointer_to_generic_catalog
    );

#ifdef MAKE_TEST
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    batch_test_parsing_time += elapsed_time;
    printf(" - Execution time for parsing passengers: %.4f seconds\n", elapsed_time);
    fprintf(batch_test_test_report, " - Execution time for parsing passengers: %.4f seconds\n", elapsed_time);
#endif

    free(passengersdata_path);

    TEST_EXPR(start_time = clock();)
    // catalog_sort(passengers_catalog, (GCompareFunc)&passengersCatalog_full_compare_func);

#ifdef MAKE_TEST
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    batch_test_sorting_time += elapsed_time;
    printf(" - Execution time for sorting passengers: %.4f seconds\n\n", elapsed_time);
    fprintf(batch_test_test_report, " - Execution time for sorting passengers: %.4f seconds\n\n", elapsed_time);

    start_time = clock();
#endif

    Catalog* reservation_catalog = catalog_init(g_direct_hash, g_direct_equal, NULL);
    char* reservationsdata_path = join_paths(2, dataset_dir, "reservations.csv");
    parse_file(
        reservationsdata_path,
        &tokenize_csv,
        &preprocessor_reservation,
        &verify_reservation_tokens,
        &parse_reservation,
        &reservationsCatalog_write_to_catalog,
        &discard_reservation,
        &csv_destructor_extended,
        user_catalog,
        reservation_catalog,
        pointer_to_generic_catalog
    );

#ifdef MAKE_TEST
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    batch_test_parsing_time += elapsed_time;
    printf(" - Execution time for parsing reservations: %.4f seconds\n", elapsed_time);
    fprintf(batch_test_test_report, " - Execution time for parsing reservations: %.4f seconds\n", elapsed_time);
#endif

    free(reservationsdata_path);

    TEST_EXPR(start_time = clock();)
    catalog_sort(reservation_catalog, (GCompareFunc)&reservationsCatalog_full_compare_func);

#ifdef MAKE_TEST
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    batch_test_sorting_time += elapsed_time;
    printf(" - Execution time for sorting reservations: %.4f seconds\n\n", elapsed_time);
    fprintf(batch_test_test_report, " - Execution time for sorting reservations: %.4f seconds\n\n", elapsed_time);
#endif

    TEST_EXPR(start_time = clock();)
    Stats_info stats_info = create_stats_info(flight_catalog, pointer_to_generic_catalog);

#ifdef MAKE_TEST
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    batch_test_sorting_time += elapsed_time;
    printf(" - Execution time for inicializing statistics: %.4f seconds\n", elapsed_time);
    fprintf(batch_test_test_report, " - Execution time for inicializing statistics: %.4f seconds\n", elapsed_time);
#endif

    void** catalogues = (void**)malloc(6 * sizeof(Catalog*));
    catalogues[0] = user_catalog;
    catalogues[1] = flight_catalog;
    catalogues[2] = passengers_catalog;
    catalogues[3] = reservation_catalog;
    catalogues[4] = (Stats_info)stats_info;
    catalogues[5] = (GArray*)flights2_array;

    return catalogues;
}