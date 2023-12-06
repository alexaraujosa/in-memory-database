#ifndef FLIGHTS_CATALOG
#define FLIGHTS_CATALOG

#include "collections/flight.h"
#include "common.h"

/**
 * @file flights_catalog.h
 * @brief Header file for Flights Catalog.
 */

/**
 * @brief Comparison function for the Flights Catalog.
 *
 * @param a The first item to compare.
 * @param b The second item to compare.
 * @return An integer less than, equal to, or greater than zero if a is found, respectively, to be less than, equal to, or greater than b.
 */
gint flightsCatalog_full_compare_func(gconstpointer a, gconstpointer b);

gint flight_origin_compare_func(gconstpointer a, gconstpointer b);

gint flight_destination_compare_func(gconstpointer a, gconstpointer b);
gint flight_destination_compare_funcB(gconstpointer a, gconstpointer b);

/**
 * @brief Prints the Flights Catalog array.
 *
 * @param data The data to be printed.
 * @param flight_data The flight data.
 */
void flightsCatalog_print_array(gpointer data, gpointer flight_data);

/**
 * @brief Writes a flight to the Flights Catalog.
 *
 * @param flight The flight to be added.
 * @param store The parser store.
 */
void flightsCatalog_write_to_catalog(void* _flight, ParserStore store);

/**
 * @brief Prints key-value pairs of the Flights Catalog.
 *
 * @param key The key to be printed.
 * @param value The value to be printed.
 */
void flightsCatalog_print_flights_key_value(void *key, void *value);

#endif /* FLIGHTS_CATALOG */