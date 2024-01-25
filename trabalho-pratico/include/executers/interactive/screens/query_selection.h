#ifndef TP_EXECUTERS_INTERACTIVE_SCREEN_QUERY_SELECTION_H
#define TP_EXECUTERS_INTERACTIVE_SCREEN_QUERY_SELECTION_H

#include "executers/interactive/interactive_common.h"

/**
 * @brief Creates a cache for the Query Selection screen.
 * 
 * @param term The current GM_Term.
 * @param store The current FrameStore.
 */
Cache make_cache_query_selection(GM_Term term, FrameStore store);

/**
 * @brief Destroys the cache for the Query Selection screen.
 * 
 * @param cache The Cache.
 * @param force Whether to forcefully destroy the cache, if implemented. It frees the cache and sets the pointer to NULL.
 */
void destroy_cache_query_selection(Cache cache, int force);

/**
 * @brief Draws the Query Selection screen.
 * 
 * @param term The current GM_Term.
 */
void draw_query_selection(GM_Term term, FrameStore store, Cache cache);

/**
 * @brief Handles keypresses while on the Query Selection screen.
 * 
 * @param term The current GM_Term.
 * @param store The current FrameStore.
 * @param key The keypress recieved.
 */
Keypress_Code keypress_query_selection(GM_Term term, FrameStore store, Cache cache, GM_Key key);

#endif