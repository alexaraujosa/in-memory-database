#include "executers/interactive/interactive.h"
#include "executers/interactive/screens/screens.h"
#include "executers/interactive/screens/query_output.h"

#include <time.h>
#include <stdlib.h>

// // ============================ SCREEN INCLUDES ===========================
// #include "executers/interactive/screens/xterm_warn.h"

#pragma GCC push_options
#pragma GCC optimize ("O2")

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
    gm_init_color(term, COLOR_RED, 255, 0, 0);
    gm_init_color(term, COLOR_GREY1, 97, 97, 97);

    gm_init_color_pair(term, COLORPAIR_SELECTED, COLOR_WHITE, COLOR_BLACK);
    gm_init_color_pair(term, COLORPAIR_ERROR, COLOR_RED, COLOR_DEFAULT_BG);
    gm_init_color_pair(term, COLORPAIR_DISABLED, COLOR_GREY1, COLOR_DEFAULT_BG);

    // ------- Initialize Screen -------
    gm_hide_cursor(term, TRUE);
    enable_bracketed_paste();

    // ------- Initialize Frame Store -------
    FRAME_STORE store = {
        .awaiting_keypress = 0,
        .is_XTerm = 0,
        .locales = NULL,
        .current_locale = NULL,
        .screen_caches = NULL,
        .settings = NULL,
        .current_screen = 0,
        .datasets = NULL,
        .defer_control = NULL,
        .current_query = NULL,
        .query_pages = NULL,
        .load_query = FALSE
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

    // ------- Initialize screen cache storage -------
    store.screen_caches = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, NULL);

    // ------- Set default menu -------
    store.current_screen = SCREEN_MAIN_MENU;

    // ------- Initialize datasets -------
    store.datasets = make_dataset_data(NULL);

    // ------- Initialize Task Defer Control -------
    store.defer_control = make_defer_control();

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
    destroy_dataset_data(store.datasets);
    destroy_defer_control(store.defer_control);
    // if (store.current_query != NULL) free(store.current_query);
    if (store.current_query != NULL) destroy_query(store.current_query);
    if (store.query_pages != NULL) destroy_query_page_cache(&store, TRUE);

    // ======= Destroy Terminal =======
    disable_bracketed_paste();
    gm_term_end(term);
}

// ============================ MAKE FRAME ===========================

int make_frame(GM_Term term, FrameStore store) {
    if (store->is_XTerm == NOT_XTERM_UNCONFIRMED) {
        // draw_xterm_warn(term, store);
        manage_screen(SCREEN_XTERM_WARN, term, store);
        return 0;
    }

    // manage_screen(SCREEN_SETTINGS, term, store);
    // manage_screen(SCREEN_MAIN_MENU, term, store);
    
    manage_screen(store->current_screen, term, store);

    // gm_printf(term, 0, 0, "Hello there.");

    return 0;
}

// ============================ BUILD FRAME ===========================

Keypress_Code handle_keypresses(GM_Term term, FrameStore store) {
    GM_Key key = gm_get_key(term);

    if (gm_get_canonical_key(key) == GM_KEY_CTRL_C) return KEY_ABORT;

    if (store->is_XTerm == NOT_XTERM_UNCONFIRMED) {
        store->is_XTerm = NOT_XTERM_CONFIRMED;
    }

    // keypress_screen(SCREEN_SETTINGS, term, store, key);
    // keypress_screen(SCREEN_MAIN_MENU, term, store, key);
    
    return keypress_screen(store->current_screen, term, store, key);

    return KEY_RECIEVED;
}

int build_frame(GM_Term term, FrameStore store) {
    // Handle deferred work before starting new frame.
    defer_try(store->defer_control, term, store);

    Keypress_Code keypress = 0;
    if (gm_kbhit(term)) keypress = handle_keypresses(term, store);

    if (keypress == KEY_ABORT) return 1;

    return 0;
}

#pragma GCC pop_options