#ifndef TP_COLLECTIONS_FLIGHT_H
#define TP_COLLECTIONS_FLIGHT_H

#include "common.h"
#include "util/error.h"
#include "util/misc.h"
#include "util/string.h"
#include "util/date.h"
#include "user.h"

#define MAX_PLANE_NAME_LEN 13

#define MAX_AIRLINE_NAME_LEN 31

#define LOCATION_LEN 3

typedef struct flights FLIGHT, *Flight;

/**
 * @brief Verifies if tokens are valid to create a flight.
 * 
 * @param tokens A pointer to the struct used for verifying the data.
 * 
 * @return 1 if all the tokens are valid. Otherwise, 0.
*/
int verify_flight_tokens(Tokens tokens, ParserStore store);

int get_flight_id(const Flight flight);
void set_flight_id(Flight flight, int id);
char *get_flight_airline(const Flight flight);
void set_flight_airline(Flight flight, const char *airline);
char *get_flight_plane_model(const Flight flight);
void set_flight_plane_model(Flight flight, const char *plane_model);
char *get_flight_origin(const Flight flight);
void set_flight_origin(Flight flight, const char *origin);
char *get_flight_destination(const Flight flight);
void set_flight_destination(Flight flight, const char *destination);
int get_flight_schedule_departure_date(const Flight flight);
void set_flight_schedule_departure_date(Flight flight, int schedule_departure_date);
int get_flight_schedule_arrival_date(const Flight flight);
void set_flight_schedule_arrival_date(Flight flight, int schedule_arrival_date);
int get_flight_real_departure_date(const Flight flight);
void set_flight_real_departure_date(Flight flight, int real_departure_date);
int get_flight_passengers(const Flight flight);
void set_flight_passengers(Flight flight, int passengers);
void increment_flight_passengers(Flight flight);

/**
 * @brief Creates a flight, transforming data.
 * 
 * @param tokens Valid tokens previously verified.
 * 
 * @return Pointer to the Flight struct.
*/
void* parse_flight(Tokens tokens);

/**
 * @brief Flight pre processor for parsing a file.
 * 
 * @param stream File to be stored.
 * @param store Store that connects the outside of the function with the inside.
 * @param args Variadic arguments.
 * 
 * @return void.
*/
void preprocessor_flight(FILE* stream, ParserStore store, va_list args);

/**
 * @brief Creates the flight_errors file and writes the lines with invalid data.
 * 
 * @param raw_data Invalid data contained in the line.
 * @param store Connection with the outside part of the parser, used to store the discard file address.
 * 
 * @return void.
*/
void discard_flight(void* raw_data, ParserStore store);

/**
 * @brief Prints a flight (Mainly usage for debug).
 * 
 * @param flight Pointer to the flight struct.
 * 
 * @return void.
*/
void print_flight(void* flight);

#endif