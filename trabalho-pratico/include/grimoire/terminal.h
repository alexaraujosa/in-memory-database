#ifndef TP_GRIMOIRE_TERMINAL_H
#define TP_GRIMOIRE_TERMINAL_H

#include "grimoire/grimoire_public.h"

/* ============== TYPEDEFS ============== */

/* ============== TERM ============== */
GM_Term gm_term_init();
void gm_term_end(GM_Term term);

/**
 * Checks whenever the current terminal is POSSIBLY an XTerm terminal.
 * Due to terminfo having a fuckton and a half of memory leaks, only baked-in XTerm ANSI codes are used, because I don't
 * have the time to read the terminfo database manually.
 * 
 * @param term The current terminal.
 */
int gm_term_is_xterm(GM_Term term);

/**
 * Returns the display size of the terminal.
 * 
 * @param term The current terminal.
 */
GM_TERM_SIZE gm_term_get_size(GM_Term term);

/* ============== TUI SIZE ============== */
GM_TERM_SIZE gm_get_tui_size();

/* ============== EVENTS ============== */
#define GM_TERM_EVENT_LISTENER(name) void(*name)(GM_Term, int)
typedef GM_TERM_EVENT_LISTENER(TermEventListener);

#define GM_TERMINAL_RESIZE_LISTENER(name) void(*name)(GM_Term, GM_TERM_SIZE)
typedef GM_TERMINAL_RESIZE_LISTENER(TermResizeListener);

void gm_refresh(GM_Term term);

// void gm_setup_tui_events();
// void gm_close_tui_events();
// char* gm_term_get_key_cap(GM_Term term, char* key);

int gm_term_add_tui_resize_listener(GM_Term term, GM_TERMINAL_RESIZE_LISTENER(listener));
int gm_term_remove_tui_resize_listener(GM_Term term, GM_TERMINAL_RESIZE_LISTENER(listener));

#endif