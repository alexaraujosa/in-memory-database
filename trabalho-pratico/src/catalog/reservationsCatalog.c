#include "catalog/catalogManager.h"
#include "collections/reservation.h"

gint reservationTreeCompareFunc(gconstpointer a, gconstpointer b, gpointer user_data) {
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

void writeReservation(RESERVATION* reservation, Catalog* reservationCatalog) {
    catalog_add_to_catalog(reservationCatalog, reservation->id, reservation, reservation);
}

void printReservation(void* pt) {
    RESERVATION* reservation = (RESERVATION*)pt;
    char breakfast_status[10] = "No";

    if (reservation->includes_breakfast) {
        strcpy(breakfast_status, "Yes");
    }

    printf(
        "{Id:%d; "
        "UserId:%d; "
        "HotelId:%d; "
        "HotelName:%s; "
        "HotelStars:%u; "
        "CityTax:%u; "
        "BeginDate:%d; "
        "EndDate:%d; "
        "PricePerNight:%u; "
        "IncludesBreakfast:%s; "
        "Rating:%d}\n",
        reservation->id, reservation->user_id, reservation->hotel_id,
        reservation->hotel_name, reservation->hotel_stars, reservation->city_tax,
        reservation->begin_date, reservation->end_date, reservation->price_per_night,
        breakfast_status, reservation->rating);
}