#include "catalog/catalogManager.h"
#include "collections/flight.h"

//BUG Não guarda a origin como deve de ser
//BUG Para guardar na hasTable é necessario converter key de int para string
gint flight_tree_compare_func(gconstpointer a, gconstpointer b, gpointer flight_data) {
    const FLIGHT* flight1 = (const FLIGHT*)a;
    const FLIGHT* flight2 = (const FLIGHT*)b;

    int origin_comparison = strcasecmp(flight1->origin, flight2->origin);
    if (origin_comparison != 0) {
        return origin_comparison;
    }

    if (flight1->schedule_departure_date < flight2->schedule_departure_date) return -1;
    if (flight1->schedule_departure_date > flight2->schedule_departure_date) return 1;

    if (flight1->schedule_arrival_date < flight2->schedule_arrival_date) return -1;
    if (flight1->schedule_arrival_date > flight2->schedule_arrival_date) return 1;

    if (flight1->id < flight2->id) return -1;
    if (flight1->id > flight2->id) return 1;

    return 0;
}

void flight_print_tree(gpointer data, gpointer flight_data) {
    const FLIGHT* flight = (const FLIGHT*)flight_data;
    g_print("flight_origin: %s; schedule_departure_date: %d\n", flight->origin, flight->schedule_departure_date);
}

void write_flight(FLIGHT* fligh, Catalog* fligh_catalog) {
    catalog_add_to_catalog(fligh_catalog, fligh->id, fligh, fligh);
}
