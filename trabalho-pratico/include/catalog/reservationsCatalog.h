#ifndef RESERVATION_CATALOG
#define RESERVATION_CATALOG

#include "common.h"
#include "collections/reservation.h"

/**
 * @file reservation_catalog.h
 * @brief Header file for Reservation Catalog.
 */

/**
 * @brief Comparison function for the Reservation Catalog.
 *
 * @param a The first item to compare.
 * @param b The second item to compare.
 * @return An integer less than, equal to, or greater than zero if a is found, respectively, to be less than, equal to, or greater than b.
 */
gint reservationsCatalog_full_compare_func(gconstpointer *a, gconstpointer *b);

/**
 * @brief Prints the Reservation Catalog array.
 *
 * @param data The data to be printed.
 * @param reservation_data The reservation data.
 */
void reservationsCatalog_print_array(gpointer data, gpointer reservation_data);

/**
 * @brief Writes a reservation to the Reservation Catalog.
 *
 * @param reservation The reservation to be added.
 * @param store The parser store.
 */
void reservationsCatalog_write_to_catalog(RESERVATION *reservation, ParserStore store);

/**
 * @brief Prints key-value pairs of the Reservation Catalog.
 *
 * @param key The key to be printed.
 * @param value The value to be printed.
 */
void reservationsCatalog_print_reservation_key_value(void *key, void *value);

#endif /* RESERVATION_CATALOG */
