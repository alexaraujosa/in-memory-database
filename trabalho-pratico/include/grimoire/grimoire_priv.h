#ifndef TP_GRIMOIRE_PRIV_H
#define TP_GRIMOIRE_PRIV_H

#include "grimoire/grimoire_public.h"
#include <stdint.h>

/* ============== CONSTANTS ============== */

/**
 * Max supported ANSI Sequence
 * \\e[BBB;FFF;B;U;Bm
 * \\e[BG_COLOR;FG_COLOR;BOLD;UNDERLINE;BLINKm
 */
#define MAX_ANSI_SEQ 17
#define MAX_UTF8_SEQ 4 // Max UTF8 char sequence
#define MAX_CHAR_SEQ_BYTES MAX_ANSI_SEQ + MAX_UTF8_SEQ // Maximum bytes per character

/* ============== TYPEDEFS ============== */

typedef struct gm_buf {
    char** data;
    int rows;
    int cols;
} GM_BUF, *GM_Buf;

typedef struct gm_color_pair {
    uint8_t id;
    uint8_t fg_color;
    uint8_t bg_color;
} GM_COLOR_PAIR, *GM_ColorPair;

typedef struct gm_term {
    GM_TERM_SIZE size;
    GM_Buf buf;
    GHashTable* color_pairs;
    uint16_t attr;
} TERM, *GM_Term;

// ------- terminal.c -------
int gm_term_buf_truepos(GM_Term term, int row, int col);

#endif