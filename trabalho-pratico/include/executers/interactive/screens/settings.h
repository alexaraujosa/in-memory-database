#ifndef TP_EXECUTERS_INTERACTIVE_SCREEN_SETTINGS_H
#define TP_EXECUTERS_INTERACTIVE_SCREEN_SETTINGS_H

#include "executers/interactive/interactive_common.h"

/**
 * @brief Creates a cache for the settings screen.
 * 
 * @param term The current GM_Term.
 * @param store The current FrameStore.
 */
Cache make_cache_settings(GM_Term term, FrameStore store);

/**
 * @brief Destroys the cache for the settings screen.
 * 
 * @param cache The Cache.
 */
void destroy_cache_settings(Cache cache);

/**
 * @brief Draws the settings screen.
 * 
 * @param term The current GM_Term.
 */
void draw_settings(GM_Term term, FrameStore store, Cache cache);

/**
 * @brief Handles keypresses while on the settings screen.
 * 
 * @param term The current GM_Term.
 * @param store The current FrameStore.
 * @param key The keypress recieved.
 */
Keypress_Code keypress_settings(GM_Term term, FrameStore store, Cache cache, GM_Key key);

#endif