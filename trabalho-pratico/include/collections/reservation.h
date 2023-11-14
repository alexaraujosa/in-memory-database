#ifndef TP_COLLECTIONS_RESERVATION_H
#define TP_COLLECTIONS_RESERVATION_H

#include "common.h"
#include "util/error.h"
#include "util/misc.h"
#include "util/string.h"
#include "user.h"

#define HOTEL_ID_OFFSET 3
#define MAX_HOTEL_NAME_LEN 60

#define RESERVATION_ID_OFFSET 4
#define RESERVATION_HOTEL_STARS_BF 3
#define RESERVATION_CITY_TAX_BF 3
#define RESERVATION_PRICE_PER_NIGHT_BF 15
#define RESERVATION_RATING_BF 5

typedef struct reservation RESERVATION, *Reservation;

int verify_reservation_tokens(Tokens tokens, ParserStore store);

Reservation make_reservation(
    int id, 
    UserId(user_id), 
    short int hotel_id, 
    char hotel_name[MAX_HOTEL_NAME_LEN], 
    unsigned int hotel_stars, 
    unsigned int city_tax, 
    int begin_date, 
    int end_date, 
    unsigned int price_per_night, 
    bool includes_breakfast, 
    int rating
);

int get_reservation_id(Reservation reservation);
void set_reservation_id(Reservation reservation, int id);
const char *get_reservation_userID(Reservation reservation);
void set_reservation_userID(Reservation reservation, const char *user_id);
short int get_reservation_hotelID(Reservation reservation);
void set_reservation_hotelID(Reservation reservation, short int hotel_id);
const char *get_reservation_hotel_name(Reservation resarvation);
void set_reservation_hotel_name(Reservation reservation, const char *hotel_name);
unsigned int get_reservation_hotel_stars(Reservation reservation);
void set_reservation_hotel_stars(Reservation reservation, unsigned int hotel_stars);
unsigned int get_reservation_city_tax(Reservation reservation);
void set_reservation_city_tax(Reservation reservation, unsigned int city_tax);
int get_reservation_begin_date(Reservation reservation);
void set_reservation_begin_date(Reservation reservation, int begin_date);
int get_reservation_end_date(Reservation reservation);
void set_reservation_end_data(Reservation reservation, int end_date);
unsigned int get_reservation_price_per_night(Reservation reservation);
void set_reservation_price_per_night(Reservation reservation, unsigned int price_per_night);
bool get_reservation_includes_breakfast(Reservation reservation);
void set_reservation_includes_breakfast(Reservation reservation, bool includes_breakfast);
int get_reservation_rating(Reservation reservation);
void set_reservation_rating(Reservation reservation, int rating);

Reservation parse_reservation(Tokens tokens);
void discard_reservation(void* raw_data, ParserStore store);
RESERVATION parseReservationFromLine(char* line, int len);

void print_reservation(void* reservation);
#endif