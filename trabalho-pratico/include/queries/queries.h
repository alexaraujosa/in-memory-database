/**
 * QUERIES
 *
 * This module is composed by the Query Manager and the Queries.
 *
 * Catalogue order:
 * 0: Users
 * 1: Flights
 * 2: Passengers
 * 3: Reservations
 */

#ifndef TP_QUERY_H
#define TP_QUERY_H

#include "catalog/catalogManager.h"
#include "catalog/flightsCatalog.h"
#include "catalog/reservationsCatalog.h"
#include "catalog/usersCatalog.h"
#include "collections/flight.h"
#include "collections/passenger.h"
#include "collections/reservation.h"
#include "collections/user.h"
#include "common.h"
#include "debug.h"
#include "output/output.h"
#include "parser/parser.h"
#include "stats/stats.h"
#include "util/io.h"
#include "util/misc.h"

#define QUERIES_CHAR_LEN 3  // Including null-terminator
#define QUERIES_MAX_ARGS 3

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

typedef struct query2_info {
    // TO DEFINE
} Q_INFO2, *Q_info2;

typedef struct query3_info {
    double rating;
} Q_INFO3, *Q_info3;

typedef struct query4_info {
    int reservationID;
    int reservation_begin_date;
    int reservation_end_date;
    char* userID;
    int reservation_rating;
    double price;
} Q_INFO4, *Q_info4;

typedef struct query5_info {
    int flight_id;
    int schedule_departure_date;
    char* destination;
    char* airline;
    char* plane_model;
} Q_INFO5, *Q_info5;

typedef struct query6_info {
    char origin[4];
    int passangers;
} Q_INFO6, *Q_info6;
typedef struct query7_info {
    char* origin;
    int median;
} Q_INFO7, *Q_info7;

typedef struct query8_info {
    int revenue;
} Q_INFO8, *Q_info8;

typedef struct query9_info {
    char* userID;
    char* user_name;
} Q_INFO9, *Q_info9;

typedef struct query10_info {
    // TO DEFINE
} Q_INFO10, *Q_info10;

typedef struct {
    char id[QUERIES_CHAR_LEN];
    char flag;
    int argc;
    char* argv[QUERIES_MAX_ARGS];
} QUERY, *Query;

void query_execute(Query query, void** catalogues, FILE* output_file);
void query_run_bulk(char* input_file, char* output_filer, void** catalogues);
void query_run_single(char* query, ssize_t len);

void query1(char flag, int argc, char** argv, void** catalogues, FILE* output_file);
void query2(char flag, int argc, char** argv, void** catalogues, FILE* output_file);
void query3(char flag, int argc, char** argv, void** catalogues, FILE* output_file);
void query4(char flag, int argc, char** argv, void** catalogues, FILE* output_file);
void query5(char flag, int argc, char** argv, void** catalogues, FILE* output_file);
void query6(char flag, int argc, char** argv, void** catalogues, FILE* output_file);
void query7(char flag, int argc, char** argv, void** catalogues, FILE* output_file);
void query8(char flag, int argc, char** argv, void** catalogues, FILE* output_file);
void query9(char flag, int argc, char** argv, void** catalogues, FILE* output_file);
void query10(char flag, int argc, char** argv, void** catalogues, FILE* output_file);

#endif