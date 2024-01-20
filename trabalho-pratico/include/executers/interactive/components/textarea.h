#ifndef TP_EXECUTERS_INTERACTIVE_COMPONENT_TEXTAREA_H
#define TP_EXECUTERS_INTERACTIVE_COMPONENT_TEXTAREA_H

#include "executers/interactive/interactive_common.h"
#include "grimoire/grimoire.h"
#include "cache/cache.h"

typedef struct textarea *TextArea;

/**
 * @brief Creates a new TextArea component.
 * 
 * @param term The current Grimoire Terminal.
 * @param store The current screen's FrameStore.
 * @param x The top left X coordinate for this component.
 * @param y The top left Y coordinate for this component.
 * @param size_cols The size in the X axis for this component.
 * @param size_rows The size in the Y axis for this component.
 * @param max_text_len The maximum length of the input buffer.
 * @param active_colorpair The ColorPair to be used when this component is active.
 * @param inactive_colorpair The ColorPair to be used when this component is inactive.
 * @param header_text The text to be displayed above the TextArea input box.
 */
TextArea make_textarea(
    GM_Term term, 
    FrameStore store, 
    int x, 
    int y, 
    int size_cols, 
    int size_rows, 
    int max_text_len,
    int active_colorpair,
    int inactive_colorpair, 
    char* header_text
);

/**
 * @brief Destroys a given TextArea component.
 * 
 * @param text_area The TextArea to be destroyed.
 */
void destroy_textarea(TextArea text_area);

/**
 * @brief Draws a given TextArea component.
 * 
 * @param term The current Grimoire Terminal.
 * @param store The current screen's FrameStore.
 * @param text_area The TextArea to be drawn.
 */
void draw_textarea(GM_Term term, FrameStore store, TextArea text_area);

/**
 * @brief Handles the keypresses for a  given TextArea component.
 * 
 * @param text_area The TextArea to be handled.
 * @param key The key to be handled. 
 */
Keypress_Code keypress_textarea(TextArea text_area, GM_Key key);

#endif