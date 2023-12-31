#ifndef TP_GRIMOIRE_ATTR_H
#define TP_GRIMOIRE_ATTR_H

#include <stdint.h>

#include "grimoire/grimoire_public.h"

#define GM_RESET      0
#define GM_BOLD       1 << 1
#define GM_ITALIC     1 << 2
#define GM_UNDERLINE  1 << 3
#define GM_BLINK      1 << 4

#define _GM_COLOR_OFFSET 5
#define _GM_ATRR_COUNT 5

// #define GM_ColorPair(color) ((uint8_t)(color)) << _GM_COLOR_OFFSET
#define GM_COLOR_PAIR(color) (((uint8_t)(color) + 1) << _GM_COLOR_OFFSET)

void gm_attron(GM_Term term, int attr);
void gm_attroff(GM_Term term, int attr);

#endif