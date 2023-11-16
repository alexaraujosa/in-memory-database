#ifndef CATALOG_MANAGER_H
#define CATALOG_MANAGER_H

/**
 * @file catalog_manager.h
 * @brief Header file for Catalog Manager.
 */

/**
 * @struct catalog
 * @brief Represents a Catalog structure.
 */
typedef struct catalog Catalog;

/**
 * @brief Initializes a new Catalog structure.
 *
 * @param hash_function A pointer to the hash function used for the GHashTable.
 * @param equals A pointer to the function used for equality comparison in the GHashTable.
 * @param my_free A pointer to the function used for freeing allocated memory.
 *
 * @return A pointer to the newly allocated Catalog structure.
 */
Catalog *catalog_init(GHashFunc hash_function, GEqualFunc equals, void (*my_free)(void *));

/**
 * @brief Adds an item to the Catalog with a string key.
 *
 * @param catalog The Catalog structure.
 * @param hashKey The hashTable key of the item to be added (must be a string).
 * @param value A pointer to the item to be added.
 */
void catalog_add_str_to_catalog(Catalog *catalog, gpointer hashKey, gpointer value);

/**
 * @brief Adds an item to the Catalog with an integer key.
 *
 * @param catalog The Catalog structure.
 * @param hashKey The hashTable key of the item to be added (must be an int).
 * @param value A pointer to the item to be added.
 */
void catalog_add_int_to_catalog(Catalog *catalog, gpointer hashKey, gpointer value);

/**
 * @brief Removes an item from the Catalog.
 *
 * @param catalog The Catalog structure.
 * @param key The key of the item to be removed.
 */
void catalog_remove_from_catalog(Catalog *catalog, gpointer key);

/**
 * @brief Searches for an item in the GHashTable within the Catalog with a string key.
 *
 * @param catalog The Catalog structure.
 * @param key The key of the item to be searched (must be a string).
 * @return A pointer to the item if found, otherwise NULL.
 */
void *catalog_search_in_str_hashtable(Catalog *catalog, gpointer key);

/**
 * @brief Searches for an item in the GHashTable within the Catalog with an integer key.
 *
 * @param catalog The Catalog structure.
 * @param key The key of the item to be searched (must be an int).
 * @return A pointer to the item if found, otherwise NULL.
 */
void *catalog_search_in_int_hashtable(Catalog *catalog, int key);

/**
 * @brief Checks if an item exists in the Catalog array.
 *
 * @param catalog The Catalog structure.
 * @param target The target item to search for.
 * @param compare_func A pointer to the function used for comparison.
 * @param out_match_index A pointer to store the index of the matching item (if found).
 * @return TRUE if the item exists, FALSE otherwise.
 */
gboolean catalog_exists_in_array(Catalog *catalog, gconstpointer target, GCompareFunc compare_func, guint *out_match_index);

/**
 * @brief Searches for an item in the Catalog array.
 *
 * @param catalog The Catalog structure.
 * @param index The index of the item in the array.
 * @return A pointer to the item if found, otherwise NULL.
 */
void *catalog_search_in_array(Catalog *catalog, guint index);

/**
 * @brief Gets the number of items in the Catalog.
 *
 * @param catalog The Catalog structure.
 * @return The number of items in the Catalog.
 */
int catalog_get_item_count(Catalog *catalog);

/**
 * @brief Prints the key-value pair of the GHashTable within the Catalog.
 *
 * @param catalog The Catalog structure to be printed.
 * @param printFunction A pointer to the function used to print each value in the hash table.
 */
void catalog_print_hash_table(Catalog *catalog, void (*printFunction)(gpointer, gpointer));

/**
 * @brief Prints all items in the Catalog array.
 *
 * @param catalog The Catalog structure.
 * @param printFunction A pointer to the function used to print each value in the array.
 */
void catalog_print_array(Catalog *catalog, void (*printFunction)(gpointer));

/**
 * @brief Sorts the Catalog array using the specified comparison function.
 *
 * @param catalog The Catalog structure.
 * @param compare_func A pointer to the comparison function used for sorting.
 */
void catalog_sort(Catalog *catalog, GCompareFunc compare_func);

/**
 * @brief Clears all items from the Catalog.
 *
 * @param catalog The Catalog structure.
 */
void catalog_clear_all_items(Catalog *catalog);

/**
 * @brief Destroys the Catalog structure, freeing all associated resources.
 *
 * @param catalog The Catalog structure to be destroyed.
 */
void catalog_destroy(Catalog *catalog);

#endif /* CATALOG_MANAGER_H */
