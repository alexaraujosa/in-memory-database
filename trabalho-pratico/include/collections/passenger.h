#ifndef TP_COLLECTIONS_PASSENGER_H
#define TP_COLLECTIONS_PASSENGER_H

#include "debug.h"
#include "user.h"
#include "parser/parser.h"

typedef struct passenger {
    int flight_id; // Relates to Flight
    UserId(user_id); // Relates to User
} PASSENGER, *Passenger;

Passenger make_passenger(
    int flight_id,
    UserId(user_id)
);

Passenger parse_passenger(Tokens tokens);
void discard_passenger(void* raw_data, ParserStore store);
PASSENGER parsePassengerFromLine(char* line, int len);

#endif