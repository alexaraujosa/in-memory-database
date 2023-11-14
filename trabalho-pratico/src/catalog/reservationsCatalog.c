#include "catalog/catalogManager.h"
#include "collections/reservation.h"


gint reservation_tree_compare_func(gconstpointer a, gconstpointer b, gpointer reservation_data) {
    const RESERVATION *reservation1 = (const RESERVATION*)a;
    const RESERVATION *reservation2 = (const RESERVATION*)b;

    uint8_t hotel_id1 = get_reservation_hotelID(reservation1);
    uint8_t hotel_id2 = get_reservation_hotelID(reservation2);
    int begin_date1 = get_reservation_begin_date(reservation1);
    int begin_date2 = get_reservation_begin_date(reservation2);
    int id1 = get_reservation_id(reservation1);
    int id2 = get_reservation_id(reservation1);

    if (hotel_id1 < hotel_id2) return -1;
    if (hotel_id1 > hotel_id2) return 1;
    if (begin_date1 < begin_date2) return -1;
    if (begin_date1 > begin_date2) return 1;
    if (id1 < id2) return -1;
    if (id1 > id2) return 1;
    return 0;
}

void reservation_print_tree(gpointer data, gpointer reservation_data) {
    const RESERVATION *reservation = (const RESERVATION*)reservation_data;
    int id = get_reservation_id(reservation);
    uint8_t hotel_id = get_reservation_hotelID(reservation);
    g_print("hotelId: %d; begin_Date: %d\n", id, hotel_id);
}

// void write_reservation(RESERVATION *reservation, Catalog *reservation_catalog) {
//     int id = get_reservation_id(reservation);
//     catalog_add_to_catalog(reservation_catalog, id, reservation, reservation);
// }
