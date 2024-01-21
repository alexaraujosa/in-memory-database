/**
 * @file usersCatalog.h
 * @brief Header file for users catalog functions.
 */

#ifndef USERS_CATALOG_H
#define USERS_CATALOG_H

#include "catalog/catalogManager.h"
#include "catalog/genericCatalog.h"
#include "collections/user.h"
#include "parser/parser.h"
#include "common.h"

/**
 * @brief Compare function for sorting users in the catalog based on name and ID.
 *
 * This function compares two users based on their names and user IDs. It is intended for use with
 * Glib's sorting functions.
 *
 * @param user_A Pointer to the first user.
 * @param user_B Pointer to the second user.
 * @return An integer less than, equal to, or greater than zero if the first argument is considered
 * to be respectively less than, equal to, or greater than the second.
 */
gint usersCatalog_full_compare_func(gconstpointer user_A, gconstpointer user_B);

/**
 * @brief Compare function for sorting users in the catalog based on name and ID using strcoll.
 *
 * This function compares two users based on their names and user IDs using the strcoll function.
 * It is intended for use with Glib's sorting functions.
 *
 * @param user_A Pointer to the first user.
 * @param user_B Pointer to the second user.
 * @return An integer less than, equal to, or greater than zero if the first argument is considered
 * to be respectively less than, equal to, or greater than the second.
 */
gint usersCatalog_strcoll_compare_func(gconstpointer user_A, gconstpointer user_B);

/**
 * @brief Compare function for searching users in the catalog based on a partial name.
 *
 * This function compares a user's name with a specified partial name. It is intended for use in
 * searching functions.
 *
 * @param user_A Pointer to the user.
 * @param user_name Pointer to the partial name.
 * @return An integer less than, equal to, or greater than zero if the first argument is considered
 * to be respectively less than, equal to, or greater than the second.
 */
gint usersCatalog_name_compare_func(gconstpointer user_A, gconstpointer user_name);

/**
 * @brief Write user information to the catalog.
 *
 * This function adds a user to the catalog using the user's ID as the key.
 *
 * @param _user Pointer to the user.
 * @param store ParserStore containing the user catalog.
 */
void usersCatalog_write_to_catalog(void *_user, ParserStore store);

/**
 * @brief Print users in key-value format.
 *
 * This function prints user information in key-value format, with the user ID as the key.
 *
 * @param key Pointer to the user ID.
 * @param value Pointer to the user.
 */
void usersCatalog_print_user_key_value(void *key, void *value);

#endif // USERS_CATALOG_H
