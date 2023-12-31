#ifndef TP_GRIMOIRE_DRAW_H
#define TP_GRIMOIRE_DRAW_H

#include <stdint.h>

#include "grimoire/grimoire_public.h"

void gm_printf(GM_Term term, int row, int col, const char *format, ...);

#endif