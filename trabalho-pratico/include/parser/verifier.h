#ifndef TP_VERIFIER_H
#define TP_VERIFIER_H

#include "util/string.h"
#include "parser/parser.h"

int verify_user_tokens(Tokens tokens);
int verify_reservation_tokens(Tokens tokens);
int verify_flight_tokens(Tokens tokens);
int verify_passenger_tokens(Tokens tokens);

#endif