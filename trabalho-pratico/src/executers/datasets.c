#include "executers/datasets.h"
#include "parser/parser.h"
#include "catalog/usersCatalog.h"
#include "catalog/reservationsCatalog.h"
#include "catalog/flightsCatalog.h"
#include "catalog/passengersCatalog.h"
#include "catalog/catalogManager.h"

typedef struct dataset_data {
    char* dataset_dir;
    int datasets_loaded;
    Catalog* users;
    Catalog* reservations;
    Catalog* passengers;
    Catalog* flights;

    // TODO: Kept for backwards compatibility with the current query implementation. 
    // Should be removed aftwerwards.
    Catalog** _catalog_arr; 
} DATASET_DATA, *DatasetData;

// Forward declarations, for ease of reading.
Catalog** make_catalogues(char* dataset_dir);

// ============== DATASET STORE ==============
DatasetData make_dataset_data(char* input) {
    DatasetData dd = (DatasetData)malloc(sizeof(DATASET_DATA));
    dd->datasets_loaded = FALSE;
    dd->users = NULL;
    dd->reservations = NULL;
    dd->passengers = NULL;
    dd->flights = NULL;

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
    dd->users        = dd->_catalog_arr[0];
    dd->flights      = dd->_catalog_arr[1];
    dd->passengers   = dd->_catalog_arr[2];
    dd->reservations = dd->_catalog_arr[3];

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

Catalog** dataset_data_get_catalog_array(DatasetData dd) {
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
        free(dd->_catalog_arr);
    }

    free(dd);
}

// ============== PARSE DATASETS ==============
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

Catalog** make_catalogues(char* dataset_dir) {
    // ------- User Catalogue -------
    Catalog* user_catalog = catalog_init(g_str_hash, g_str_equal, free);
    char* userdata_path = join_paths(2, dataset_dir, "users.csv");
    parse_file(
        userdata_path,     
        &tokenize_csv,
        &_default_catalog_preprocessor,
        &verify_user_tokens, 
        &parse_user, 
        &usersCatalog_write_to_catalog, 
        &discard_user,
        &default_csv_destructor,
        user_catalog
    );  
    free(userdata_path);

    // ------- Flight Catalogue -------
    Catalog* flight_catalog = catalog_init(g_direct_hash, g_direct_equal, NULL);
    char* flightsdata_path = join_paths(2, dataset_dir, "flights.csv");
    parse_file(
        flightsdata_path,
        &tokenize_csv,
        &_default_catalog_preprocessor,
        &verify_flight_tokens, 
        &parse_flight, 
        &flightsCatalog_write_to_catalog, 
        &discard_flight,
        &default_csv_destructor,
        flight_catalog
    );
    free(flightsdata_path);

    // ------- Passengers Catalogue -------
    Catalog* passengers_catalog = catalog_init(NULL, NULL, NULL);
    char* passengersdata_path = join_paths(2, dataset_dir, "passengers.csv");
    parse_file(
        passengersdata_path,
        &tokenize_csv,
        &_passenger_preprocessor,
        &verify_passenger_tokens,
        &parse_passenger, 
        &passengersCatalog_write_to_catalog, 
        &discard_passenger,
        &csv_destructor_extended,
        user_catalog,
        flight_catalog,
        passengers_catalog
    );
    free(passengersdata_path);

    // ------- Reservation Catalogue -------
    Catalog* reservation_catalog = catalog_init(g_direct_hash, g_direct_equal, NULL);
    char* reservationsdata_path = join_paths(2, dataset_dir, "reservations.csv");
    parse_file(
        reservationsdata_path,
        &tokenize_csv,
        &_reservation_preprocessor,
        &verify_reservation_tokens, 
        &parse_reservation, 
        &reservationsCatalog_write_to_catalog, 
        &discard_reservation,
        &csv_destructor_extended,
        user_catalog,
        reservation_catalog
    );
    free(reservationsdata_path);
    
    catalog_sort(user_catalog, (GCompareFunc)&usersCatalog_full_compare_func);
    catalog_sort(flight_catalog, (GCompareFunc)&flightsCatalog_full_compare_func);
    catalog_sort(passengers_catalog, (GCompareFunc)&passengersCatalog_full_compare_func);
    catalog_sort(reservation_catalog, (GCompareFunc)&reservationsCatalog_full_compare_func);

    Catalog** catalogues = (Catalog**)malloc(4 * sizeof(Catalog*));
    catalogues[0] = user_catalog;
    catalogues[1] = flight_catalog;
    catalogues[2] = passengers_catalog;
    catalogues[3] = reservation_catalog;

    return catalogues;
}