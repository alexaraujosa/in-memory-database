/**
 * @file reservationsCatalog.h
 * @brief Header file for reservations catalog functions.
 */
#ifndef RESERVATIONS_CATALOG_H
#define RESERVATIONS_CATALOG_H

#include "catalog/catalogManager.h"
#include "catalog/genericCatalog.h"
#include "collections/reservation.h"
#include "stats/stats.h"
#include "common.h"

/**
 * @brief Compare function for sorting reservations in the catalog based on multiple criteria.
 *
 * This function compares two reservations based on their hotel ID, begin date, and reservation ID.
 * It is intended for use with Glib's sorting functions.
 *
 * @param reservation_A Pointer to the first reservation.
 * @param reservation_B Pointer to the second reservation.
 * @return An integer less than, equal to, or greater than zero if the first argument is considered
 * to be respectively less than, equal to, or greater than the second.
 */
gint reservationsCatalog_full_compare_func(gconstpointer* reservation_A, gconstpointer* reservation_B);

/**
 * @brief Compare function for sorting reservations in the catalog based on hotel ID.
 *
 * This function compares a reservation with a hotel ID. It is intended for use with Glib's sorting
 * functions.
 *
 * @param reservation_A Pointer to the reservation.
 * @param reservation_hotelID Pointer to the hotel ID.
 * @return An integer less than, equal to, or greater than zero if the first argument is considered
 * to be respectively less than, equal to, or greater than the second.
 */
gint reservationsCatalog_hotelID_compare_func(gconstpointer reservation_A, gconstpointer reservation_hotelID);

/**
 * @brief Compare function for sorting reservations in the catalog based on begin date.
 *
 * This function compares two reservations based on their begin date and reservation ID. It is
 * intended for use with Glib's sorting functions.
 *
 * @param reservation_A Pointer to the first reservation.
 * @param reservation_B Pointer to the second reservation.
 * @return An integer less than, equal to, or greater than zero if the first argument is considered
 * to be respectively less than, equal to, or greater than the second.
 */
gint reservationsCatalog_date_compare_func(gconstpointer reservation_A, gconstpointer reservation_B);

/**
 * @brief Write reservation information to the catalog.
 *
 * This function adds a reservation to the catalog using its ID as the key.
 *
 * @param _reservation Pointer to the reservation.
 * @param store ParserStore containing the reservation catalog.
 */
void reservationsCatalog_write_to_catalog(void* _reservation, ParserStore store);

/**
 * @brief Print reservations in key-value format.
 *
 * This function prints reservation information in key-value format, with the reservation ID as the key.
 *
 * @param key Pointer to the reservation ID.
 * @param value Pointer to the reservation.
 */
void reservationsCatalog_print_reservation_key_value(void* key, void* value);

#endif  // RESERVATIONS_CATALOG_H
