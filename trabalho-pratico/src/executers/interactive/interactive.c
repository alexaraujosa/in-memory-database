#include "executers/interactive/interactive.h"
#include "grimoire/grimoire.h"
#include <time.h>
#include <stdlib.h>

// ============================ SCREEN INCLUDES ===========================
#include "executers/interactive/screens/xterm_warn.h"

#pragma GCC push_options
#pragma GCC optimize ("O0")

typedef enum xterm_state {
    UNINITIALIZED = 0,
    IS_XTERM,
    NOT_XTERM_UNCONFIRMED,
    NOT_XTERM_CONFIRMED
} XTerm_State;

typedef enum keypress_code {
    KEY_SKIP,
    KEY_RECIEVED,
    KEY_ABORT
} Keypress_Code;

typedef struct frame_store {
    int awaiting_keypress;
    XTerm_State is_XTerm; // 0 - Uninitialized | 1 - Is XTerm | 2 - Not XTerm & Not confirmed | 3- Not XTerm & confirmed
} FRAME_STORE, *FrameStore;

int make_frame(GM_Term term, FrameStore store);
int build_frame(GM_Term term, FrameStore store);

// ============================ ENTRY POINT ===========================

void interactive() {
    // ======= Set Locale =======
    setlocale(LC_ALL, "en_US.UTF-8");
    setenv("TZ", "", 1);

    // ======= Initialize Terminal =======
    GM_Term term = gm_term_init();

    // ------- Initialize Color Pairs -------
    gm_init_color_defaults(term);

    gm_init_color(term, 2, 255, 0 , 0);
    gm_init_color(term, 3, 0, 255 , 0);
    gm_init_color(term, 4, 0, 0 , 255);

    gm_init_color_pair(term, 1, 2, 3);
    gm_init_color_pair(term, 2, 2, 4);
    gm_init_color_pair(term, 3, 3, 4);

    // ------- Initialize Screen -------
    gm_hide_cursor(term);

    // ------- Initialize Frame Store -------
    FRAME_STORE store = {
        .awaiting_keypress = 0,
        .is_XTerm = 0
    };

    if (gm_term_is_xterm(term)) store.is_XTerm = IS_XTERM;
    else {
        store.is_XTerm = NOT_XTERM_UNCONFIRMED;
        store.awaiting_keypress = 1;
    }
    // store.is_XTerm = NOT_XTERM_UNCONFIRMED;

    // ======= Frame Loop =======
    int stop = 0;
    int tickStart, tickEnd, tickDur, tickDelay;

    while (!stop) {
        tickStart = clock();

        int bf = build_frame(term, &store);
        if (bf) stop = 1;

        int mf = make_frame(term, &store);
        if (mf) stop = 1;

        gm_refresh(term);

        /*
            In order to ensure no tearing occurs with frames overlapping,
            the tick speed is capped to TICK_DURATION_MS. If this tick was
            faster than TICK_DURATION_MS, a delay is added to uniformize the 
            display framerate. If the tick takes longer than TICK_DURATION_MS,
            no delay is added, and the display will retain the previous frame
            until the next tick starts.
        */

        tickEnd = clock();
        tickDur = ((float)(tickEnd - tickStart)) / CLOCKS_PER_SEC * 1000;
        tickDelay = TICK_DURATION_MS - tickDur;
        
        if (tickDelay > 0) {
            usleep((uint32_t)(tickDelay * 1000));
        }
    }

    // ======= Destroy Terminal =======
    gm_term_end(term);
}

// ============================ MAKE FRAME ===========================

int make_frame(GM_Term term, FrameStore store) {
    if (store->is_XTerm == NOT_XTERM_UNCONFIRMED) {
        draw_xterm_warn(term);
        return 0;
    }

    gm_printf(term, 0, 0, "Hello there.");

    return 0;
}

// ============================ BUILD FRAME ===========================

Keypress_Code handle_keypresses(GM_Term term, FrameStore store) {
    GM_Key key = gm_get_key(term);

    if (gm_get_canonical_key(key) == GM_KEY_CTRL_C) return KEY_ABORT;

    // Temporary emergency nuclear button
    if (gm_get_canonical_key(key) == 'a') {
        printf("FUCK\n");
        exit(EXIT_SUCCESS);
    }

    if (store->is_XTerm == NOT_XTERM_UNCONFIRMED) {
        store->is_XTerm = NOT_XTERM_CONFIRMED;
    }

    return KEY_RECIEVED;
}

int build_frame(GM_Term term, FrameStore store) {
    Keypress_Code keypress = 0;
    if (gm_kbhit(term)) keypress = handle_keypresses(term, store);

    if (keypress == KEY_ABORT) return 1;

    return 0;
}

#pragma GCC pop_options