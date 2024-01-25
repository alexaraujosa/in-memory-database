#ifndef TP_EXECUTERS_INTERACTIVE_SCREEN_XTERM_WARN_H
#define TP_EXECUTERS_INTERACTIVE_SCREEN_XTERM_WARN_H

#include "executers/interactive/interactive_common.h"

/**
 * @brief Creates a cache for the XTerm Warn screen.
 * 
 * @param term The current GM_Term.
 * @param store The current FrameStore.
 */
Cache make_cache_xterm_warn(GM_Term term, FrameStore store);

/**
 * @brief Destroys the cache for the XTerm Warn screen.
 * 
 * @param cache The Cache.
 * @param force Whether to forcefully destroy the cache, if implemented. It frees the cache and sets the pointer to NULL.
 */
void destroy_cache_xterm_warn(Cache cache, int force);

/**
 * @brief Draws the XTerm Warn screen.
 * 
 * @param term The current GM_Term.
 * @param store The current FrameStore.
 */
void draw_xterm_warn(GM_Term term, FrameStore store, Cache cache);

/**
 * @brief Handles keypresses while on the XTterm Warn screen.
 * 
 * @param term The current GM_Term.
 * @param store The current FrameStore.
 * @param key The keypress recieved.
 */
Keypress_Code keypress_xterm_warn(GM_Term term, FrameStore store, Cache cache, GM_Key key);


#endif