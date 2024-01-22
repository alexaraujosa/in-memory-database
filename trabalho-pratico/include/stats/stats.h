/**
 * @file stats.h
 * @brief Header file for statistical calculations related to flights.
 *
 * Detailed description of the file (if needed).
 */

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

/**
 * @brief Struct to hold origin_delay information.
 */
typedef struct origin_delay {
    char origin[4];
    short int delay;
} ORIGIN_DELAY, *Origin_delay;

/**
 * @brief Struct to hold statistics information.
 */
typedef struct stats_info {
    GHashTable *aeroports;
    GHashTable *origins;
    GArray *origin_delay;
    GArray *query10;
} STATS_INFO, *Stats_info;

/**
 * @brief Calculate the total price of a reservation.
 *
 * Detailed description of the function.
 * @param reservation The reservation for which to calculate the total price.
 * @return The total price of the reservation.
 */
int calculate_reservation_total_price(Reservation reservation);

/**
 * @brief Calculate the median delay for flights from a specific origin.
 *
 * Detailed description of the function.
 * @param flights_catalog The catalog of flights.
 * @param origin_name The name of the origin to calculate the median delay for.
 * @return The median delay for flights from the specified origin.
 */
int calculate_flight_delay_median(Catalog *flights_catalog, char *origin_name);

/**
 * @brief Calculate the number of passengers for each aeroport.
 *
 * Detailed description of the function.
 * @param origin_passengers Hash table to store the number of passengers for each origin.
 * @param flight_catalog Catalog of flights.
 * @param year The year for which to calculate the number of passengers.
 */
void calculate_aeroport_n_passengers(GHashTable *origin_passengers, Catalog *flight_catalog, int *year);

/**
 * @brief Create a statistics information structure.
 *
 * Detailed description of the function.
 * @param users_catalog Catalog of flights.
 * @param flights_catalog Catalog of flights.
 * @param generic_catalog Pointer to the generic_Catalog
 * @return A pointer to the created Stats_info structure.
 */
Stats_info create_stats_info(Catalog* users_catalog, Catalog *flights_catalog, GArray* generic_catalog);

/**
 * @brief Destroy a Stats_info structure and free associated memory.
 *
 * Detailed description of the function.
 * @param stats The Stats_info structure to destroy.
 */
void stats_destroy(Stats_info stats);

#endif /* STATS_H */
