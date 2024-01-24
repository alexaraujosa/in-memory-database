#ifndef TP_QUERY_10_H
#define TP_QUERY_10_H

#include "queries/queries_common.h"

typedef struct query10_info {
    int type;
    int users;
    int flights;
    int passengers;
    int unique_passengers;
    int reservations;
    int date;
} Q_INFO10, *Q_info10;

void query10(char flag, int argc, char** argv, void** catalogues, FILE* output_file, QueryWriter writer);

char* query10_write(void* query_info);

#endif