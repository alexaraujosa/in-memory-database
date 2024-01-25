#ifndef TP_QUERY_4_H
#define TP_QUERY_4_H

#include "queries/queries_common.h"

typedef struct query4_info {
    int reservationID;
    int reservation_begin_date;
    int reservation_end_date;
    char* userID;
    int reservation_rating;
    double price;
} Q_INFO4, *Q_info4;

int query4_verify(Query query, void* catalogues, char** error);

void query4(char flag, int argc, char** argv, void** catalogues, FILE* output_file, QueryWriter writer);

char* query4_write(void* query_info);

#endif