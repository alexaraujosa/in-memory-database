#include "catalog/flightsCatalog.h"

gint flightsCatalog_full_compare_func(gconstpointer flight_A, gconstpointer flight_B) {
    const Flight flight1 = *(const Flight*)flight_A;
    const Flight flight2 = *(const Flight*)flight_B;

    char* origin1 = get_flight_origin(flight1);
    char* origin2 = get_flight_origin(flight2);

    int origin_comparison = strcasecmp(origin1, origin2);
    if (origin_comparison != 0) {
        free(origin1);
        free(origin2);
        return origin_comparison;
    }
    free(origin1);
    free(origin2);

    int schedule_departure_date1 = get_flight_schedule_departure_date(flight1);
    int schedule_departure_date2 = get_flight_schedule_departure_date(flight2);
    int schedule_arrival_date1 = get_flight_schedule_arrival_date(flight1);
    int schedule_arrival_date2 = get_flight_schedule_arrival_date(flight2);
    int id1 = get_flight_id(flight1);
    int id2 = get_flight_id(flight2);

    if (schedule_departure_date1 < schedule_departure_date2) return 1;
    if (schedule_departure_date1 > schedule_departure_date2) return -1;

    if (schedule_arrival_date1 < schedule_arrival_date2) return -1;
    if (schedule_arrival_date1 > schedule_arrival_date2) return 1;

    if (id1 < id2) return -1;
    if (id1 > id2) return 1;

    return 0;
}

gint flightsCatalog_origin_compare_func(gconstpointer flight_A, gconstpointer flight_origin) {
    const Flight* flight1 = (const Flight*)flight_A;
    char* flight_origin2 = (char*)flight_origin;

    char* flight_origin1 = get_flight_origin(*flight1);

    int origin_comparison = strcasecmp(flight_origin1, flight_origin2);

    free(flight_origin1);

    return origin_comparison;
}

void flightsCatalog_write_to_catalog(void* _flight, ParserStore store) {
    Flight flight = (Flight)_flight;

    int id = get_flight_id(flight);
    Catalog* flight_catalog = g_array_index(store, Catalog*, 2);

    GArray* generic_catalog = g_array_index(store, GArray*, 3);
    int flight_year = get_year(get_flight_schedule_departure_date(flight));
    int flight_month = get_month(get_flight_schedule_departure_date(flight));
    int flight_day = get_day(get_flight_schedule_departure_date(flight));

    genCat_add(flight_year, generic_catalog);
    increment_flight_conteudo(flight_year, flight_month, flight_day, generic_catalog);

    catalog_add_int_to_catalog(flight_catalog, GINT_TO_POINTER(id), flight);
}

void flightsCatalog_print_flights_key_value(void* key, void* value) {
    int key_int = GPOINTER_TO_INT(key);
    printf("Key:(%d)\t", key_int);
    print_flight(value);
}