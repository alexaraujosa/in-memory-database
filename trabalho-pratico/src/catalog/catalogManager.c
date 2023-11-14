#include "catalog/catalogManager.h"

typedef struct catalog {
    GHashTable *hashTable;
    GTree *tree;
    int itemCount;
} Catalog;

Catalog *catalog_init(GCompareDataFunc tree_compare_func, GHashFunc hash_function, GEqualFunc equals) {
    Catalog *catalog = g_malloc(sizeof(Catalog));
    catalog->hashTable = g_hash_table_new_full(hash_function, equals, NULL, free);
    catalog->tree = g_tree_new(tree_compare_func);
    catalog->itemCount = 0;
    return catalog;
}

static void _add_to_hashtable(GHashTable *hashTable, gpointer key, gpointer value) {
    g_hash_table_insert(hashTable, key, value);
}

static void _remove_from_hashtable(GHashTable *hashTable, gpointer key) {
    g_hash_table_remove(hashTable, key);
}

static void _add_to_tree(GTree *tree, gpointer key, gpointer value) {
    g_tree_insert(tree, key, value);
}

static void _remove_from_tree(GTree *tree, gpointer key) {
    g_tree_remove(tree, key);
}

void catalog_add_str_to_catalog(Catalog *catalog, gpointer hashKey, gpointer treeKey, gpointer value) {
    _add_to_hashtable(catalog->hashTable, g_strdup(hashKey), value);
    _add_to_tree(catalog->tree, treeKey, value);
    catalog->itemCount++;
}

void catalog_add_int_to_catalog(Catalog *catalog, gpointer hashKey, gpointer treeKey, gpointer value) {
    _add_to_hashtable(catalog->hashTable, hashKey, value);
    _add_to_tree(catalog->tree, treeKey, value);
    catalog->itemCount++;
}

void catalog_remove_from_catalog(Catalog *catalog, gpointer key) {
    _remove_from_hashtable(catalog->hashTable, key);
    _remove_from_tree(catalog->tree, key);
    catalog->itemCount--;
}

void *catalog_search_in_str_hashtable(Catalog *catalog, gpointer key) {
    return g_hash_table_lookup(catalog->hashTable, key);
}

void *catalog_search_in_int_hashtable(Catalog *catalog, int key) {
    return g_hash_table_lookup(catalog->hashTable, GINT_TO_POINTER(key));
}

void *catalog_search_in_tree(Catalog *catalog, gpointer key) {
    return g_tree_lookup(catalog->tree, key);
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

void catalog_print_tree(Catalog *catalog, void (*printFunction)(gpointer)) {
    GTraverseFunc printTreeFunc = (GTraverseFunc)printFunction;
    g_tree_foreach(catalog->tree, printTreeFunc, NULL);
}

void catalog_clear_all_items(Catalog *catalog, GCompareDataFunc treeCompareFunc) {
    g_hash_table_remove_all(catalog->hashTable);
    g_tree_destroy(catalog->tree);
    catalog->tree = g_tree_new(treeCompareFunc);
    catalog->itemCount = 0;
}

void catalog_destroy(Catalog *catalog) {
    g_hash_table_destroy(catalog->hashTable);
    g_tree_destroy(catalog->tree);
    g_free(catalog);
}