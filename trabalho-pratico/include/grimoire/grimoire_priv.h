#ifndef TP_GRIMOIRE_PRIV_H
#define TP_GRIMOIRE_PRIV_H
#define TP_GRIMOIRE_LOCK

#include "grimoire/grimoire_public.h"
#include <stdint.h>
#include "common.h"

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
// TODO: Make modules opaque

#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\e[%d;%dH", (y), (x))
#define clear_attr() printf("\e[0m");


#define GM_ATTR_RESOLVER(name) void(*name)(GM_Term, struct gm_attr*)
typedef uint16_t GM_AttrInt;
typedef struct gm_attr {
    int type;
    int row_start;
    int col_start;
    int row_end;
    int col_end;
    GM_ATTR_RESOLVER(resolver);
    GM_AttrInt data;
} GM_ATTR, *GM_Attr;

typedef struct gm_buf {
    char** data;
    int charsize;
    int rows;
    int cols;
} GM_BUF, *GM_Buf;

// Name incompatibility with attr flag macro.
typedef struct gm_color_pair {
    uint8_t id;
    uint8_t fg_color;
    uint8_t bg_color;
} GM_COLOR_PAIR_STRUCT, *GM_ColorPair;

typedef struct gm_term {
    GM_TERM_SIZE size;
    GM_Buf buf;
    GM_Buf print_buf;
    GHashTable* color_pairs;
    GHashTable* colors;
    GM_AttrInt attr;
    GArray* attr_queue;
} GM_TERM, *GM_Term;

// ------- terminal.c -------
int gm_term_buf_truepos(GM_Term term, int row, int col);

// ------- attr.c -------
typedef enum {
    GM_ATTR_TYPE_LINE,
    GM_ATTR_TYPE_length
} GM_AttrTypes;

GM_Attr gm_attr_make(
    GM_Term term, 
    GM_AttrTypes type,
    int row_start,
    int col_start,
    int row_end,
    int col_end,
    GM_ATTR_RESOLVER(resolver)
);
void gm_attr_destroy(GM_Attr attr);

// Defined here as it is related to attributes, but only used by the terminal.
// TODO: Remove term from name?
void    gm_term_attr_add(GM_Term term, GM_Attr attr);
GM_Attr gm_term_attr_get(GM_Term term, int ind);
void    gm_term_attr_reset(GM_Term term);

void gm_attr_resolve_line(GM_Term term, GM_Attr attr);
void gm_attr_resolve(GM_Term term, GM_Attr attr);

// ------- color.c -------
#include "grimoire/color.h"
// ==== FETCHERS ====
RGBColor gm_get_color(GM_Term term, int id);
GM_ColorPair gm_get_color_pair(GM_Term term, int id);

#endif