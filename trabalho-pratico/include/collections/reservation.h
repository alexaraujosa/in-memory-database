#ifndef TP_COLLECTIONS_RESERVATION_H
#define TP_COLLECTIONS_RESERVATION_H

#include "common.h"
#include "util/error.h"
#include "util/misc.h"
#include "util/string.h"
#include "util/date.h"
#include "user.h"

#define HOTEL_ID_OFFSET 3
#define MAX_HOTEL_NAME_LEN 30

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

int get_reservation_id(const Reservation reservation);
void set_reservation_id(Reservation reservation, int id);
char *get_reservation_userID(const Reservation reservation);
void set_reservation_userID(Reservation reservation, const char *user_id);
short int get_reservation_hotelID(const Reservation reservation);
void set_reservation_hotelID(Reservation reservation, short int hotel_id);
char *get_reservation_hotel_name(const Reservation resarvation);
void set_reservation_hotel_name(Reservation reservation, const char *hotel_name);
unsigned int get_reservation_hotel_stars(const Reservation reservation);
void set_reservation_hotel_stars(Reservation reservation, unsigned int hotel_stars);
unsigned int get_reservation_city_tax(const Reservation reservation);
void set_reservation_city_tax(Reservation reservation, unsigned int city_tax);
int get_reservation_begin_date(const Reservation reservation);
void set_reservation_begin_date(Reservation reservation, int begin_date);
int get_reservation_end_date(const Reservation reservation);
void set_reservation_end_data(Reservation reservation, int end_date);
unsigned int get_reservation_price_per_night(const Reservation reservation);
void set_reservation_price_per_night(Reservation reservation, unsigned int price_per_night);
bool get_reservation_includes_breakfast(const Reservation reservation);
void set_reservation_includes_breakfast(Reservation reservation, bool includes_breakfast);
int get_reservation_rating(const Reservation reservation);
void set_reservation_rating(Reservation reservation, int rating);

/**
 * @brief Creates a reservation, transforming data.
 * 
 * @param tokens Valid tokens previously verified.
 * 
 * @return Pointer to the Reservation struct.
*/
void* parse_reservation(Tokens tokens);

/**
 * @brief Default pre processor for the reservation catalog.
 * 
 * @param stream File to be stored.
 * @param store Store that connects the outside of the function with the inside.
 * @param args Variadic arguments.
 * 
 * @return void.
*/
void preprocessor_reservation(FILE* stream, ParserStore store, va_list args);

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