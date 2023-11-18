#include "catalog/catalogManager.h"
#include "collections/passenger.h"

gint passengersCatalog_full_compare_func(gconstpointer a, gconstpointer b){
    const Passenger passenger1 = *(const Passenger*)a;
    const Passenger passenger2 = *(const Passenger*)b;

    int id1 = get_passenger_flightID(passenger1);
    int id2 = get_passenger_flightID(passenger2);

    if(id1 < id2) return -1;
    if(id1 > id2) return 1;

    char *userId1 = get_passenger_userdID(passenger1);
    char *userId2 = get_passenger_userdID(passenger2);

    int name_comparison = strcasecmp(userId1, userId2);
    free(userId1);
    free(userId2);
    return name_comparison;
}

gint passenger_flightID_compare_func(gconstpointer a, gconstpointer b){
    const Passenger *passenger1 = (const Passenger*)a;
    const int flightID2 = GPOINTER_TO_INT(b);

    short int flightID1 = get_passenger_flightID(*passenger1);

    if (flightID1 < flightID2) return -1;
    if (flightID1 > flightID2) return 1;
    return 0;
}

void passengersCatalog_write_to_catalog(Passenger passenger, ParserStore store) {
    Catalog* passenger_catalog = g_array_index(store, Catalog*, 4);
    catalog_add_int_to_catalog(passenger_catalog, NULL, passenger);
}