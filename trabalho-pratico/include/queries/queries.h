#ifndef TP_QUERY_H
#define TP_QUERY_H

#include "common.h"
#include "debug.h"
#include "util/io.h"
#include "util/misc.h"
#include "parser/parser.h"

#define QUERIES_CHAR_LEN 3 // Including null-terminator
#define QUERIES_MAX_ARGS 3

typedef struct {
    char id[QUERIES_CHAR_LEN];
    char flag;
    char* args[QUERIES_MAX_ARGS];
} QUERY, *Query;

void* query_execute(Query query, char* args);
void query_run_bulk(char* input_file, char* output_file);
void query_run_single(char* query, ssize_t len);

#endif