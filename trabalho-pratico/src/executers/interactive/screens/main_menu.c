#include "executers/interactive/screens/main_menu.h"

#define TITLE_OFFSET_Y_OFFSET -2
// #define MAX_OPTION_INDEX 2

#define OPTION_INDEX_KEY "_option_index"

static enum main_menu_options {
    MAIN_MENU_EXEC_QUERIES,
    MAIN_MENU_SETTINGS,
    MAIN_MENU_EXIT,

    // Automagically get the max option index.
    MAX_OPTION_INDEX
};

#pragma GCC push_options
#pragma GCC optimize ("O0")
static DrawText make_dt_from_str(char* text, GM_TERM_SIZE size, int offsetX, int offsetY) {
    IGNORE_ARG(size);

    ssize_t text_len = strlen(text);
    MAX_COLS_AND_ROWS text_mcar = get_max_rows_and_cols(text, text_len);
    int x = ((size.cols - text_mcar.cols + 0) / 2) + offsetX;
    int y = ((size.rows - text_mcar.rows + 0) / 2) + offsetY;

    DrawText text_dt = make_draw_text(text, text_len, x, y, text_mcar.rows, text_mcar.cols);
    return text_dt;
}

Cache make_cache_main_menu(GM_Term term, FrameStore store) {
    IGNORE_ARG(term);
    IGNORE_ARG(store);
    
    GM_TERM_SIZE size = gm_term_get_size(term);
    Cache cache = make_cache(NULL);

    // TITLE
    char* title = get_localized_string(store->current_locale, LOCALE_SCREEN_MAIN_MENU_TITLE);
    DrawText title_dt = make_dt_from_str(title, size, 0, TITLE_OFFSET_Y_OFFSET);
    add_cache_elem(cache, LOCALE_SCREEN_MAIN_MENU_TITLE, title_dt);

    // EXECUTE QUERIES
    char* exec_query = get_localized_string(store->current_locale, LOCALE_SCREEN_MAIN_EXECUTE_QUERY);
    DrawText exec_query_dt = make_dt_from_str(exec_query, size, 0, 1);
    add_cache_elem(cache, LOCALE_SCREEN_MAIN_EXECUTE_QUERY, exec_query_dt);

    // SETTINGS
    char* settings = get_localized_string(store->current_locale, LOCALE_SCREEN_SETTINGS_TITLE_PROPERCASE);
    DrawText settings_dt = make_dt_from_str(settings, size, 0, 2);
    add_cache_elem(cache, LOCALE_SCREEN_SETTINGS_TITLE_PROPERCASE, settings_dt);

    // EXIT
    char* exit = get_localized_string(store->current_locale, LOCALE_EXIT);
    DrawText exit_dt = make_dt_from_str(exit, size, 0, 3);
    add_cache_elem(cache, LOCALE_EXIT, exit_dt);

    int* option_index = (int*)malloc(sizeof(int));
    *option_index = 0;
    add_cache_elem(cache, OPTION_INDEX_KEY, option_index);

    return cache;
}

void destroy_cache_main_menu(Cache cache, int force) {
    IGNORE_ARG(cache);
    IGNORE_ARG(force);

    DrawText title_dt = get_cache_elem(cache, LOCALE_SCREEN_MAIN_MENU_TITLE);
    destroy_draw_text(title_dt);

    DrawText exec_query_dt = get_cache_elem(cache, LOCALE_SCREEN_MAIN_EXECUTE_QUERY);
    destroy_draw_text(exec_query_dt);

    DrawText settings_dt = get_cache_elem(cache, LOCALE_SCREEN_SETTINGS_TITLE_PROPERCASE);
    destroy_draw_text(settings_dt);

    DrawText exit_dt = get_cache_elem(cache, LOCALE_EXIT);
    destroy_draw_text(exit_dt);

    int* option_index = get_cache_elem(cache, OPTION_INDEX_KEY);
    free(option_index);
}

void draw_main_menu(GM_Term term, FrameStore store, Cache cache) {
    IGNORE_ARG(term);
    IGNORE_ARG(store);
    IGNORE_ARG(cache);
    
    GM_TERM_SIZE size = gm_term_get_size(term);

    int* option_index = get_cache_elem(cache, OPTION_INDEX_KEY);

    DrawText title_dt = get_cache_elem(cache, LOCALE_SCREEN_MAIN_MENU_TITLE);
    DrawText exec_query_dt = get_cache_elem(cache, LOCALE_SCREEN_MAIN_EXECUTE_QUERY);
    DrawText settings_dt = get_cache_elem(cache, LOCALE_SCREEN_SETTINGS_TITLE_PROPERCASE);
    DrawText exit_dt = get_cache_elem(cache, LOCALE_EXIT);

    DrawText dts[] = { exec_query_dt, settings_dt, exit_dt };
    int dts_len = 3;

    gm_attroff(term, GM_RESET);

    gm_printf(term, title_dt->y, title_dt->x, title_dt->text);

    // ------- Calculate box width -------
    int max_col = 0;
    for (int i = 0; i < dts_len; i++) {
        if (max_col < dts[i]->x + dts[i]->len) max_col = dts[i]->x + dts[i]->len;
    }
    if (max_col % 2 == 0) max_col--;

    // ------- Draw Border Box -------
    gm_box(term, dts[0]->y - 1, dts[0]->x - 1, dts[0]->y + dts_len, max_col);

    for (int i = 0; i < dts_len; i++) {
        if (*option_index == i) gm_attron(term, GM_COLOR_PAIR(COLORPAIR_SELECTED));
        gm_printf(term, dts[i]->y, dts[i]->x, dts[i]->text);
        if (*option_index == i) gm_attroff(term, GM_COLOR_PAIR(COLORPAIR_SELECTED));
    }
}

Keypress_Code keypress_main_menu(GM_Term term, FrameStore store, Cache cache, GM_Key key) {
    IGNORE_ARG(term);
    IGNORE_ARG(store);
    IGNORE_ARG(cache);
    IGNORE_ARG(key);

    GM_Key ckey = gm_get_canonical_key(key);
    int* option_index = get_cache_elem(cache, OPTION_INDEX_KEY);
 
    if (ckey == GM_KEY_ARROW_DOWN) {
        (*option_index)++;

        if (*option_index == MAX_OPTION_INDEX) *option_index = 0;
    } else if (ckey == GM_KEY_ARROW_UP) {
        (*option_index)--;

        if (*option_index == -1) *option_index = MAX_OPTION_INDEX - 1;
    } else if (ckey == GM_KEY_ENTER) {
        switch (*option_index) {
            case MAIN_MENU_EXEC_QUERIES: {
                store->current_screen = SCREEN_DATASET_QUESTION;
                break;
            }
            case MAIN_MENU_SETTINGS: {
                store->current_screen = SCREEN_SETTINGS;
                break;
            }
            case MAIN_MENU_EXIT: {
                return KEY_ABORT;
                break;
            }
        }
    }

    return KEY_RECIEVED;
}
#pragma GCC pop_options
