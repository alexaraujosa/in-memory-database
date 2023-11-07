#include "catalog/catalogManager.h"

typedef struct catalog
{
    GHashTable *hashTable;
    GTree *tree;
    int itemCount;
} Catalog;

// TODO Verificar se as funções que quero usar dentro da hash estão bem
Catalog *catalog_init()
{
    Catalog *catalog = g_malloc(sizeof(Catalog));
    catalog->hashTable = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
    catalog->tree = g_tree_new_full((GCompareDataFunc)g_strcmp0, NULL, g_free, g_free);
    catalog->itemCount = 0;
    return catalog;
}

static void _add_to_hashtable(GHashTable *hashTable, char *key, void *value)
{
    g_hash_table_insert(hashTable, g_strdup(key), value);
}

static void _remove_from_hashtable(GHashTable *hashTable, char *key)
{
    g_hash_table_remove(hashTable, key);
}

static void _add_to_tree(GTree *tree, char *key, void *value)
{
    g_tree_insert(tree, g_strdup(key), value);
}

static void _remove_from_tree(GTree *tree, char *key)
{
    g_tree_remove(tree, key);
}

void catalog_add_to_catalog(Catalog *catalog, char *key, void *value)
{
    _add_to_hashtable(catalog->hashTable, key, value);
    _add_to_tree(catalog->tree, key, value);
    catalog->itemCount++;
}

void catalog_remove_from_catalog(Catalog *catalog, char *key)
{
    _remove_from_hashtable(catalog->hashTable, key);
    _remove_from_tree(catalog->tree, key);
    catalog->itemCount--;
}

void *catalog_search_in_hashtable(Catalog *catalog, char *key)
{
    return g_hash_table_lookup(catalog->hashTable, key);
}

void *catalog_search_in_tree(Catalog *catalog, char *key)
{
    return g_tree_lookup(catalog->tree, key);
}

int catalog_get_item_count(Catalog *catalog)
{
    return catalog->itemCount;
}

void catalog_print_hash_table(Catalog *catalog)
{
    GHashTableIter hashIter;
    gpointer hashKey, hashValue;
    g_hash_table_iter_init(&hashIter, catalog->hashTable);
    while (g_hash_table_iter_next(&hashIter, &hashKey, &hashValue))
    {
        printf("Key: %s, Value: %p\n", (char *)hashKey, hashValue);
    }
}

// TODO Maneira mais simples de imprimir a arvore (esquerda para a diretira)
gboolean printTree(gpointer key, gpointer value)
{
    printf("Key: %s, Value: %p\n", (char *)key, value);
    return FALSE;
}

void catalog_print_tree(Catalog *catalog)
{
    GTraverseFunc printTreeFunc = (GTraverseFunc)printTree;
    g_tree_foreach(catalog->tree, printTreeFunc, NULL);
}

void catalog_rebalance_tree(Catalog *catalog)
{
    // g_tree_balance(catalog->tree);
}

void catalog_clear_all_items(Catalog *catalog)
{
    g_hash_table_remove_all(catalog->hashTable);
    g_tree_destroy(catalog->tree);
    catalog->tree = g_tree_new_full((GCompareDataFunc)g_strcmp0, NULL, g_free, g_free);
    catalog->itemCount = 0;
}

void catalog_destroy(Catalog *catalog)
{
    g_hash_table_destroy(catalog->hashTable);
    g_tree_destroy(catalog->tree);
    g_free(catalog);
}