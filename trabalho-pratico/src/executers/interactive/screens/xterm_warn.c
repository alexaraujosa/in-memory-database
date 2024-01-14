#include "executers/interactive/interactive_common.h"
#include "executers/interactive/screens/xterm_warn.h"

Cache make_cache_xterm_warn(GM_Term term, FrameStore store) {
    Cache cache = make_cache(NULL);
    GM_TERM_SIZE size = gm_term_get_size(term);
    
    char* text = get_localized_string(store->current_locale, LOCALE_SCREEN_XTERM_NOT_SUPPORTED);
    int len = strlen(text);
    Tokens lines = get_lines(text, len);

    int max_col = 0;
    for (int i = 0; i < lines->len; i++) {
        int len = strlen(lines->data[i]);
        if (max_col < len) max_col = len; 
    }

    int x = (size.cols - max_col) / 2;
    int y = (size.rows - lines->len - 1) / 2;

    destroy_tokens(lines);
    
    DrawText dt = make_draw_text(text, len, x, y, lines->len, max_col);
    // memoize_cache_elem(cache, LOCALE_SCREEN_XTERM_NOT_SUPPORTED, dt);
    add_cache_elem(cache, LOCALE_SCREEN_XTERM_NOT_SUPPORTED, dt);

    return cache;
}

void destroy_cache_xterm_warn(Cache cache) {
    DrawText dt = get_cache_elem(cache, LOCALE_SCREEN_XTERM_NOT_SUPPORTED);
    destroy_draw_text(dt);
}

void draw_xterm_warn(GM_Term term, FrameStore store, Cache cache) {
    GM_TERM_SIZE size = gm_term_get_size(term);

    gm_attroff(term, GM_RESET);

    gm_box(term, 0, 0, size.rows - 1, size.cols - 2);
    
    // char* text = "          The current terminal is not supported.         \n"
    //              "There is no guarantee this program will work as expected.\n"
    //              "                Press any key to continue.               ";
    // int lines = 3;
    // int len = 59;

    // int x = (size.cols - len) / 2;
    // int y = (size.rows - lines - 1) / 2;


    
    // char* text = get_localized_string(store->current_locale, LOCALE_SCREEN_XTERM_NOT_SUPPORTED);
    // Tokens lines = get_lines(text, strlen(text));

    // int max_col = 0;
    // for (int i = 0; i < lines->len; i++) {
    //     int len = strlen(lines->data[i]);
    //     if (max_col < len) max_col = len; 
    // }

    // int x = (size.cols - max_col) / 2;
    // int y = (size.rows - lines->len - 1) / 2;

    DrawText dl = get_cache_elem(cache, LOCALE_SCREEN_XTERM_NOT_SUPPORTED);

    gm_printf(term, dl->y, dl->x, dl->text);
}
