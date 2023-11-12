#include "catalog/catalogManager.h"
#include "collections/reservation.h"

//BUG A ordem não esta bem feita... possivelmente pelo unint8_t
//BUG Para guardar na hasTable é necessario converter key de int para string
gint reservation_tree_compare_func(gconstpointer a, gconstpointer b, gpointer reservation_data) {
    const RESERVATION* reservation1 = (const RESERVATION*)a;
    const RESERVATION* reservation2 = (const RESERVATION*)b;

    if (reservation1->hotel_id < reservation2->hotel_id) return -1;
    if (reservation1->hotel_id > reservation2->hotel_id) return 1;
    if (reservation1->begin_date < reservation2->begin_date) return -1;
    if (reservation1->begin_date > reservation2->begin_date) return 1;
    if (reservation1->id < reservation2->id) return -1;
    if (reservation1->id > reservation2->id) return 1;
    return 0;
}

void reservation_print_tree(gpointer data, gpointer reservation_data) {
    const RESERVATION* reservation = (const RESERVATION*)reservation_data;
    g_print("hotelId: %d; begin_Date: %d\n", reservation->hotel_id, reservation->begin_date);
}

void write_reservation(RESERVATION* reservation, Catalog* reservation_catalog) {
    catalog_add_to_catalog(reservation_catalog, reservation->id, reservation, reservation);
}
