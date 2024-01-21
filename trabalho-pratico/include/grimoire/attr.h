#ifndef TP_GRIMOIRE_ATTR_H
#define TP_GRIMOIRE_ATTR_H

#include <stdint.h>

#include "grimoire/grimoire_public.h"

#define GM_RESET                0
#define GM_BOLD                 1 << 1
#define GM_ITALIC               1 << 2
#define GM_UNDERLINE            1 << 3
#define GM_BLINK                1 << 4
#define GM_PRINT_OVERFLOW_BREAK 1 << 5

#define _GM_COLOR_OFFSET 6
#define _GM_ATRR_COUNT 6

#define GM_COLOR_PAIR(color) (((uint8_t)(color) + 1) << _GM_COLOR_OFFSET)

void gm_attron(GM_Term term, int attr);
void gm_attroff(GM_Term term, int attr);

/**
 * @brief Checks whether a specific attribute is currently active.
 * 
 * @param term The current terminal.
 * @param attr The attribute to check.
 */
int gm_has_attribute(GM_Term term, int attr);

/**
 * @brief Sets the top left box drawing character for subsequent draw calls.
 * 
 * @param term The current terminal.
 */
// void gm_set_box_top_left_corner(GM_Term term, GM_Char ch);
void gm_set_box_top_left_corner(GM_Term term, GM_Char ch[MAX_UTF8_SEQ]);

/**
 * @brief Sets the top right box drawing character for subsequent draw calls.
 * 
 * @param term The current terminal.
 */
// void gm_set_box_top_right_corner(GM_Term term, GM_Char ch);
void gm_set_box_top_right_corner(GM_Term term, GM_Char ch[MAX_UTF8_SEQ]);

/**
 * @brief Sets the bottom left box drawing character for subsequent draw calls.
 * 
 * @param term The current terminal.
 */
// void gm_set_box_bottom_left_corner(GM_Term term, GM_Char ch);
void gm_set_box_bottom_left_corner(GM_Term term, GM_Char ch[MAX_UTF8_SEQ]);

/**
 * @brief Sets the bottom right box drawing character for subsequent draw calls.
 * 
 * @param term The current terminal.
 */
// void gm_set_box_bottom_right_corner(GM_Term term, GM_Char ch);
void gm_set_box_bottom_right_corner(GM_Term term, GM_Char ch[MAX_UTF8_SEQ]);

/**
 * @brief Sets the left intersection box drawing character for subsequent draw calls.
 * 
 * @param term The current terminal.
 */
// void gm_set_box_left_intersection(GM_Term term, GM_Char ch);
void gm_set_box_left_intersection(GM_Term term, GM_Char ch[MAX_UTF8_SEQ]);

/**
 * @brief Sets the right intersection box drawing character for subsequent draw calls.
 * 
 * @param term The current terminal.
 */
// void gm_set_box_right_intersection(GM_Term term, GM_Char ch);
void gm_set_box_right_intersection(GM_Term term, GM_Char ch[MAX_UTF8_SEQ]);

/**
 * @brief Sets the top intersection box drawing character for subsequent draw calls.
 * 
 * @param term The current terminal.
 */
// void gm_set_box_top_intersection(GM_Term term, GM_Char ch);
void gm_set_box_top_intersection(GM_Term term, GM_Char ch[MAX_UTF8_SEQ]);

/**
 * @brief Sets the bottom intersection box drawing character for subsequent draw calls.
 * 
 * @param term The current terminal.
 */
// void gm_set_box_bottom_intersection(GM_Term term, GM_Char ch);
void gm_set_box_bottom_intersection(GM_Term term, GM_Char ch[MAX_UTF8_SEQ]);

/**
 * @brief Sets the center intersection box drawing character for subsequent draw calls.
 * 
 * @param term The current terminal.
 */
// void gm_set_box_center_intersection(GM_Term term, GM_Char ch);
void gm_set_box_center_intersection(GM_Term term, GM_Char ch[MAX_UTF8_SEQ]);


#endif