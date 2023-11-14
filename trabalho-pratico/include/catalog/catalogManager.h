#ifndef CATALOG_MANAGER_H
#define CATALOG_MANAGER_H

#include "common.h"
// #include "util/error.h"
// #include "util/misc.h"
// #include "util/string.h"

// Ao declarar a estrutura deste modo, mantemos o encapsulamento dos dados e permitimos a ao outros modulos usar apenas apontadores para catalogos..
// não tendo acesso direto ao que está dentro da estrutura.
typedef struct catalog Catalog;

/**
 * @brief Initializes a new Catalog structure.
 *
 * @param compareFunc A pointer to the function used for comparison in the GTree structure.
 *
 * @return A pointer to the newly allocated Catalog structure.
 */
Catalog *catalog_init(GCompareDataFunc tree_compare_func, GHashFunc hash_function, GEqualFunc equals);

/**
 * @brief Adds an item to the Catalog.
 *
 * @param catalog The Catalog structure.
 * @param hashkey The hashTable key of the item to be added(must be a string).
 * @param key The key of the item to be added.
 * @param value A pointer to the item to be added.
 */
void catalog_add_str_to_catalog(Catalog *catalog, gpointer hashKey, gpointer treeKey, gpointer value);

/**
 * @brief Adds an item to the Catalog.
 *
 * @param catalog The Catalog structure.
 * @param hashkey The hashTable key of the item to be added(must be an int).
 * @param key The key of the item to be added.
 * @param value A pointer to the item to be added.
 */
void catalog_add_int_to_catalog(Catalog *catalog, gpointer hashKey, gpointer treeKey, gpointer value);

/**
 * @brief Removes an item from the Catalog.
 *
 * @param catalog The Catalog structure.
 * @param key The key of the item to be removed.
 */
void catalog_remove_from_catalog(Catalog *catalog, gpointer key);

/**
 * @brief Searches for an item in the GHashTable within the Catalog.
 *
 * @param catalog The Catalog structure.
 * @param key The key of the item to be searched(must be a string).
 * @return A pointer to the item if found, otherwise NULL.
 */
void *catalog_search_in_str_hashtable(Catalog *catalog, gpointer key);

/**
 * @brief Searches for an item in the GHashTable within the Catalog.
 *
 * @param catalog The Catalog structure.
 * @param key The key of the item to be searched(must be an int).
 * @return A pointer to the item if found, otherwise NULL.
 */
void *catalog_search_in_int_hashtable(Catalog *catalog, int key);

/**
 * @brief Searches for an item in the GTree within the Catalog.
 *
 * @param catalog The Catalog structure.
 * @param key The key of the item to be searched.
 * @return A pointer to the item if found, otherwise NULL.
 */
void *catalog_search_in_tree(Catalog *catalog, gpointer key);

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
 * @brief Prints all items in the GTree within the Catalog.
 *
 * @param catalog The Catalog structure.
 * @param printFunction A pointer to the function used to print each value in the hash table.
 */
void catalog_print_tree(Catalog *catalog, void (*printFunction)(gpointer));

/**
 * @brief Clears all items from the Catalog.
 *
 * @param catalog The Catalog structure.
 */
void catalog_clear_all_items(Catalog *catalog, GCompareDataFunc treeCompareFunc);

/**
 * @brief Destroys the Catalog structure, freeing all associated resources.
 *
 * @param catalog The Catalog structure to be destroyed.
 */
void catalog_destroy(Catalog *catalog);

/* FUNÇÕES QUE SÃO INTERNAS AO CÓDIGO... NÃO AS VOU QUERER DISPONIBILIZAR A NÃO SER QUE SEJA ABSOLUTAMENTE NECESSÁRIO
void catalog_remove_item_from_tree(Catalog *catalog, char *key);
void catalog_remove_item_from_hashtable(Catalog *catalog, char *key);
void catalog_add_item_to_hashtable(Catalog *catalog, char *key, void *value);
void catalog_add_item_to_tree(Catalog *catalog, char *key, void *value);
*/

#endif
