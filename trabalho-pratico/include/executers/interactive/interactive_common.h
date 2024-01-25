#ifndef TP_EXECUTERS_INTERACTIVE_COMMON_H
#define TP_EXECUTERS_INTERACTIVE_COMMON_H

#include "grimoire/grimoire.h"
#include "data/locale.h"
#include "data/locale_keys.h"
#include "data/settings.h"
#include "cache/cache.h"
#include "executers/datasets.h"
#include "queries/queries.h"

// // When using KEY_SPECIAL on screen keypress handlers, if information needs to be passed to the manager,
// // set this key. It should not be defined for anything else and be considered a reserved key.
// #define CACHE_KEY_TRANSPORT_BUS "__transport_bus__"

typedef enum screen_id {
    SCREEN_XTERM_WARN,
    SCREEN_SETTINGS,
    SCREEN_MAIN_MENU,
    SCREEN_DATASET_QUESTION,
    SCREEN_LOADING,
    SCREEN_QUERY_OUTPUT,
    SCREEN_QUERY_SELECTION
} ScreenId;

enum color_ids {
    COLOR_DEFAULT_BG = 0,
    COLOR_DEFAULT_FG = 1,
    COLOR_WHITE = 2,
    COLOR_BLACK,
    COLOR_RED,
    COLOR_GREY1
};

enum color_pair_ids {
    COLORPAIR_DEFAULT = 0,
    COLORPAIR_SELECTED = 1,
    COLORPAIR_ERROR,
    COLORPAIR_DISABLED
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

typedef struct defer_control *DeferControl;

typedef struct frame_store {
    int awaiting_keypress;
    XTerm_State is_XTerm; // 0 - Uninitialized | 1 - Is XTerm | 2 - Not XTerm & Not confirmed | 3- Not XTerm & confirmed
    DataLocales locales;
    DataLocale current_locale;
    GHashTable* screen_caches;
    DataSettings settings;
    ScreenId current_screen;
    DatasetData datasets;
    DeferControl defer_control;
    // char* current_query;
    Query current_query;
    GArray** query_pages;
    int load_query;
} FRAME_STORE, *FrameStore;

// Separated from the DeferControl due to it's dependency on FrameStore.
/**
 * @brief A DeferNotify function executes deferred work when called.
 */
typedef void(*DeferNotify)(GM_Term, FrameStore);

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

/**
 * @brief Initializes the Defer Control.
 * 
 * @param term The current Terminal.
 * @param store The current FrameStore.
 */
DeferControl make_defer_control();

/**
 * @brief Destroys the Defer Control.
 * 
 * @param defer_control The Defer Control sequence.
 */
void destroy_defer_control(DeferControl df);

/**
 * @brief Defers a task to be executed at a later point.
 * 
 * @param defer_control The Defer Control sequence.
 * @param term The current Terminal.
 * @param store The current FrameStore.
 */
void defer_load(DeferControl defer_control, DeferNotify notify_func);

/**
 * @brief Attempts to run the defered task, if it exists.
 * 
 * @param defer_control The Defer Control sequence.
 * @param term The current Terminal.
 * @param store The current FrameStore.
 */
void defer_try(DeferControl defer_control, GM_Term term, FrameStore store);

/**
 * @brief Invalidates the cache of a given screen.
 * 
 * @param term The current Terminal.
 * @param store The current FrameStore.
 * @param id The ID of the Screen to invalidate.
 */
void invalidate_screen_cache(GM_Term term, FrameStore store, ScreenId id);

#endif