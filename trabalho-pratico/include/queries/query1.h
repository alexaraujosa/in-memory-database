#ifndef TP_QUERY_1_H
#define TP_QUERY_1_H

#include "queries/queries_common.h"
#include <stdint.h>

typedef struct {
    char* name;
    bool sex;
    uint8_t age;
    char* country_code;
    int n_flights;
    char* passport;
    int n_reservas;
    double total_spent;
} QUERY1_USER, *Query1_user;

typedef struct {
    char* airline;
    char* airplane_model;
    char* origin;
    char* destination;
    int schedule_departure_date;
    int schedule_arrival_date;
    int passangers;
    short int delay;
} QUERY1_FLIGHT, *Query1_flight;

typedef struct {
    short int hotelID;
    char* hotel_name;
    unsigned short int hotel_stars;
    int reservation_begin_date;
    int reservation_end_date;
    bool breakfast;
    int nights;
    double total_price;
} QUERY1_RESERVATION, *Query1_reservation;

typedef struct query1_info {
    short int type;  // 1 = user, 2 = flight, 3 = reservation
    Query1_user user_info;
    Query1_flight flight_info;
    Query1_reservation reservation_info;
} Q_INFO1, *Q_info1;

void query1(char flag, int argc, char** argv, void** catalogues, FILE* output_file, QueryWriter writer);

char* query1_write(void* query_info);

#endif