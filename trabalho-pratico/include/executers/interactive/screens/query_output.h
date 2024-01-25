#ifndef TP_EXECUTERS_INTERACTIVE_SCREEN_QUERY_OUTPUT_H
#define TP_EXECUTERS_INTERACTIVE_SCREEN_QUERY_OUTPUT_H

#include "executers/interactive/interactive_common.h"

void make_query_page_cache(FrameStore store);

void reset_query_page_cache(FrameStore store);

void load_query_page_cache(FrameStore store);

void destroy_query_page_cache(FrameStore store, int force);

/**
 * @brief Creates a cache for the Query Output screen.
 * 
 * @param term The current GM_Term.
 * @param store The current FrameStore.
 */
Cache make_cache_query_output(GM_Term term, FrameStore store);

/**
 * @brief Destroys the cache for the Query Output screen.
 * 
 * @param cache The Cache.
 * @param force Whether to forcefully destroy the cache, if implemented. It frees the cache and sets the pointer to NULL.
 */
void destroy_cache_query_output(Cache cache, int force);

/**
 * @brief Draws the Query Output screen.
 * 
 * @param term The current GM_Term.
 */
void draw_query_output(GM_Term term, FrameStore store, Cache cache);

/**
 * @brief Handles keypresses while on the Query Output screen.
 * 
 * @param term The current GM_Term.
 * @param store The current FrameStore.
 * @param key The keypress recieved.
 */
Keypress_Code keypress_query_output(GM_Term term, FrameStore store, Cache cache, GM_Key key);


#endif