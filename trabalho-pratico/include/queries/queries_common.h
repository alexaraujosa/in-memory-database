#ifndef TP_QUERY_COMMON_H
#define TP_QUERY_COMMON_H

#include "catalog/catalogManager.h"
#include "catalog/flightsCatalog.h"
#include "catalog/reservationsCatalog.h"
#include "catalog/usersCatalog.h"
#include "collections/flight.h"
#include "collections/passenger.h"
#include "collections/reservation.h"
#include "collections/user.h"
// #include "data/locale.h"
#include "data/locale_keys.h"

#include "common.h"
#include "debug.h"

#define QUERIES_CHAR_LEN 3  // Including null-terminator
#define QUERIES_MAX_ARGS 3

typedef struct query {
    char id[QUERIES_CHAR_LEN];
    char flag;
    int argc;
    char* argv[QUERIES_MAX_ARGS];
} QUERY, *Query;

typedef void(*QueryWriter)(int, char, void*, FILE*, int);
typedef int(*QueryVerifier)(Query query, void* catalogues, char** error);

#endif