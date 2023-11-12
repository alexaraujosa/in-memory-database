#include "collections/reservation.h"

RESERVATION makeReservation(
    int id,
    UserId(user_id),
    uint8_t hotel_id,
    char hotel_name[MAX_HOTEL_NAME_LEN],
    unsigned int hotel_stars,
    unsigned int city_tax,
    int begin_date,
    int end_date,
    unsigned int price_per_night,
    bool includes_breakfast,
    int rating) {
    rt_assert(hotel_stars > 0 && hotel_stars <= 5, "Hotel Stars must be an integer between 1 and 5.");
    rt_assert(
        city_tax > 0 && city_tax <= MAX_BIT_VALUE(RESERVATION_CITY_TAX_BF),
        isnprintf("City tax must be an integer between 1 and %d", MAX_BIT_VALUE(RESERVATION_CITY_TAX_BF)));
    // rt_assert(hotel_id > -1 && hotel_id <= 9999, "Hotel Id must be a 4-digit integer.");

    RESERVATION reservation = {
        .id = id,
        // .user_id = user_id,
        .hotel_id = hotel_id,
        // .hotel_name = hotel_name,
        .hotel_stars = hotel_stars,
        .city_tax = city_tax,
        .begin_date = begin_date,
        .end_date = end_date,
        .price_per_night = price_per_night,
        .includes_breakfast = includes_breakfast,
        .rating = rating};

    strncpy(reservation.user_id, user_id, MAX_USER_ID_LEN);
    strncpy(reservation.hotel_name, hotel_name, MAX_HOTEL_NAME_LEN);

    return reservation;
}

RESERVATION parseReservationFromLine(char* line, int len) {
    IGNORE_ARG(line);
    IGNORE_ARG(len);
    // TODO: Parse Reservation from CSV line
}

void print_reservation(void* pt) {
    RESERVATION* reservation = (RESERVATION*)pt;
    char breakfast_status[10] = "No";

    if (reservation->includes_breakfast) {
        strcpy(breakfast_status, "Yes");
    }

    printf(
        "{Id:%d; "
        "UserId:%s; "
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