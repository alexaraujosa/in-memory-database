#ifndef TP_QUERY_6_H
#define TP_QUERY_6_H

#include "queries/queries_common.h"

typedef struct query6_info {
    char origin[4];
    int passangers;
} Q_INFO6, *Q_info6;

int query6_verify(Query query, void* catalogues, char** error);

void query6(char flag, int argc, char** argv, void** catalogues, FILE* output_file, QueryWriter writer);

char* query6_write(void* query_info);

#endif