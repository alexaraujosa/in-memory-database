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

int verify_flight_tokens(Tokens tokens);

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

int get_flight_id(Flight flight);
void set_flight_id(Flight flight, int id);
const char *get_flight_airline(Flight flight);
void set_flight_airline(Flight flight, const char *airline);
const char *get_flight_plane_model(Flight flight);
void set_flight_plane_model(Flight flight, const char *plane_model);
const char *get_flight_origin(Flight flight);
void set_flight_origin(Flight flight, const char *origin);
const char *get_flight_destination(Flight flight);
void set_flight_destination(Flight flight, const char *destination);
int get_flight_schedule_departure_date(Flight flight);
void set_flight_schedule_departure_date(Flight flight, int schedule_departure_date);
int get_flight_schedule_arrival_date(Flight flight);
void set_flight_schedule_arrival_date(Flight flight, int schedule_arrival_date);
int get_flight_real_departure_date(Flight flight);
void set_flight_real_departure_date(Flight flight, int real_departure_date);

Flight parse_flight(Tokens tokens);
void discard_flight(void* raw_data, ParserStore store);
FLIGHT parseFlightFromLine(char* line, int len);

void print_flight(void* flight);

#endif