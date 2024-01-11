#ifndef TP_GRIMOIRE_COLOR_H
#define TP_GRIMOIRE_COLOR_H

#include <stdint.h>

#include "grimoire/grimoire_public.h"

#define GM_COLOR_PAIR_DEFAULT 0
#define GM_COLOR_ID_MIN 0
#define GM_COLOR_ID_MAX 254

typedef struct rgb_color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t mod;
} RGB_COLOR, *RGBColor;

// ======= ASSERTS =======
int gm_has_color(GM_Term term, int id);
int gm_has_color_pair(GM_Term term, int id);

// ======= CONSTRUCTORS =======
void gm_init_color_pair(GM_Term term, int id, int fg, int bg);
void gm_init_color(GM_Term term, int id, uint8_t r, uint8_t g, uint8_t b);
void gm_init_color_defaults(GM_Term term);

#endif