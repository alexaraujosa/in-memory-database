#include "catalog/catalogManager.h"

#include <glib.h>

#include "common.h"

typedef struct catalog {
    GHashTable *hashTable;
    GArray *array;
    int itemCount;
} Catalog;

Catalog *catalog_init(GHashFunc hash_function, GEqualFunc equals, void (*my_free)(void *)) {
    Catalog *catalog = g_malloc(sizeof(Catalog));
    catalog->hashTable = g_hash_table_new_full(hash_function, equals, my_free, NULL);
    catalog->array = g_array_new(FALSE, FALSE, sizeof(gpointer));
    catalog->itemCount = 0;
    return catalog;
}

static void _add_to_hashtable(GHashTable *hashTable, gpointer key, gpointer value) {
    if (key != NULL) {
        g_hash_table_insert(hashTable, key, value);
    }
}

static void _remove_from_hashtable(GHashTable *hashTable, gpointer key) {
    g_hash_table_remove(hashTable, key);
}

static void _add_to_array(GArray *array, gpointer value) {
    g_array_append_val(array, value);
}

static void _remove_from_array(GArray *array, guint index) {
    g_array_remove_index(array, index);
}

void catalog_add_str_to_catalog(Catalog *catalog, gpointer hashKey, gpointer value) {
    _add_to_hashtable(catalog->hashTable, g_strdup(hashKey), value);
    _add_to_array(catalog->array, value);
    catalog->itemCount++;
}

void catalog_add_int_to_catalog(Catalog *catalog, gpointer hashKey, gpointer value) {
    _add_to_hashtable(catalog->hashTable, hashKey, value);
    _add_to_array(catalog->array, value);
    catalog->itemCount++;
}

void catalog_remove_from_catalog(Catalog *catalog, gpointer key) {
    guint i;
    for (i = 0; i < catalog->itemCount; i++) {
        if (g_array_index(catalog->array, gpointer, i) == key) {
            _remove_from_hashtable(catalog->hashTable, key);
            _remove_from_array(catalog->array, i);
            catalog->itemCount--;
            break;
        }
    }
}

void *catalog_search_in_str_hashtable(Catalog *catalog, gpointer key) {
    return g_hash_table_lookup(catalog->hashTable, key);
}

void *catalog_search_in_int_hashtable(Catalog *catalog, int key) {
    return g_hash_table_lookup(catalog->hashTable, GINT_TO_POINTER(key));
}

gboolean catalog_exists_in_array(Catalog *catalog, gconstpointer target, GCompareFunc compare_func, guint *out_match_index) {
    return g_array_binary_search(catalog->array, target, compare_func, out_match_index);
}

void *catalog_search_in_array(Catalog *catalog, guint index) {
    return g_array_index(catalog->array, gpointer, index);
}

int catalog_get_item_count(Catalog *catalog) {
    return catalog->itemCount;
}

void catalog_print_hash_table(Catalog *catalog, void (*printFunction)(gpointer, gpointer)) {
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, catalog->hashTable);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        printFunction(key, value);
    }
}

void catalog_print_array(Catalog *catalog, void (*printFunction)(gpointer)) {
    guint i;
    for (i = 0; i < catalog->itemCount; i++) {
        printFunction(g_array_index(catalog->array, gpointer, i));
    }
}

void catalog_sort(Catalog *catalog, GCompareFunc compare_func) {
    g_array_sort(catalog->array, compare_func);
}


void catalog_clear_all_items(Catalog *catalog) {
    g_hash_table_remove_all(catalog->hashTable);
    g_array_set_size(catalog->array, 0);
    catalog->itemCount = 0;
}

void catalog_destroy(Catalog *catalog) {
    g_hash_table_destroy(catalog->hashTable);
    for (int i = 0; i < catalog->itemCount; i++) {
        free(g_array_index(catalog->array, gpointer, i));
    }
    g_array_free(catalog->array, TRUE);
    g_free(catalog);
}