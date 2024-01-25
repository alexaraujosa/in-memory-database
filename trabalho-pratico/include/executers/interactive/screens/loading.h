#ifndef TP_EXECUTERS_INTERACTIVE_SCREEN_LOADING_H
#define TP_EXECUTERS_INTERACTIVE_SCREEN_LOADING_H

#include "executers/interactive/interactive_common.h"

/**
 * @brief Creates a cache for the Loading screen.
 * 
 * @param term The current GM_Term.
 * @param store The current FrameStore.
 */
Cache make_cache_loading(GM_Term term, FrameStore store);

/**
 * @brief Destroys the cache for the Loading screen.
 * 
 * @param cache The Cache.
 * @param force Whether to forcefully destroy the cache, if implemented. It frees the cache and sets the pointer to NULL.
 */
void destroy_cache_loading(Cache cache, int force);

/**
 * @brief Draws the Loading screen.
 * 
 * @param term The current GM_Term.
 * @param store The current FrameStore.
 */
void draw_loading(GM_Term term, FrameStore store, Cache cache);

/**
 * @brief Handles keypresses while on the XTterm Warn screen.
 * 
 * @param term The current GM_Term.
 * @param store The current FrameStore.
 * @param key The keypress recieved.
 */
Keypress_Code keypress_loading(GM_Term term, FrameStore store, Cache cache, GM_Key key);


#endif