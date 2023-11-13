#include "catalog/catalogManager.h"
#include "collections/flight.h"

//BUG Aparece "CityCity" na origem
gint flight_tree_compare_func(gconstpointer a, gconstpointer b, gpointer flight_data) {
    const FLIGHT *flight1 = (const FLIGHT*)a;
    const FLIGHT *flight2 = (const FLIGHT*)b;

    const char *origin1 = get_flight_origin(flight1);
    const char *origin2 = get_flight_origin(flight2);

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
    
    if (schedule_departure_date1 < schedule_departure_date2) return -1;
    if (schedule_departure_date1 > schedule_departure_date2) return 1;

    if (schedule_arrival_date1 < schedule_arrival_date2) return -1;
    if (schedule_arrival_date1 > schedule_arrival_date2) return 1;

    if (id1 < id2) return -1;
    if (id1 > id2) return 1;

    return 0;
}

void flight_print_tree(gpointer data, gpointer flight_data) {
    const FLIGHT* flight = (const FLIGHT*)flight_data;
    const char *origin = get_flight_origin(flight);
    int schedule_departure_date = get_flight_schedule_departure_date(flight);
    g_print("flight_origin: %s; schedule_departure_date: %d\n", origin, schedule_departure_date);
    free(origin);
}

void write_flight(FLIGHT* flight, Catalog* flight_catalog) {
    int id = get_flight_origin(flight);
    catalog_add_to_catalog(flight_catalog, id, flight, flight);
}
