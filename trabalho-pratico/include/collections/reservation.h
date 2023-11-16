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

/**
 * @brief Verifies if tokens are valid to create a reservation.
 * 
 * @param tokens A pointer to the struct used for verifying the data.
 * 
 * @return 1 if all the tokens are valid. Otherwise, 0.
*/
int verify_reservation_tokens(Tokens tokens, ParserStore store);

/**
 * @brief Creates a reservation using dynamic memory.
 * 
 * @param id Reservation id.
 * @param user_id User id.
 * @param hotel_id Hotel id.
 * @param hotel_name Hotel name.
 * @param hotel_stars Hotel stars.
 * @param city_tax City tax.
 * @param begin_date Offset from system start date, relative to the reservation begin date.
 * @param end_date Offset from system start date, relative to the reservation end date.
 * @param price_per_night Price per night.
 * @param includes_breakfast Includes breakfast.
 * @param rating Rating.
 * 
 * @return Pointer to the Reservation struct.
*/
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

/**
 * @brief Creates a reservation, transforming data.
 * 
 * @param tokens Valid tokens previously verified.
 * 
 * @return Pointer to the Reservation struct.
*/
Reservation parse_reservation(Tokens tokens);

/**
 * @brief Creates the reservation_errors file and writes the lines with invalid data.
 * 
 * @param raw_data Invalid data contained in the line.
 * @param store Connection with the outside part of the parser, used to store the discard file address.
 * 
 * @return void.
*/
void discard_reservation(void* raw_data, ParserStore store);

/**
 * @brief Prints a reservation (Mainly usage for debug).
 * 
 * @param reservation Pointer to the reservation struct.
 * 
 * @return void.
*/
void print_reservation(void* reservation);
#endif