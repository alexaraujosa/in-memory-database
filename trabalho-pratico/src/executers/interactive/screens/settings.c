#include "executers/interactive/screens/xterm_warn.h"

#define LANGUAGE_NAME_KEY "language_name"
#define RIGHT_OFFSET 2
#define TOP_OFFSET 1

DrawText make_dt_from_str(char* text, GM_TERM_SIZE size, int offsetX, int offsetY) {
    ssize_t text_len = strlen(text);
    MAX_COLS_AND_ROWS text_mcar = get_max_rows_and_cols(text, text_len);
    int x = (size.cols - text_mcar.cols) / 2 + offsetX;
    int y = (size.rows - text_mcar.rows - 1) / 2 + offsetY;

    DrawText text_dt = make_draw_text(text, text_len, x, y, text_mcar.rows, text_mcar.cols);
    return text_dt;
}

Cache make_cache_settings(GM_Term term, FrameStore store) {
    Cache cache = make_cache(NULL);
    GM_TERM_SIZE size = gm_term_get_size(term);

    char* title = get_localized_string(store->current_locale, LOCALE_SCREEN_SETTINGS_TITLE);
    DrawText title_dt = make_dt_from_str(title, size, 0, 0);
    title_dt->x = RIGHT_OFFSET; 
    title_dt->y = TOP_OFFSET;
    add_cache_elem(cache, LOCALE_SCREEN_SETTINGS_TITLE, title_dt);

    char* language = get_localized_string(store->current_locale, LOCALE_SCREEN_SETTINGS_LANGUAGE);
    DrawText language_dt = make_dt_from_str(language, size, 0, 0);
    language_dt->x = RIGHT_OFFSET; 
    language_dt->y = TOP_OFFSET + 2;
    add_cache_elem(cache, LOCALE_SCREEN_SETTINGS_LANGUAGE, language_dt);

    char* language_name = get_locale_name(store->current_locale);
    DrawText language_name_dt = make_dt_from_str(language_name, size, 0, 0);
    language_name_dt->x = RIGHT_OFFSET + 1 + language_dt->cols; 
    language_name_dt->y = TOP_OFFSET + 2;
    add_cache_elem(cache, LANGUAGE_NAME_KEY, language_name_dt);

    return cache;
}

void destroy_cache_settings(Cache cache) {
    DrawText title_dt = get_cache_elem(cache, LOCALE_SCREEN_SETTINGS_TITLE);
    destroy_draw_text(title_dt);

    DrawText language_dt = get_cache_elem(cache, LOCALE_SCREEN_SETTINGS_LANGUAGE);
    destroy_draw_text(language_dt);

    DrawText language_name_dt = get_cache_elem(cache, LANGUAGE_NAME_KEY);
    destroy_draw_text(language_name_dt);
}

void draw_settings(GM_Term term, FrameStore store, Cache cache) {
    GM_TERM_SIZE size = gm_term_get_size(term);

    DrawText title_dt = get_cache_elem(cache, LOCALE_SCREEN_SETTINGS_TITLE);
    DrawText language_dt = get_cache_elem(cache, LOCALE_SCREEN_SETTINGS_LANGUAGE);
    DrawText language_name_dt = get_cache_elem(cache, LANGUAGE_NAME_KEY);

    DrawText dts[] = { title_dt, language_dt, language_name_dt };

    int max_col = 0;
    if (max_col < title_dt->len) max_col = title_dt->len; 
    if (max_col < language_dt->len + language_name_dt->len + 1) max_col = language_dt->len + language_name_dt->len + 1;

    gm_attroff(term, GM_RESET);

    gm_box(term, 0, 0, TOP_OFFSET + 3, RIGHT_OFFSET + max_col);

    for (int i = 0; i < 3; i++) {
        gm_printf(term, dts[i]->y, dts[i]->x, dts[i]->text);
    }
}
