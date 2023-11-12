#include "collections/passenger.h"

PASSENGER makePassenger(
    int flight_id,
    UserId(user_id)) {
    PASSENGER passenger = {
        .flight_id = flight_id,
        // .user_id = user_id
    };

    strncpy(passenger.user_id, user_id, (size_t)MAX_USER_ID_LEN);

    return passenger;
}

PASSENGER parsePassengerFromLine(char* line, int len) {
    IGNORE_ARG(line);
    IGNORE_ARG(len);
}