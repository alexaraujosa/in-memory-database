/**
 * @file passengersCatalog.h
 * @brief Header file for passengers catalog functions.
 */

#ifndef PASSENGERS_CATALOG_H
#define PASSENGERS_CATALOG_H

#include "catalog/catalogManager.h"
#include "collections/passenger.h"
#include "common.h"

/**
 * @brief Compare function for sorting passengers in the catalog based on multiple criteria.
 *
 * This function compares two passengers based on their user ID and flight ID. It is intended for
 * use with Glib's sorting functions.
 *
 * @param passenger_A Pointer to the first passenger.
 * @param passenger_B Pointer to the second passenger.
 * @return An integer less than, equal to, or greater than zero if the first argument is considered
 * to be respectively less than, equal to, or greater than the second.
 */
gint passengersCatalog_full_compare_func(gconstpointer passenger_A, gconstpointer passenger_B);

/**
 * @brief Compare function for searching passengers in the catalog based on flight ID.
 *
 * This function compares a passenger with a flight ID. It is intended for use with Glib's searching
 * functions.
 *
 * @param passenger_A Pointer to the passenger.
 * @param passenger_flightID Pointer to the flight ID.
 * @return An integer less than, equal to, or greater than zero if the first argument is considered
 * to be respectively less than, equal to, or greater than the second.
 */
gint passengersCatalog_flightID_compare_func(gconstpointer passenger_A, gconstpointer passenger_flightID);

/**
 * @brief Compare function for searching passengers in the catalog based on user ID.
 *
 * This function compares a passenger with a user ID. It is intended for use with Glib's searching
 * functions.
 *
 * @param passenger_A Pointer to the passenger.
 * @param passenger_userID Pointer to the user ID.
 * @return An integer less than, equal to, or greater than zero if the first argument is considered
 * to be respectively less than, equal to, or greater than the second.
 */
gint passengersCatalog_userID_compare_func(gconstpointer passenger_A, gconstpointer passenger_userID);

/**
 * @brief Write passenger information to the catalog.
 *
 * This function adds a passenger to the catalog without using a specific key.
 *
 * @param _passenger Pointer to the passenger.
 * @param store ParserStore containing the passenger catalog.
 */
void passengersCatalog_write_to_catalog(void* _passenger, ParserStore store);

#endif  // PASSENGERS_CATALOG_H
