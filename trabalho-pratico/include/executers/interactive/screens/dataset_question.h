#ifndef TP_EXECUTERS_INTERACTIVE_SCREEN_DATASET_QUESTION_H
#define TP_EXECUTERS_INTERACTIVE_SCREEN_DATASET_QUESTION_H

#include "executers/interactive/interactive_common.h"

/**
 * @brief Creates a cache for the dataset_question screen.
 * 
 * @param term The current GM_Term.
 * @param store The current FrameStore.
 */
Cache make_cache_dataset_question(GM_Term term, FrameStore store);

/**
 * @brief Destroys the cache for the dataset_question screen.
 * 
 * @param cache The Cache.
 * @param force Whether to forcefully destroy the cache, if implemented. It frees the cache and sets the pointer to NULL.
 */
void destroy_cache_dataset_question(Cache cache, int force);

/**
 * @brief Draws the dataset_question screen.
 * 
 * @param term The current GM_Term.
 */
void draw_dataset_question(GM_Term term, FrameStore store, Cache cache);

/**
 * @brief Handles keypresses while on the dataset_question screen.
 * 
 * @param term The current GM_Term.
 * @param store The current FrameStore.
 * @param key The keypress recieved.
 */
Keypress_Code keypress_dataset_question(GM_Term term, FrameStore store, Cache cache, GM_Key key);

#endif