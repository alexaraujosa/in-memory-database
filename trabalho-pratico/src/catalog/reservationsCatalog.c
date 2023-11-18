#include "catalog/catalogManager.h"
#include "collections/reservation.h"


gint reservationsCatalog_full_compare_func(gconstpointer* a, gconstpointer* b) {
    const Reservation reservation1 = *(const Reservation*)a;
    const Reservation reservation2 = *(const Reservation*)b;

    short int hotel_id1 = get_reservation_hotelID(reservation1);
    short int hotel_id2 = get_reservation_hotelID(reservation2);
    int begin_date1 = get_reservation_begin_date(reservation1);
    int begin_date2 = get_reservation_begin_date(reservation2);
    int id1 = get_reservation_id(reservation1);
    int id2 = get_reservation_id(reservation2);
    
    if (hotel_id1 < hotel_id2) return -1;
    if (hotel_id1 > hotel_id2) return 1;
    if (begin_date1 < begin_date2) return -1;
    if (begin_date1 > begin_date2) return 1;
    if (id1 < id2) return -1;
    if (id1 > id2) return 1;
    return 0;
}

gint reservation_hotelID_compare_func(gconstpointer a, gconstpointer b) {
    const Reservation *reservation1 = (const Reservation*)a;
    int hotel_id2_i = GPOINTER_TO_INT(b);
    
    short int hotel_id2 = (short int)hotel_id2_i;
    short int hotel_id1 = get_reservation_hotelID(*reservation1);

    if (hotel_id1 < hotel_id2) return -1;
    if (hotel_id1 > hotel_id2) return 1;
    return 0;
}

gint reservation_date_compare_func(gconstpointer a, gconstpointer b) {
    const Reservation reservation1 = *(const Reservation*)a;
    const Reservation reservation2 = *(const Reservation*)b;
    
    int start_date1 = get_reservation_begin_date(reservation1);
    int start_date2 = get_reservation_begin_date(reservation2);
    int id1 = get_reservation_id(reservation1);
    int id2 = get_reservation_id(reservation2);
    
    if (start_date1 < start_date2) return 1;
    if (start_date1 > start_date2) return -1;


    if (id1 < id2) return -1;
    if (id1 > id2) return 1;

    return 0;
}

void reservationsCatalog_write_to_catalog(RESERVATION *reservation, ParserStore store) {
    int id = get_reservation_id(reservation);
    Catalog* reservation_catalog = g_array_index(store, Catalog*, 3);
    catalog_add_int_to_catalog(reservation_catalog, GINT_TO_POINTER(id), reservation);
}

void reservationsCatalog_print_reservation_key_value(void* key, void* value) {
    int *key_int = (int *)key;
    printf("Key:(%d)\t", *key_int);
    print_reservation(value);
}

