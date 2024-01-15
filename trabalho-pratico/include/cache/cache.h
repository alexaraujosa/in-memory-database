#ifndef TP_CACHE_H
#define TP_CACHE_H

#include "common.h"

typedef struct cache *Cache;

/**
 * @brief Creates a new cache instance. 
 * The cache stores pointers to data without copying it. The responsibility to ensure the data is not modified
 * belongs to the caller.
 * 
 * @param free_func A function that frees the data inserted inside the cache.
 */
Cache make_cache(GFreeFunc free_func);

/**
 * @brief Destroys a cache instance.
 * 
 * @param cache A cache instance.
 */
void destroy_cache(Cache cache, int free_cache);

/**
 * @brief Checks whenever a non-freed cache was destroyed.
 * 
 * @warning This is an unsafe method, and it is the caller's responsibility to ensure the cache instance was not freed.
 * 
 * @param cache A cache instance.
 */
int is_cache_destroyed(Cache cache);

/**
 * @brief Checks if a cache instance contains a key. 
 * 
 * @param cache A Cache instance.
 * @param key A key that might be contained inside the cache instance.
 */
int has_cache_elem(Cache cache, char* key);

/**
 * @brief Returns an element from the cache.
 * If it does not exist, NULL is returned.
 * 
 * @param cache A Cache instance.
 * @param key A key that might be contained inside the cache instance.
 */
gpointer get_cache_elem(Cache cache, char* key);

/**
 * @brief Sets an element in the cache.
 * If it does not exist, it is inserted.
 * 
 * @param cache A Cache instance.
 * @param key A key for the element.
 * @param value The value to be set.
 */
void set_cache_elem(Cache cache, char* key, gpointer value);

/**
 * @brief Inserts an element in the cache.
 * If it already exists, 1 is returned, 0 otherwise.
 * 
 * @param cache A Cache instance.
 * @param key A key for the element.
 * @param value The value to be inserted.
 */
int add_cache_elem(Cache cache, char* key, gpointer value);

/**
 * @brief Removes an element from the cache.
 * If it does not exist, 1 is returned, 0 otherwise.
 * 
 * @param cache A Cache instance.
 * @param key A key for the element.
 */
int remove_cache_elem(Cache cache, char* key);

/**
 * @brief Removes all elements from the cache.
 * 
 * @param cache A Cache instance.
 */
void clear_cache(Cache cache);

/**
 * @brief Memoizes an element in the cache.
 * If it does not exist, it is added to the cache and the value is returned.
 * If it exists, it is returned.
 * 
 * @param cache A Cache instance.
 * @param key A key for the element.
 */
gpointer memoize_cache_elem(Cache cache, char* key, gpointer value);

#endif