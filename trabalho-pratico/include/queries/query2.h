#ifndef TP_QUERY_2_H
#define TP_QUERY_2_H

#include "queries/queries_common.h"

typedef struct query2_info {
    short int type;      // 0 = flight, 1 = reservation, 2 = both
    short int both_type; // 0 = flight, 1 = reservation
    int id;
    int date;
} Q_INFO2, *Q_info2;

int query2_verify(Query query, void* catalogues, char** error);

void query2(char flag, int argc, char** argv, void** catalogues, FILE* output_file, QueryWriter writer);

char* query2_write(void* query_info);

#endif