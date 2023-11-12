#ifndef TP_COLLECTIONS_RESERVATION_H
#define TP_COLLECTIONS_RESERVATION_H

#include "common.h"
#include "util/error.h"
#include "util/misc.h"
#include "util/string.h"
#include "user.h"

#define MAX_HOTEL_NAME_LEN 60

#define RESERVATION_HOTEL_STARS_BF 3
#define RESERVATION_CITY_TAX_BF 3
#define RESERVATION_PRICE_PER_NIGHT_BF 15
#define RESERVATION_RATING_BF 5

typedef struct reservation {
    int id;
    UserId(user_id); // Relates to User
    uint8_t hotel_id; // No relation, symbolic
    char hotel_name[MAX_HOTEL_NAME_LEN];
    unsigned int hotel_stars:RESERVATION_HOTEL_STARS_BF;
    unsigned int city_tax:RESERVATION_CITY_TAX_BF;
    int begin_date; // Offset from Base Date
    int end_date; // Offset from Base Date
    unsigned int price_per_night:RESERVATION_PRICE_PER_NIGHT_BF;
    bool includes_breakfast;
    int rating:RESERVATION_RATING_BF; // Depends on the scale (0-5, 0-10, etc), but can possibly be bitfielded.
} RESERVATION, *Reservation;

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
    int rating
);

RESERVATION parseReservationFromLine(char* line, int len);

void print_reservation(void* reservation);
#endif