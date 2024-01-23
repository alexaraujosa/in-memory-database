#ifndef TP_QUERY_5_H
#define TP_QUERY_5_H

#include "queries/queries_common.h"

typedef struct query5_info {
    int flight_id;
    int schedule_departure_date;
    char* destination;
    char* airline;
    char* plane_model;
} Q_INFO5, *Q_info5;

void query5(char flag, int argc, char** argv, void** catalogues, FILE* output_file, QueryWriter writer);

char* query5_write(void* query_info);

#endif