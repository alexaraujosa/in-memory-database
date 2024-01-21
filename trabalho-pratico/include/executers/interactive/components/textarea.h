#ifndef TP_EXECUTERS_INTERACTIVE_COMPONENT_TEXTAREA_H
#define TP_EXECUTERS_INTERACTIVE_COMPONENT_TEXTAREA_H

#include "executers/interactive/interactive_common.h"
#include "grimoire/grimoire.h"
#include "cache/cache.h"

typedef struct textarea *TextArea;
typedef void* TextAreaKeypressCallbackUserData;
typedef Keypress_Code(*TextAreaKeypressCallback)(GM_Term, FrameStore, TextArea, GM_Key, TextAreaKeypressCallbackUserData);

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
 * @brief Returns the input a TextArea instance currently holds.
 * 
 * @param text_area The TextArea instance. 
 */
char* get_textarea_input(TextArea text_area);

/**
 * @brief Returns top left X coordinate of a TextArea instance.
 * 
 * @param text_area The TextArea instance.
 */
int get_textarea_x(TextArea text_area);

/**
 * @brief Returns top left Y coordinate of a TextArea instance.
 * 
 * @param text_area The TextArea instance.
 */
int get_textarea_y(TextArea text_area);

/**
 * @brief Returns the size in the X axis of a TextArea instance.
 * 
 * @param text_area The TextArea instance.
 */
int get_textarea_size_cols(TextArea text_area);

/**
 * @brief Returns the size in the Y axis of a TextArea instance.
 * 
 * @param text_area The TextArea instance.
 */
int get_textarea_size_rows(TextArea text_area);

/**
 * @brief Returns whether a TextArea instance is active.
 * 
 * @param text_area The TextArea instance.
 */
int is_textarea_active(TextArea text_area);

/**
 * @brief Sets the active state of a TextArea instance.
 * 
 * @param text_area The TextArea instance.
 * @param active Either TRUE or FALSE.
 */
void set_textarea_active(TextArea text_area, int active);

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
Keypress_Code keypress_textarea(
    GM_Term term, 
    FrameStore store, 
    TextArea text_area, 
    GM_Key key, 
    TextAreaKeypressCallback callback,
    TextAreaKeypressCallbackUserData user_data
);

#endif