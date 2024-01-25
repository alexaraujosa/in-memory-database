#ifndef TP_QUERY_9_H
#define TP_QUERY_9_H

#include "queries/queries_common.h"

typedef struct query9_info {
    char* userID;
    char* user_name;
} Q_INFO9, *Q_info9;

int query9_verify(Query query, void* catalogues, char** error);

void query9(char flag, int argc, char** argv, void** catalogues, FILE* output_file, QueryWriter writer);

char* query9_write(void* query_info);

#endif