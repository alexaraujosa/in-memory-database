#include "catalog/reservationsCatalog.h"

gint reservationsCatalog_full_compare_func(gconstpointer* reservation_A, gconstpointer* reservation_B) {
    const Reservation reservation1 = *(const Reservation*)reservation_A;
    const Reservation reservation2 = *(const Reservation*)reservation_B;

    short int hotel_id1 = get_reservation_hotelID(reservation1);
    short int hotel_id2 = get_reservation_hotelID(reservation2);
    if (hotel_id1 < hotel_id2) return -1;
    if (hotel_id1 > hotel_id2) return 1;

    int begin_date1 = get_reservation_begin_date(reservation1);
    int begin_date2 = get_reservation_begin_date(reservation2);
    if (begin_date1 < begin_date2) return -1;
    if (begin_date1 > begin_date2) return 1;

    int id1 = get_reservation_id(reservation1);
    int id2 = get_reservation_id(reservation2);
    if (id1 < id2) return -1;
    if (id1 > id2) return 1;

    return 0;
}

gint reservationsCatalog_hotelID_compare_func(gconstpointer reservation_A, gconstpointer reservation_hotelID) {
    const Reservation* reservation1 = (const Reservation*)reservation_A;
    int hotel_id2_i = GPOINTER_TO_INT(reservation_hotelID);

    short int hotel_id2 = (short int)hotel_id2_i;
    short int hotel_id1 = get_reservation_hotelID(*reservation1);

    if (hotel_id1 < hotel_id2) return -1;
    if (hotel_id1 > hotel_id2) return 1;
    return 0;
}

gint reservationsCatalog_date_compare_func(gconstpointer reservation_A, gconstpointer reservation_B) {
    const Reservation reservation1 = *(const Reservation*)reservation_A;
    const Reservation reservation2 = *(const Reservation*)reservation_B;

    int start_date1 = get_reservation_begin_date(reservation1);
    int start_date2 = get_reservation_begin_date(reservation2);
    if (start_date1 < start_date2) return 1;
    if (start_date1 > start_date2) return -1;

    int id1 = get_reservation_id(reservation1);
    int id2 = get_reservation_id(reservation2);
    if (id1 < id2) return -1;
    if (id1 > id2) return 1;

    return 0;
}

void reservationsCatalog_write_to_catalog(void* _reservation, ParserStore store) {
    RESERVATION* reservation = (RESERVATION*)_reservation;

    int id = get_reservation_id(reservation);
    Catalog* reservation_catalog = g_array_index(store, Catalog*, 3);
    Catalog* user_catalog = g_array_index(store, Catalog*, 2);
    User user = catalog_search_in_str_hashtable(user_catalog, get_reservation_userID(reservation));


    add_user_total_spend(user, calculate_reservation_total_price(reservation));

    catalog_add_int_to_catalog(reservation_catalog, GINT_TO_POINTER(id), reservation);
}

void reservationsCatalog_print_reservation_key_value(void* key, void* value) {
    int* key_int = (int*)key;
    printf("Key:(%d)\t", *key_int);
    print_reservation(value);
}
