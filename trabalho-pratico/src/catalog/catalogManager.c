#include "catalog/catalogManager.h"

typedef struct catalog {
    GHashTable *hashTable;
    GTree *tree;
    int itemCount;
} Catalog;

// TODO Verificar se as funções que quero usar dentro da hash estão bem
Catalog *catalog_init(GCompareDataFunc tree_compare_func) {
    Catalog *catalog = g_malloc(sizeof(Catalog));
    catalog->hashTable = g_hash_table_new(g_str_hash, g_str_equal);
    catalog->tree = g_tree_new(tree_compare_func);
    catalog->itemCount = 0;
    return catalog;
}

static void _add_to_hashtable(GHashTable *hashTable, gpointer key, gpointer value) {
    gchar *formattedKey = NULL;

    if (GPOINTER_TO_INT(key) || GPOINTER_TO_INT(key) == 0) {
        formattedKey = g_strdup_printf("%p", key);
    } else if (GPOINTER_TO_INT(key) == 0 && g_utf8_validate((const gchar *)key, -1, NULL)) {
        formattedKey = g_strdup(key);
    } else {
        g_warning("Tipo de chave não suportado");
        return;
    }

    g_hash_table_insert(hashTable, formattedKey, value);

    g_free(formattedKey);
}

static void _remove_from_hashtable(GHashTable *hashTable, char *key) {
    gpointer value = g_hash_table_lookup(hashTable, key);
    if (value) {
        g_free(key);
        g_hash_table_remove(hashTable, key);
    }
}

static void _add_to_tree(GTree *tree, void *key, void *value) {
    g_tree_insert(tree, key, value);
}

static void _remove_from_tree(GTree *tree, char *key) {
    g_tree_remove(tree, key);
}

void catalog_add_to_catalog(Catalog *catalog, void *hashKey, void *treeKey, void *value) {
    _add_to_hashtable(catalog->hashTable, hashKey, value);
    _add_to_tree(catalog->tree, treeKey, value);
    catalog->itemCount++;
}

void catalog_remove_from_catalog(Catalog *catalog, char *key) {
    _remove_from_hashtable(catalog->hashTable, key);
    _remove_from_tree(catalog->tree, key);
    catalog->itemCount--;
}

void *catalog_search_in_hashtable(Catalog *catalog, char *key) {
    return g_hash_table_lookup(catalog->hashTable, key);
}

void *catalog_search_in_tree(Catalog *catalog, char *key) {
    return g_tree_lookup(catalog->tree, key);
}

int catalog_get_item_count(Catalog *catalog) {
    return catalog->itemCount;
}

void catalog_print_hash_table(Catalog *catalog, void (*printFunction)(void *)) {
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, catalog->hashTable);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        printFunction(value);
    }
}

void catalog_print_tree(Catalog *catalog, void (*printFunction)(void *)) {
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
