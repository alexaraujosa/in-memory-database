#include "catalog/catalogManager.h"
#include "collections/passenger.h"

gint passengersCatalog_full_compare_func(gconstpointer a, gconstpointer b){
    const Passenger passenger1 = *(const Passenger*)a;
    const Passenger passenger2 = *(const Passenger*)b;

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

gint passenger_flightID_compare_func(gconstpointer a, gconstpointer b){
    const Passenger *passenger1 = (const Passenger*)a;
    const int flightID2 = (const int *)b;

    short int flightID1 = get_passenger_flightID(*passenger1);

    if (flightID1 < flightID2) return -1;
    if (flightID1 > flightID2) return 1;
    return 0;
}

//FIXME THis fucntion is not well implemented
void passengersCatalog_print_array(gpointer data, gpointer passenger_data) {
    const Passenger passenger = (Passenger)passenger_data;
    int flightID = get_passenger_flightID(passenger);
    const char* userID = get_passenger_userdID(passenger);
    g_print("flightID: %d; userID: %s\n", flightID, userID);
    free(userID);
}

void passengersCatalog_write_to_catalog(Passenger passenger, ParserStore store) {
    Catalog* passenger_catalog = g_array_index(store, Catalog*, 4);
    catalog_add_int_to_catalog(passenger_catalog, NULL, passenger);
}