#ifndef TP_QUERY_3_H
#define TP_QUERY_3_H

#include "queries/queries_common.h"

typedef struct query3_info {
    double rating;
} Q_INFO3, *Q_info3;

void query3(char flag, int argc, char** argv, void** catalogues, FILE* output_file, QueryWriter writer);

char* query3_write(void* query_info);

#endif