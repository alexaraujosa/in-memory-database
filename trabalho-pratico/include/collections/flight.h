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

typedef struct flights {
    int id;
    char airline[MAX_AIRLINE_NAME_LEN + 1]; // Maybe GLib String
    char plane_model[MAX_PLANE_NAME_LEN + 1]; // Maybe GLib String
    char origin[LOCATION_LEN + 1];
    char destination[LOCATION_LEN + 1];
    int schedule_departure_date; // Offset from Base Date
    int schedule_arrival_date; // Offset from Base Date
    int real_departure_date; // Offset from Base Date
} FLIGHT, *Flight;

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

Flight parse_flight(Tokens tokens);
void discard_flight(void* raw_data, ParserStore store);
FLIGHT parseFlightFromLine(char* line, int len);

void print_flight(void* flight);

#endif