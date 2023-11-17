#ifndef USER_CATALOG
#define USER_CATALOG

#include "collections/user.h"
#include "common.h"

/**
 * @file user_catalog.h
 * @brief Header file for User Catalog.
 */

/**
 * @brief Comparison function for the User Catalog.
 *
 * @param a The first item to compare.
 * @param b The second item to compare.
 * @return An integer less than, equal to, or greater than zero if a is found, respectively, to be less than, equal to, or greater than b.
 */
gint usersCatalog_full_compare_func(gconstpointer a, gconstpointer b);

gint users_full_compare_func(gconstpointer a, gconstpointer b);
gint user_username_compare_func(gconstpointer a, gconstpointer b);

/**
 * @brief Prints the User Catalog array.
 *
 * @param data The data to be printed.
 * @param user_data The user data.
 */
void usersCatalog_print_array(gpointer data, gpointer user_data);

/**
 * @brief Writes a user to the User Catalog.
 *
 * @param user The user to be added.
 * @param store The parser store.
 */
void usersCatalog_write_to_catalog(User user, ParserStore store);

/**
 * @brief Prints key-value pairs of the User Catalog.
 *
 * @param key The key to be printed.
 * @param value The value to be printed.
 */
void usersCatalog_print_user_key_value(void *key, void *value);

#endif /* USER_CATALOG */
