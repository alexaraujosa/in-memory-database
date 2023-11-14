#include "catalog/catalogManager.h"
#include "collections/passenger.h"

gint passenger_tree_compare_func(gconstpointer a, gconstpointer b){
    Passenger passenger1 = (Passenger)a;
    Passenger passenger2 = (Passenger)b;

    int id1 = get_passenger_flightID(passenger1);
    int id2 = get_passenger_flightID(passenger2);

    if(id1 < id2) return -1;
    if(id1 > id2) return 1;

    const char *userId1 = get_passenger_userdID(passenger1);
    const char *userId2 = get_passenger_userdID(passenger2);

    int name_comparison = strcasecmp(userId1, userId2);
    free(userId1);
    free(userId2);
    return name_comparison;
}

void passenger_print_tree(gpointer data, gpointer passenger_data) {
    const Passenger passenger = (Passenger)passenger_data;
    int flightID = get_passenger_flightID(passenger);
    const char* userID = get_passenger_userdID(passenger);
    g_print("flightID: %d; userID: %s\n", flightID, userID);
    free(userID);
}

void write_passenger(Passenger passenger, ParserStore store) {
    Catalog* passenger_catalog = g_array_index(store, Catalog*, 2);
    catalog_add_int_to_catalog(passenger_catalog, NULL, passenger, passenger);
}