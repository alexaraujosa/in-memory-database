#ifndef TP_COLLECTIONS_PASSENGER_H
#define TP_COLLECTIONS_PASSENGER_H

#include "debug.h"
#include "user.h"
#include "flight.h"
#include "parser/parser.h"
#include "time.h"

typedef struct passenger PASSENGER, *Passenger;

/**
 * @brief Verifies if tokens are valid to create a passenger.
 * 
 * @param tokens A pointer to the struct used for verifying the data.
 * 
 * @return 1 if all the tokens are valid. Otherwise, 0.
*/
int verify_passenger_tokens(Tokens tokens, ParserStore store);

int get_passenger_flightID(const Passenger passenger);
void set_passenger_flightID(Passenger passenger, int flight_id);
char *get_passenger_userID(const Passenger passenger);
void set_passenger_userID(Passenger passenger, const char *user_id);

/**
 * @brief Creates a passenger, transforming data.
 * 
 * @param tokens Valid tokens previously verified.
 * 
 * @return Pointer to the Passenger struct.
*/
void* parse_passenger(Tokens tokens);

/**
 * @brief Default pre processor for the passenger catalog.
 * 
 * @param stream File to be stored.
 * @param store Store that connects the outside of the function with the inside.
 * @param args Variadic arguments.
 * 
 * @return void.
*/
void preprocessor_passenger(FILE* stream, ParserStore store, va_list args);

/**
 * @brief Creates the passengers_errors file and writes the lines with invalid data.
 * 
 * @param raw_data Invalid data contained in the line.
 * @param store Connection with the outside part of the parser, used to store the discard file address.
 * 
 * @return void.
*/
void discard_passenger(void* raw_data, ParserStore store);

/**
 * @brief Prints a passenger (Mainly usage for debug).
 * 
 * @param passenger Pointer to the passenger struct.
 * 
 * @return void.
*/
void print_passenger(void* pt);

#endif