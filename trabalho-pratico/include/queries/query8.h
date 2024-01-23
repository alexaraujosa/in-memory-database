#ifndef TP_QUERY_8_H
#define TP_QUERY_8_H

#include "queries/queries_common.h"

typedef struct query8_info {
    int revenue;
} Q_INFO8, *Q_info8;

void query8(char flag, int argc, char** argv, void** catalogues, FILE* output_file, QueryWriter writer);

char* query8_write(void* query_info);

#endif