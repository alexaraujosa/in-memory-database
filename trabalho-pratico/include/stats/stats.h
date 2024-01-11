#ifndef STATS_H
#define STATS_H

#include "catalog/catalogManager.h"
#include "catalog/flightsCatalog.h"
#include "catalog/passengersCatalog.h"
#include "catalog/reservationsCatalog.h"
#include "catalog/usersCatalog.h"
#include "collections/flight.h"
#include "collections/passenger.h"
#include "collections/reservation.h"
#include "collections/user.h"
#include "common.h"

typedef struct stats_info{
    //Acrescentar mais se necessaio
    GArray *origin_delay;
} STATS_INFO, *Stats_info;


gint reservationsCatalog_hotelID_compare_func(gconstpointer a, gconstpointer b);

typedef struct users_stats USERS_STATS, *Users_stats;

typedef struct reservations_stats RESERVATIONS_STATS, *Reservations_stats;

typedef struct flights_stats FLIGHTS_STATS, *Flights_stats;

typedef struct hotels_stats HOTELS_STATS, *Hotels_stats;

typedef struct aeroport_stats AEROPORT_STATS, *Aeroport_stats;

int calculate_user_n_flights(Catalog *catalog, char *userID);

double calculate_user_total_spent(Catalog *catalog, char *userID, int *n_reservations);

int calculate_reservation_total_price(Reservation reservation);

int calculate_flight_total_passengers(Catalog *catalog, int *flightId);

int calculate_flight_delay_median(Catalog *catalog, char *origin_name);

int calculate_aeroport_n_passengers(Catalog *flights_catalog, char *origin_name, int *year);

int calculate_aeroport_n_passengers2(Catalog *flight_catalog, char *origin_name, int *year);

Stats_info create_stats_info(Catalog *flights_catalog);

#endif