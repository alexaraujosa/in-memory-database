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

gint user_name_compare_func(gconstpointer a, gconstpointer b);

gint usersCatalog_strcoll_compare_func(gconstpointer a, gconstpointer b);

/**
 * @brief Writes a user to the User Catalog.
 *
 * @param user The user to be added.
 * @param store The parser store.
 */
void usersCatalog_write_to_catalog(void* _user, ParserStore store);

/**
 * @brief Prints key-value pairs of the User Catalog.
 *
 * @param key The key to be printed.
 * @param value The value to be printed.
 */
void usersCatalog_print_user_key_value(void *key, void *value);

#endif /* USER_CATALOG */
