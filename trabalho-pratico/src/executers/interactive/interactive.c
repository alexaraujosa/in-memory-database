#include "executers/interactive/interactive.h"
#include "executers/interactive/screens/screens.h"

#include <time.h>
#include <stdlib.h>

// // ============================ SCREEN INCLUDES ===========================
// #include "executers/interactive/screens/xterm_warn.h"

#pragma GCC push_options
#pragma GCC optimize ("O0")

int make_frame(GM_Term term, FrameStore store);
int build_frame(GM_Term term, FrameStore store);

// ============================ ENTRY POINT ===========================

void interactive(DataLocales locales) {
    // ======= Set Locale =======
    setlocale(LC_ALL, "en_US.UTF-8");
    setenv("TZ", "", 1);

    // ======= Initialize Terminal =======
    GM_Term term = gm_term_init();

    // ------- Initialize Color Pairs -------
    gm_init_color_defaults(term);

    // gm_init_color(term, 2, 255, 0 , 0);
    // gm_init_color(term, 3, 0, 255 , 0);
    // gm_init_color(term, 4, 0, 0 , 255);

    // gm_init_color_pair(term, 1, 2, 3);
    // gm_init_color_pair(term, 2, 2, 4);
    // gm_init_color_pair(term, 3, 3, 4);

    gm_init_color(term, COLOR_WHITE, 255, 255, 255);
    gm_init_color(term, COLOR_BLACK, 0, 0, 0);

    gm_init_color_pair(term, COLORPAIR_SELECTED, COLOR_WHITE, COLOR_BLACK);

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

    // ------ Read settings file -------
    store.settings = read_data_settings();

    // ------ Load locales -------
    store.locales = locales;
    // store.current_locale = get_locale(locales, "en_US"); // TODO: Load from settings
    char* cur_loc_id = ds_get_locale(store.settings);
    store.current_locale = get_locale(locales, cur_loc_id);
    free(cur_loc_id);

    store.screen_caches = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, NULL);

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

    // ======= Destroy FrameStore =======
    destroy_screens(term, &store);
    destroy_data_settings(store.settings);

    // ======= Destroy Terminal =======
    gm_term_end(term);
}

// ============================ MAKE FRAME ===========================

int make_frame(GM_Term term, FrameStore store) {
    if (store->is_XTerm == NOT_XTERM_UNCONFIRMED) {
        // draw_xterm_warn(term, store);
        manage_screen(SCREEN_XTERM_WARN, term, store);
        return 0;
    }

    manage_screen(SCREEN_SETTINGS, term, store);

    // gm_printf(term, 0, 0, "Hello there.");

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

    keypress_screen(SCREEN_SETTINGS, term, store, key);

    return KEY_RECIEVED;
}

int build_frame(GM_Term term, FrameStore store) {
    Keypress_Code keypress = 0;
    if (gm_kbhit(term)) keypress = handle_keypresses(term, store);

    if (keypress == KEY_ABORT) return 1;

    return 0;
}

#pragma GCC pop_options