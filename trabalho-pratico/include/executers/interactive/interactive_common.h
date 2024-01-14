#ifndef TP_EXECUTERS_INTERACTIVE_COMMON_H
#define TP_EXECUTERS_INTERACTIVE_COMMON_H

#include "grimoire/grimoire.h"
#include "data/locale.h"
#include "data/locale_keys.h"
#include "cache/cache.h"

typedef enum xterm_state {
    UNINITIALIZED = 0,
    IS_XTERM,
    NOT_XTERM_UNCONFIRMED,
    NOT_XTERM_CONFIRMED
} XTerm_State;

typedef struct frame_store {
    int awaiting_keypress;
    XTerm_State is_XTerm; // 0 - Uninitialized | 1 - Is XTerm | 2 - Not XTerm & Not confirmed | 3- Not XTerm & confirmed
    DataLocales locales;
    DataLocale current_locale;
    GHashTable* screen_caches;
} FRAME_STORE, *FrameStore;

typedef struct draw_text {
    char* text;
    int len;
    int x;
    int y;
} DRAW_TEXT, *DrawText;

DrawText make_draw_text(char* text, int len, int x, int y);
void destroy_draw_text(DrawText dt);

#endif