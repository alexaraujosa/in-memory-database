#ifndef TP_GRIMOIRE_TERMINAL_H
#define TP_GRIMOIRE_TERMINAL_H

#include "grimoire/grimoire_public.h"

/* ============== TYPEDEFS ============== */
// typedef struct gm_term TERM, *GM_Term;

// typedef struct gm_term_size {
//     unsigned short int rows;
//     unsigned short int cols;
// } GM_TERM_SIZE;

/* ============== TERM ============== */
GM_Term gm_term_init();

/* ============== TUI SIZE ============== */
GM_TERM_SIZE gm_get_tui_size();

/* ============== EVENTS ============== */
#define GM_TERM_EVENT_LISTENER(name) void(*name)(GM_Term, int)
typedef GM_TERM_EVENT_LISTENER(TermEventListener);

#define GM_TERMINAL_RESIZE_LISTENER(name) void(*name)(GM_Term, GM_TERM_SIZE)
typedef GM_TERMINAL_RESIZE_LISTENER(TermResizeListener);

void gm_setup_tui_events();
void gm_close_tui_events();
int gm_term_add_tui_resize_listener(GM_Term term, GM_TERMINAL_RESIZE_LISTENER(listener));
int gm_term_remove_tui_resize_listener(GM_Term term, GM_TERMINAL_RESIZE_LISTENER(listener));

#endif