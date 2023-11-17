#ifndef STATS_H
#define STATS_H

gint reservation_hotelID_compare_func(gconstpointer a, gconstpointer b);

typedef struct users_stats USERS_STATS, *Users_stats;

typedef struct reservations_stats RESERVATIONS_STATS, *Reservations_stats;

typedef struct flights_stats FLIGHTS_STATS, *Flights_stats;

typedef struct hotels_stats HOTELS_STATS, *Hotels_stats;

typedef struct aeroport_stats AEROPORT_STATS, *Aeroport_stats;

double calculate_hotel_average_rating(Catalog * catalog, int hotel_id);

int calculate_user_n_flights(Catalog *catalog, char *userID);

double calculate_user_total_spent(Catalog *catalog, char *userID, int *n_reservations);

int calculate_flight_total_passengers(Catalog *catalog, int flightId);

#endif