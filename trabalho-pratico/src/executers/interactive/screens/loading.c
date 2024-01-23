#include "executers/interactive/interactive_common.h"
#include "executers/interactive/screens/loading.h"

#undef lines

Cache make_cache_loading(GM_Term term, FrameStore store) {
    Cache cache = make_cache(NULL);
    GM_TERM_SIZE size = gm_term_get_size(term);
    
    char* text = get_localized_string(store->current_locale, LOCALE_SCREEN_LOADING_TITLE);
    int len = strlen(text);
    Tokens lines = get_lines(text, len);

    int max_col = 0;
    for (int i = 0; i < lines->len; i++) {
        int len = strlen(lines->data[i]);
        if (max_col < len) max_col = len; 
    }

    int x = (size.cols - max_col) / 2;
    int y = (size.rows - lines->len - 1) / 2;
    
    DrawText dt = make_draw_text(text, len, x, y, lines->len, max_col);
    add_cache_elem(cache, LOCALE_SCREEN_LOADING_TITLE, dt);

    destroy_tokens(lines);

    return cache;
}

void destroy_cache_loading(Cache cache, int force) {
    IGNORE_ARG(force);

    DrawText dt = get_cache_elem(cache, LOCALE_SCREEN_LOADING_TITLE);
    destroy_draw_text(dt);
}

void draw_loading(GM_Term term, FrameStore store, Cache cache) {
    IGNORE_ARG(store);

    gm_attroff(term, GM_RESET);

    DrawText dl = get_cache_elem(cache, LOCALE_SCREEN_LOADING_TITLE);

    gm_printf(term, dl->y, dl->x, dl->text);
}

Keypress_Code keypress_loading(GM_Term term, FrameStore store, Cache cache, GM_Key key) {
    IGNORE_ARG(term);
    IGNORE_ARG(store);
    IGNORE_ARG(cache);
    IGNORE_ARG(key);

    return KEY_SKIP;
}
