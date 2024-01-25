#ifndef TP_QUERY_7_H
#define TP_QUERY_7_H

#include "queries/queries_common.h"

typedef struct query7_info {
    char* origin;
    int median;
} Q_INFO7, *Q_info7;

int query7_verify(Query query, void* catalogues, char** error);

void query7(char flag, int argc, char** argv, void** catalogues, FILE* output_file, QueryWriter writer);

char* query7_write(void* query_info);

#endif