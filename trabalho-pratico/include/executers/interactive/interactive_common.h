#ifndef TP_EXECUTERS_INTERACTIVE_COMMON_H
#define TP_EXECUTERS_INTERACTIVE_COMMON_H

#include "grimoire/grimoire.h"
#include "data/locale.h"
#include "data/locale_keys.h"
#include "data/settings.h"
#include "cache/cache.h"

typedef enum screen_id {
    SCREEN_XTERM_WARN,
    SCREEN_SETTINGS,
    SCREEN_MAIN_MENU,
    SCREEN_DATASET_QUESTION
} ScreenId;

enum color_ids {
    COLOR_DEFAULT_BG = 0,
    COLOR_DEFAULT_FG = 1,
    COLOR_WHITE = 2,
    COLOR_BLACK
};

enum color_pair_ids {
    COLORPAIR_DEFAULT = 0,
    COLORPAIR_SELECTED = 1
};

typedef enum keypress_code {
    KEY_SKIP,     // Key was ignored.
    KEY_RECIEVED, // Key was consumed successfully
    KEY_ABORT,    // Bubble up and graciously terminate the program
    KEY_SPECIAL   // Caller should interpret the signal, and either bubble up or return another code.
} Keypress_Code;

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
    DataSettings settings;
    ScreenId current_screen;

} FRAME_STORE, *FrameStore;

typedef struct draw_text {
    char* text;
    int len;
    int x;
    int y;
    int rows;
    int cols;
} DRAW_TEXT, *DrawText;

DrawText make_draw_text(char* text, int len, int x, int y, int rows, int cols);
void destroy_draw_text(DrawText dt);

#endif