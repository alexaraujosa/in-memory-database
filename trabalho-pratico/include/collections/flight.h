#ifndef TP_COLLECTIONS_FLIGHT_H
#define TP_COLLECTIONS_FLIGHT_H

#include "common.h"
#include "util/error.h"
#include "util/misc.h"
#include "util/string.h"
#include "user.h"

#define MAX_PLANE_NAME_LEN 60

#define MAX_AIRLINE_NAME_LEN 60

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

/**
 * @brief Creates a flight using dynamic memory.
 * 
 * @param id Flight id.
 * @param airline Flight airline.
 * @param plane_model Flight plane model.
 * @param origin Flight origin.
 * @param destination Flight destination.
 * @param schedule_departure_date Offset from system start date, relative to the flight schedule departure date.
 * @param schedule_arrival_data Offset from system start date, relative to the flight schedule arrival date.
 * @param real_departure_date Offset from system start date, relative to the flight real departure date.
 * 
 * @return Pointer to the Flight struct.
*/
Flight makeFlight(
    int id,
    char* airline,
    char* plane_model,
    char* origin,
    char* destination,
    int schedule_departure_date,
    int schedule_arrival_date,
    int real_departure_date
);

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

/**
 * @brief Creates a flight, transforming data.
 * 
 * @param tokens Valid tokens previously verified.
 * 
 * @return Pointer to the Flight struct.
*/
void* parse_flight(Tokens tokens);

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