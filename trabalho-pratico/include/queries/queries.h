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

void destroy_query(Query query);

/**
 * @brief Verifies if a query is valid.
 * 
 * @param[in] query The query to verify.
 * @param[in] dd The current Dataset Data.
 * @param[out] error The error with the query, if any, NULL otherwise.
 * 
 * @return zero if the query is valid, non-zero otherwise.
 */
int query_verify_raw(Query query, DatasetData dd, char** error);

void query_execute(Query query, void** catalogues, FILE* output_file, QueryWriter writer);

void query_run_bulk(char* input_file, char* output_filer, void** catalogues);
GArray* query_run_single(char* query, ssize_t len, DatasetData dd);

/**
 * @brief Executes a query directly without parsing.
 * 
 * @param query The query to be executed.
 * @param dd The current Dataset Data.
 * 
 * @note A call to @ref query_verify_raw is required to avoid possible crashes.
 */
GArray* query_run_single_raw(Query query, DatasetData dd);

#endif