#ifndef TP_EXECUTERS_BATCH_H
#define TP_EXECUTERS_BATCH_H

#include "collections/user.h"
#include "collections/reservation.h"
#include "collections/flight.h"
#include "collections/passenger.h"
#include "queries/queries.h"
#include "catalog/userCatalog.h"
#include "catalog/reservationsCatalog.h"
#include "catalog/flightsCatalog.h"
#include "catalog/passengersCatalog.h"
#include "catalog/catalogManager.h"

void test_preprocessor(FILE* stream, ParserStore store, va_list args);
void test_parser(Tokens tokens);
void test_writer(void* raw_data, FILE** store);

void batch(char* arg1, char* arg2);

#endif