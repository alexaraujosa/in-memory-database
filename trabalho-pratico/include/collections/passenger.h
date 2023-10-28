#ifndef TP_COLLECTIONS_PASSENGER_H
#define TP_COLLECTIONS_PASSENGER_H

#include "debug.h"
#include "user.h"

typedef struct passenger {
    int flight_id; // Relates to Flight
    UserId(user_id); // Relates to User
} PASSENGER, *Passenger;

PASSENGER makePassenger(
    int flight_id,
    UserId(user_id)
);

PASSENGER parsePassengerFromLine(char* line, int len);

#endif