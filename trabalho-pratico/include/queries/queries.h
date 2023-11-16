#ifndef TP_QUERY_H
#define TP_QUERY_H

#include "common.h"
#include "debug.h"
#include "util/io.h"
#include "util/misc.h"
#include "parser/parser.h"
#include "catalog/catalogManager.h"

#define QUERIES_CHAR_LEN 3 // Including null-terminator
#define QUERIES_MAX_ARGS 3

typedef struct {
    char id[QUERIES_CHAR_LEN];
    char flag;
    int argc;
    char* argv[QUERIES_MAX_ARGS];
} QUERY, *Query;

char* query_execute(Query query, Catalog** catalogues);
void query_run_bulk(char* input_file, char* output_filer, Catalog** catalogues);
void query_run_single(char* query, ssize_t len);

char* query1(char flag, int argc, char** argv, Catalog** catalogues);
char* query2(char flag, int argc, char** argv, Catalog** catalogues);
char* query3(char flag, int argc, char** argv, Catalog** catalogues);
char* query4(char flag, int argc, char** argv, Catalog** catalogues);
char* query5(char flag, int argc, char** argv, Catalog** catalogues);
char* query6(char flag, int argc, char** argv, Catalog** catalogues);
char* query7(char flag, int argc, char** argv, Catalog** catalogues);
char* query8(char flag, int argc, char** argv, Catalog** catalogues);
char* query9(char flag, int argc, char** argv, Catalog** catalogues);
char* query10(char flag, int argc, char** argv, Catalog** catalogues);

#endif