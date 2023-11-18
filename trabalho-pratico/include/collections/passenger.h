#ifndef TP_COLLECTIONS_PASSENGER_H
#define TP_COLLECTIONS_PASSENGER_H

#include "debug.h"
#include "user.h"
#include "parser/parser.h"

typedef struct passenger PASSENGER, *Passenger;

/**
 * @brief Verifies if tokens are valid to create a passenger.
 * 
 * @param tokens A pointer to the struct used for verifying the data.
 * 
 * @return 1 if all the tokens are valid. Otherwise, 0.
*/
int verify_passenger_tokens(Tokens tokens, ParserStore store);

/**
 * @brief Creates a passenger using dynamic memory.
 * 
 * @param flight_id Flight id.
 * @param user_id User id.
 * 
 * @return Pointer to the Passengconst er struct.
*/
Passenger make_passenger(
    int flight_id,
    UserId(user_id)
);

int get_passenger_flightID(const Passenger passenger);
void set_passenger_flightID(Passenger passenger, int flight_id);
char *get_passenger_userdID(const Passenger passenger);
void set_passenger_userID(Passenger passenger, const char *user_id);

/**
 * @brief Creates a passenger, transforming data.
 * 
 * @param tokens Valid tokens previously verified.
 * 
 * @return Pointer to the Passenger struct.
*/
Passenger parse_passenger(Tokens tokens);

/**
 * @brief Creates the passengers_errors file and writes the lines with invalid data.
 * 
 * @param raw_data Invalid data contained in the line.
 * @param store Connection with the outside part of the parser, used to store the discard file address.
 * 
 * @return void.
*/
void discard_passenger(void* raw_data, ParserStore store);

void print_passenger(void* pt);

#endif