#ifndef TP_GRIMOIRE_DRAW_H
#define TP_GRIMOIRE_DRAW_H

#include <stdint.h>

#include "grimoire/grimoire_public.h"

void gm_printf(GM_Term term, int row, int col, const char *format, ...);
void gm_box(GM_Term term, int row_start, int col_start, int row_end, int col_end);

#endif