#include "cache/cache.h"

#ifdef _DEBUG
#pragma GCC push_options
#pragma GCC optimize ("O0")
#endif

typedef struct cache {
    GHashTable* data;
} CACHE, *Cache;

Cache make_cache(GFreeFunc free_func) {
    Cache cache = (Cache)malloc(sizeof(CACHE));
    cache->data = g_hash_table_new_full(g_str_hash, g_str_equal, free, free_func);
    // cache->data = g_hash_table_new_full(g_direct_hash, g_direct_equal, free, free_func);

    return cache;
}

void destroy_cache(Cache cache) {
    clear_cache(cache);
    g_hash_table_destroy(cache->data);
    free(cache);
}

inline int has_cache_elem(Cache cache, char* key) {
    // char* _key = strdup(key);
    // gboolean contains = g_hash_table_contains(cache->data, &_key);
    
    // free(_key);
    // return contains;
    gboolean contains = g_hash_table_contains(cache->data, key);
    return contains;
}

gpointer get_cache_elem(Cache cache, char* key) {
    char* _key = strdup(key);
    if (!g_hash_table_contains(cache->data, _key)) {
        return NULL;
        free(_key);
    }

    gpointer value = g_hash_table_lookup(cache->data, _key);
    free(_key);
    
    return value;
}

void set_cache_elem(Cache cache, char* key, gpointer value) {
    g_hash_table_replace(cache->data, strdup(key), value);
}

int add_cache_elem(Cache cache, char* key, gpointer value) {
    if (has_cache_elem(cache, key)) return 1;
    
    g_hash_table_insert(cache->data, strdup(key), value);
    return 0;
}

int remove_cache_elem(Cache cache, char* key) {
    if (!has_cache_elem(cache, key)) return 1;

    g_hash_table_remove(cache->data, strdup(key));
    return 0;
}

void _clearer(gpointer key, gpointer value, gpointer user_data) {
    return TRUE;
}
void clear_cache(Cache cache) {
    g_hash_table_foreach_remove(cache->data, _clearer, NULL);
}

gpointer memoize_cache_elem(Cache cache, char* key, gpointer value) {
    if (has_cache_elem(cache, key)) return get_cache_elem(cache, key);

    add_cache_elem(cache, key, value);
    gpointer nval = get_cache_elem(cache, key);
    return nval;
}

#ifdef _DEBUG
#pragma GCC pop_options
#endif