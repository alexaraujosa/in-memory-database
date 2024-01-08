/**
 * QUERIES
 * 
 * This module is composed by the Query Manager and the Queries.
 * 
 * Catalogue order:
 * 0: Users
 * 1: Flights
 * 2: Passengers
 * 3: Reservations
 */

#ifndef TP_QUERY_H
#define TP_QUERY_H

#include "common.h"
#include "debug.h"
#include "util/io.h"
#include "util/misc.h"
#include "stats/stats.h"
#include "parser/parser.h"
#include "collections/flight.h"
#include "collections/passenger.h"
#include "collections/user.h"
#include "collections/reservation.h"
#include "catalog/catalogManager.h"
#include "catalog/usersCatalog.h"
#include "catalog/flightsCatalog.h"
#include "catalog/reservationsCatalog.h"

#define QUERIES_CHAR_LEN 3 // Including null-terminator
#define QUERIES_MAX_ARGS 3

typedef struct {
    char id[QUERIES_CHAR_LEN];
    char flag;
    int argc;
    char* argv[QUERIES_MAX_ARGS];
} QUERY, *Query;

void query_execute(Query query, Catalog** catalogues, FILE* output_file);
void query_run_bulk(char* input_file, char* output_filer, Catalog** catalogues);
void query_run_single(char* query, ssize_t len);

void query1(char flag, int argc, char** argv, Catalog** catalogues, FILE* output_file);
void query2(char flag, int argc, char** argv, Catalog** catalogues, FILE* output_file);
void query3(char flag, int argc, char** argv, Catalog** catalogues, FILE* output_file);
void query4(char flag, int argc, char** argv, Catalog** catalogues, FILE* output_file);
void query5(char flag, int argc, char** argv, Catalog** catalogues, FILE* output_file);
void query6(char flag, int argc, char** argv, Catalog** catalogues, FILE* output_file);
void query7(char flag, int argc, char** argv, Catalog** catalogues, FILE* output_file);
void query8(char flag, int argc, char** argv, Catalog** catalogues, FILE* output_file);
void query9(char flag, int argc, char** argv, Catalog** catalogues, FILE* output_file);
void query10(char flag, int argc, char** argv, Catalog** catalogues, FILE* output_file);

#endif