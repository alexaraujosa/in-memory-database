/**
 * @file flights_catalog.h
 * @brief Header file for Flights Catalog.
 */

#ifndef FLIGHTS_CATALOG_H
#define FLIGHTS_CATALOG_H

#include "catalog/catalogManager.h"
#include "catalog/genericCatalog.h"
#include "collections/flight.h"
#include "common.h"

/**
 * @brief Compare function for sorting flights in the catalog based on multiple criteria.
 *
 * This function compares two flights based on their origin, schedule departure and arrival dates,
 * and flight ID. It is intended for use with Glib's sorting functions.
 *
 * @param flight_A Pointer to the first flight.
 * @param flight_B Pointer to the second flight.
 * @return An integer less than, equal to, or greater than zero if the first argument is considered
 * to be respectively less than, equal to, or greater than the second.
 */
gint flightsCatalog_full_compare_func(gconstpointer flight_A, gconstpointer flight_B);

/**
 * @brief Compare function for sorting flights in the catalog based on destination (flight and flight).
 *
 * This function compares two flights based on their destination. It is intended for use with Glib's
 * sorting functions.
 *
 * @param flight_A Pointer to the first flight.
 * @param flight_B Pointer to the second flight.
 * @return An integer less than, equal to, or greater than zero if the first argument is considered
 * to be respectively less than, equal to, or greater than the second.
 */
gint flightsCatalog_destination_compare_func(gconstpointer flight_A, gconstpointer flight_B);

/**
 * @brief Compare function for searching flights in the catalog based on destination (flight and string).
 *
 * This function compares a flight with a destination name. It is intended for use with Glib's
 * searching functions.
 *
 * @param flight_A Pointer to the flight.
 * @param flight_destination Pointer to the destination name.
 * @return An integer less than, equal to, or greater than zero if the first argument is considered
 * to be respectively less than, equal to, or greater than the second.
 */
gint flightsCatalog_destination_compare_funcB(gconstpointer flight_A, gconstpointer flight_destination);

/**
 * @brief Compare function for searching flights in the catalog based on origin (alternative).
 *
 * This function compares a flight with an origin name. It is intended for use with Glib's searching
 * functions.
 *
 * @param flight_A Pointer to the flight.
 * @param flight_origin Pointer to the origin name.
 * @return An integer less than, equal to, or greater than zero if the first argument is considered
 * to be respectively less than, equal to, or greater than the second.
 */
gint flightsCatalog_origin_compare_func(gconstpointer flight_A, gconstpointer flight_origin);

/**
 * @brief Write flight information to the catalog.
 *
 * This function adds a flight to the catalog using its ID as the key.
 *
 * @param _flight Pointer to the flight.
 * @param store ParserStore containing the flight catalog.
 */
void flightsCatalog_write_to_catalog(void* _flight, ParserStore store);

/**
 * @brief Print flights in key-value format.
 *
 * This function prints flight information in key-value format, with the flight ID as the key.
 *
 * @param key Pointer to the flight ID.
 * @param value Pointer to the flight.
 */
void flightsCatalog_print_flights_key_value(void* key, void* value);

#endif  // FLIGHTS_CATALOG_H