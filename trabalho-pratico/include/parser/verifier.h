#ifndef TP_VERIFIER_H
#define TP_VERIFIER_H

#include "util/string.h"

int verify_user_tokens(const char** parameter);
int verify_reservation_tokens(const char** parameter);
int verify_flight_tokens(const char** parameter);
int verify_passenger_tokens(const char** parameter);

#endif