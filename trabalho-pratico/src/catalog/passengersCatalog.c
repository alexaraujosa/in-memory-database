#include "catalog/passengersCatalog.h"

gint passengersCatalog_full_compare_func(gconstpointer passenger_A, gconstpointer passenger_B) {
    const Passenger passenger1 = *(const Passenger*)passenger_A;
    const Passenger passenger2 = *(const Passenger*)passenger_B;

    char* userId1 = get_passenger_userID(passenger1);
    char* userId2 = get_passenger_userID(passenger2);

    int name_comparison = strcmp(userId1, userId2);
    free(userId1);
    free(userId2);

    if (name_comparison != 0) {
        return name_comparison;
    }

    int id1 = get_passenger_flightID(passenger1);
    int id2 = get_passenger_flightID(passenger2);

    if (id1 < id2) return -1;
    if (id1 > id2) return 1;
    return 0;
}

gint passengersCatalog_flightID_compare_func(gconstpointer passenger_A, gconstpointer passenger_flightID) {
    const Passenger* passenger1 = (const Passenger*)passenger_A;
    const int flightID2 = GPOINTER_TO_INT(passenger_flightID);

    int flightID1 = get_passenger_flightID(*passenger1);

    if (flightID1 < flightID2) return -1;
    if (flightID1 > flightID2) return 1;
    return 0;
}

gint passengersCatalog_userID_compare_func(gconstpointer passenger_A, gconstpointer passenger_userID) {
    const Passenger* passenger = (const Passenger*)passenger_A;
    char* userID2 = (char*)passenger_userID;

    char* userID1 = get_passenger_userID(*passenger);
    int result = strcmp(userID1, userID2);

    free(userID1);
    return result;
}

void passengersCatalog_write_to_catalog(void* _passenger, ParserStore store) {
    Passenger passenger = (Passenger)_passenger;

    Catalog* user_catalog = g_array_index(store, Catalog*, 2);
    Catalog* flight_catalog = g_array_index(store, Catalog*, 3);
    Catalog* passenger_catalog = g_array_index(store, Catalog*, 4);

    char* user_id = get_passenger_userID(passenger);
    User user = catalog_search_in_str_hashtable(user_catalog, user_id);

    GArray* user_flights = get_user_flights(user);
    int flight_id = get_passenger_flightID(passenger);
    Flight flight = catalog_search_in_int_hashtable(flight_catalog, flight_id);
    g_array_append_val(user_flights, flight);

    GArray* generic_catalog = g_array_index(store, GArray*, 5);
    int passenger_year = get_year(get_flight_schedule_departure_date(flight));
    int passenger_month = get_month(get_flight_schedule_departure_date(flight));
    int passenger_day = get_day(get_flight_schedule_departure_date(flight));

    genCat_add(passenger_year, generic_catalog);
    increment_passenger_conteudo(passenger_year, passenger_month, passenger_day, user_id, generic_catalog);
    free(user_id);
    catalog_add_int_to_catalog(passenger_catalog, NULL, passenger);
}