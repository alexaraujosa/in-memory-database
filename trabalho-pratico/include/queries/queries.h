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
 * 4: Statistics Info
 */

#ifndef TP_QUERY_H
#define TP_QUERY_H

#include "queries/queries_common.h"
#include "executers/datasets.h"
#include "parser/parser.h"
#include "stats/stats.h"
#include "util/io.h"
#include "util/misc.h"

#define QUERIES_CHAR_LEN 3  // Including null-terminator
#define QUERIES_MAX_ARGS 3

#include "queries/query1.h"
#include "queries/query2.h"
#include "queries/query3.h"
#include "queries/query4.h"
#include "queries/query5.h"
#include "queries/query6.h"
#include "queries/query7.h"
#include "queries/query8.h"
#include "queries/query9.h"
#include "queries/query10.h"

typedef struct query {
    char id[QUERIES_CHAR_LEN];
    char flag;
    int argc;
    char* argv[QUERIES_MAX_ARGS];
} QUERY, *Query;

void destroy_query(Query query);
void query_execute(Query query, void** catalogues, FILE* output_file, QueryWriter writer);
void query_run_bulk(char* input_file, char* output_filer, void** catalogues);
GArray* query_run_single(char* query, ssize_t len, DatasetData dd);
GArray* query_run_single_raw(Query query, DatasetData dd);

#endif