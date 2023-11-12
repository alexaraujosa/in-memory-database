#ifndef TP_COLLECTIONS_PASSENGER_H
#define TP_COLLECTIONS_PASSENGER_H

#include "debug.h"
#include "user.h"
#include "parser/parser.h"

typedef struct passenger PASSENGER, *Passenger;

int verify_passenger_tokens(Tokens tokens);

Passenger make_passenger(
    int flight_id,
    UserId(user_id)
);

int get_passanger_flightID(Passenger passenger);
void set_passenger_flightID(Passenger passenger, int flight_id);
const char *get_passenger_userdID(Passenger passenger);
void set_passenger_userID(Passenger passenger, const char *user_id);

Passenger parse_passenger(Tokens tokens);
void discard_passenger(void* raw_data, ParserStore store);
PASSENGER parsePassengerFromLine(char* line, int len);

#endif