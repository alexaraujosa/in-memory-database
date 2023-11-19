#include "catalog/catalogManager.h"
#include "collections/flight.h"


gint flightsCatalog_full_compare_func(gconstpointer a, gconstpointer b) {
    const Flight flight1 = *(const Flight*)a;
    const Flight flight2 = *(const Flight*)b;

    char *origin1 = get_flight_origin(flight1);
    char *origin2 = get_flight_origin(flight2);

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

gint flight_origin_compare_func(gconstpointer a, gconstpointer b) {
    const Flight *user1 = (const Flight*)a;
    char* user_NAME2 = (char*) b;

    char* user_NAME1 = get_flight_origin(*user1);
    if(strcasecmp(user_NAME1, user_NAME2) > 0) {
        free(user_NAME1);
        return 1;
    }

    if(strcasecmp(user_NAME1, user_NAME2) < 0) {
        free(user_NAME1);
        return -1;
    }

    free(user_NAME1);
    return 0;
}

void flightsCatalog_write_to_catalog(void* _flight, ParserStore store) {
    Flight flight = (Flight)_flight;

    int id = get_flight_id(flight);
    Catalog *flight_catalog = g_array_index(store, Catalog*, 2);
    catalog_add_int_to_catalog(flight_catalog, GINT_TO_POINTER(id), flight);
}

void flightsCatalog_print_flights_key_value(void* key, void* value) {
    int key_int = GPOINTER_TO_INT(key);
    printf("Key:(%d)\t", key_int);
    print_flight(value);
}