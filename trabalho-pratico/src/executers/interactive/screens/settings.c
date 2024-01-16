// May God have mercy on my soul, for what I'm about to do lays beyond the limits of hell.

#include "executers/interactive/screens/xterm_warn.h"
#include "data/settings.h"
#include "util/math.h"

#define LANGUAGE_NAME_KEY "language_name"
#define SELECTION_KEY "_selections"
#define PAGE_SELECTION_KEY "_page"
#define SETTINGS_KEY "_data_settings"
#define OPTION_INDEX_KEY "_option_index"
#define LOCALES_KEY "_locales"
#define LOCALE_NAMES_KEY "_locale_names"
#define LOCALE_SELECTION_KEY "_selections_loc"
#define LOC_SEL_OPTION_INDEX_KEY "_loc_sel_option_index"

#define MAX_OPTION_INDEX 2
#define RIGHT_OFFSET 2
#define TOP_OFFSET 1

enum settings_pages {
    SETTINGS_PAGE_MAIN,
    SETTINGS_PAGE_LOC_SEL
};

void _draw_main_box(GM_Term term, FrameStore store, Cache cache);
void _draw_loc_sel(GM_Term term, FrameStore store, Cache cache);

static DrawText make_dt_from_str(char* text, GM_TERM_SIZE size, int offsetX, int offsetY) {
    IGNORE_ARG(size);

    ssize_t text_len = strlen(text);
    MAX_COLS_AND_ROWS text_mcar = get_max_rows_and_cols(text, text_len);
    int x = offsetX;
    int y = offsetY;

    DrawText text_dt = make_draw_text(text, text_len, x, y, text_mcar.rows, text_mcar.cols);
    return text_dt;
}

Cache make_cache_settings(GM_Term term, FrameStore store) {
    Cache cache = make_cache(NULL);
    GM_TERM_SIZE size = gm_term_get_size(term);

    // ------- Page Selection -------
    int* page_selection = (int*)malloc(sizeof(int));
    *page_selection = 0;
    add_cache_elem(cache, PAGE_SELECTION_KEY, page_selection);

    {
        // ------- MAIN PAGE -------
        // TITLE
        char* title = get_localized_string(store->current_locale, LOCALE_SCREEN_SETTINGS_TITLE);
        DrawText title_dt = make_dt_from_str(title, size, 0, 0);
        title_dt->x = RIGHT_OFFSET; 
        title_dt->y = TOP_OFFSET;
        add_cache_elem(cache, LOCALE_SCREEN_SETTINGS_TITLE, title_dt);

        // LANGUAGE KEY
        char* language = get_localized_string(store->current_locale, LOCALE_SCREEN_SETTINGS_LANGUAGE);
        DrawText language_dt = make_dt_from_str(language, size, 0, 0);
        language_dt->x = RIGHT_OFFSET; 
        language_dt->y = TOP_OFFSET + 2;
        add_cache_elem(cache, LOCALE_SCREEN_SETTINGS_LANGUAGE, language_dt);

        // LANGUAGE VALUE
        char* language_name = get_locale_name(store->current_locale);
        DrawText language_name_dt = make_dt_from_str(language_name, size, 0, 0);
        language_name_dt->x = RIGHT_OFFSET + 1 + language_dt->cols; 
        language_name_dt->y = TOP_OFFSET + 2;
        add_cache_elem(cache, LANGUAGE_NAME_KEY, language_name_dt);

        // FUCKERY KEY
        char* fuckery = strdup("12345:");
        DrawText fuckery_dt = make_dt_from_str(fuckery, size, 0, 0);
        fuckery_dt->x = RIGHT_OFFSET; 
        fuckery_dt->y = TOP_OFFSET + 3;
        add_cache_elem(cache, "fuckery", fuckery_dt);

        // FUCKERY VALUE
        char* fuckery2 = strdup("67890");
        DrawText fuckery2_dt = make_dt_from_str(fuckery2, size, 0, 0);
        fuckery2_dt->x = RIGHT_OFFSET + 1 + fuckery_dt->cols; 
        fuckery2_dt->y = TOP_OFFSET + 3;
        add_cache_elem(cache, "fuckery2", fuckery2_dt);


        // Align Values
        DrawText keys[] = { language_dt, fuckery_dt };
        DrawText values[] = { language_name_dt, fuckery2_dt };
        int pairs = 2;

        int max_x = 0;
        for (int i = 0; i < pairs; i++) {
            if (max_x < keys[i]->x + keys[i]->len) max_x = keys[i]->x + keys[i]->cols;
        }

        for (int i = 0; i < pairs; i++) values[i]->x = max_x + 2;

        DrawText* selections = (DrawText*)malloc(pairs * sizeof(DrawText));
        int sel_ind = 0;

        for (int i = 0; i < pairs; i++) {
            int lines = imax(keys[i]->rows, values[i]->rows);
            int xs = keys[i]->x;
            int xf = values[i]->x + values[i]->cols;
            int length = lines * (xf - xs + 1) + 1;

            char* text = (char*)malloc(length * sizeof(char));
            int text_ind = 0;
            for (int j = 0; j < lines; j++) {
                for (int k = 0; k < xf - xs; k++) text[text_ind++] = ' ';
                if (j != lines - 1) text[text_ind++] = '\n';
            }

            text[text_ind] = '\0';

            DrawText dt = make_draw_text(text, length, xs, keys[i]->y, keys[i]->y + lines, xf - xs);
            selections[sel_ind++] = dt;
        }
        add_cache_elem(cache, SELECTION_KEY, selections);

        // Option Select
        int* option_index = (int*)malloc(sizeof(int));
        *option_index = 0;
        add_cache_elem(cache, OPTION_INDEX_KEY, option_index);
    }
    
    // ------- Language Selection Screen -------
    {
        // Locales
        GArray* locales = get_all_locales(store->locales);
        add_cache_elem(cache, LOCALES_KEY, locales);

        char** locnames = (char**)malloc(locales->len * sizeof(char*));
        for (ssize_t i = 0; i < locales->len; i++) locnames[i] = get_locale_name(g_array_index(locales, DataLocale, i));
        add_cache_elem(cache, LOCALE_NAMES_KEY, locnames);

        int max_col = 0;
        for (ssize_t i = 0; i < locales->len; i++) {
            int len = strlen(locnames[i]);
            if (max_col < len) max_col = len;
        }

        int x = (size.cols - max_col) / 2;
        int y = (size.rows - locales->len - 1) / 2;

        DrawText* selections = (DrawText*)malloc(locales->len * sizeof(DrawText));
        int sel_ind = 0;

        for (ssize_t i = 0; i < locales->len; i++) {
            int len = strlen(locnames[i]);
            char* text = (char*)malloc((len + 1) * sizeof(char));
            int text_ind = 0;
            for (int j = 0; j < len; j++) text[text_ind++] = ' ';

            text[text_ind] = '\0';

            DrawText dt = make_draw_text(text, len, x, y + i, 1, len);
            selections[sel_ind++] = dt;
        }
        add_cache_elem(cache, LOCALE_SELECTION_KEY, selections);

        // Option Select
        ssize_t* loc_sel_option_index = (ssize_t*)malloc(sizeof(ssize_t));
        *loc_sel_option_index = 0;
        add_cache_elem(cache, LOC_SEL_OPTION_INDEX_KEY, loc_sel_option_index);
    }

    return cache;
}

void destroy_cache_settings(Cache cache, int force) {
    // ------- Page Selection -------
    int* page_selection = get_cache_elem(cache, PAGE_SELECTION_KEY);
    free(page_selection);

    // ------- MAIN PAGE -------
    {
        DrawText title_dt = get_cache_elem(cache, LOCALE_SCREEN_SETTINGS_TITLE);
        destroy_draw_text(title_dt);

        DrawText language_dt = get_cache_elem(cache, LOCALE_SCREEN_SETTINGS_LANGUAGE);
        destroy_draw_text(language_dt);

        DrawText language_name_dt = get_cache_elem(cache, LANGUAGE_NAME_KEY);
        destroy_draw_text(language_name_dt);

        int* option_index = get_cache_elem(cache, OPTION_INDEX_KEY);
        free(option_index);

        DrawText* selections = get_cache_elem(cache, SELECTION_KEY);
        for (int i = 0; i < MAX_OPTION_INDEX; i++) destroy_draw_text(selections[i]);
        free(selections);

        DrawText fuckery = get_cache_elem(cache, "fuckery");
        destroy_draw_text(fuckery);

        DrawText fuckery2 = get_cache_elem(cache, "fuckery2");
        destroy_draw_text(fuckery2);
    }
    
    // ------- Language Selection Screen -------
    {
        int* option_index = get_cache_elem(cache, LOC_SEL_OPTION_INDEX_KEY);
        free(option_index);
        
        // Not freed immediately because it's len is required.
        GArray* locs = get_cache_elem(cache, LOCALES_KEY);

        char** loc_names = get_cache_elem(cache, LOCALE_NAMES_KEY);
        for (ssize_t i = 0; i < locs->len; i++) free(loc_names[i]);
        free(loc_names);

        DrawText* selections = get_cache_elem(cache, LOCALE_SELECTION_KEY);
        for (ssize_t i = 0; i < locs->len; i++) destroy_draw_text(selections[i]);
        free(selections);
        
        g_array_free(locs, TRUE);
    }

    if (force) {
        destroy_cache(cache, FALSE);
    }
}

void draw_settings(GM_Term term, FrameStore store, Cache cache) {
    int* page_selection = get_cache_elem(cache, PAGE_SELECTION_KEY);

    _draw_main_box(term, store, cache);

    switch (*page_selection) {
        // case SETTINGS_PAGE_MAIN: {
        //     _draw_main_box(term, store, cache);
        //     break;
        // }
        case SETTINGS_PAGE_LOC_SEL: {
            _draw_loc_sel(term, store, cache);
        }
    }
}

Keypress_Code keypress_settings(GM_Term term, FrameStore store, Cache cache, GM_Key key) {
    IGNORE_ARG(term);

    GM_Key ckey = gm_get_canonical_key(key);

    int* page_selection = get_cache_elem(cache, PAGE_SELECTION_KEY);
    switch (*page_selection) {
        case SETTINGS_PAGE_MAIN: {
            int* option_index = get_cache_elem(cache, OPTION_INDEX_KEY);

            if (ckey == GM_KEY_ESCAPE) {
                store->current_screen = SCREEN_MAIN_MENU;
            } else if (ckey == GM_KEY_ARROW_DOWN) {
                (*option_index)++;

                if (*option_index == MAX_OPTION_INDEX) *option_index = 0;
            } else if (ckey == GM_KEY_ARROW_UP) {
                (*option_index)--;

                if (*option_index == -1) *option_index = MAX_OPTION_INDEX - 1;
            } else if (ckey == GM_KEY_ENTER) {
                switch (*option_index) {
                    case 0: {
                        *page_selection = SETTINGS_PAGE_LOC_SEL;
                        break;
                    }
                }
            }

            return KEY_RECIEVED;
            break;
        }
        case SETTINGS_PAGE_LOC_SEL: {
            ssize_t* option_index = get_cache_elem(cache, LOC_SEL_OPTION_INDEX_KEY);
            GArray* locs = get_cache_elem(cache, LOCALES_KEY);

            if (ckey == GM_KEY_ESCAPE) {
                *page_selection = SETTINGS_PAGE_MAIN;
                break;
            } else if (ckey == GM_KEY_ARROW_DOWN) {
                (*option_index)++;

                if (*option_index == locs->len) *option_index = 0;
            } else if (ckey == GM_KEY_ARROW_UP) {
                (*option_index)--;

                if (*option_index == -1) *option_index = locs->len - 1;
            } else if (ckey == GM_KEY_ENTER) {
                if (store->current_locale == g_array_index(locs, DataLocale, *option_index)) {
                    *page_selection = SETTINGS_PAGE_MAIN;
                    break;
                }

                store->current_locale = g_array_index(locs, DataLocale, *option_index);

                char* loc_id = get_locale_id(store->current_locale);
                ds_set_locale(store->settings, loc_id);
                free(loc_id);

                save_data_settings(store->settings);

                // destroy_cache_settings(cache, TRUE);
                // return KEY_ABORT;

                // Invalidate every single fucking cache.
                return KEY_SPECIAL;
            }

            return KEY_RECIEVED;
            break;
        }
        default: {
            // Silently ignore it.
            return KEY_SKIP;
        }
    }
    
    return KEY_SKIP; // STFU GCC
}


void _draw_main_box(GM_Term term, FrameStore store, Cache cache) {
    IGNORE_ARG(store);

    GM_TERM_SIZE size = gm_term_get_size(term);
    IGNORE_ARG(size);

    int* option_index = get_cache_elem(cache, OPTION_INDEX_KEY);

    // ------- Localized Strings -------
    DrawText title_dt = get_cache_elem(cache, LOCALE_SCREEN_SETTINGS_TITLE);
    DrawText language_dt = get_cache_elem(cache, LOCALE_SCREEN_SETTINGS_LANGUAGE);
    DrawText language_name_dt = get_cache_elem(cache, LANGUAGE_NAME_KEY);
    DrawText fuckery = get_cache_elem(cache, "fuckery");
    DrawText fuckery2 = get_cache_elem(cache, "fuckery2");

    DrawText* selections = get_cache_elem(cache, SELECTION_KEY);

    DrawText dts[] = { title_dt, language_dt, language_name_dt, fuckery, fuckery2 };
    int dts_len = 5;

    // ------- Calculate box width -------
    int max_col = 0;
    for (int i = 0; i < dts_len; i++) {
        if (max_col < dts[i]->x + dts[i]->len) max_col = dts[i]->x + dts[i]->len;
    }

    gm_attroff(term, GM_RESET);

    // ------- Draw Border Box -------
    gm_box(term, 0, 0, TOP_OFFSET + 4, RIGHT_OFFSET + max_col);

    // ------- Draw Options -------
    for (int i = 0; i < dts_len; i++) {
        if (*option_index == (i + 1) / 2 - 1 && (i + 1) / 2 == (i + 2) / 2) {
            gm_attron(term, GM_COLOR_PAIR(COLORPAIR_SELECTED));

            DrawText selection = selections[(i / 2)];
            gm_printf(term, selection->y, selection->x, selection->text);
        }

        gm_printf(term, dts[i]->y, dts[i]->x, dts[i]->text);

        if (*option_index == (i + 1) / 2 - 1 && (i + 1) / 2 != (i + 2) / 2) 
            gm_attroff(term, GM_COLOR_PAIR(COLORPAIR_SELECTED));
    }
}

void _draw_loc_sel(GM_Term term, FrameStore store, Cache cache) {
    IGNORE_ARG(store);

    GM_TERM_SIZE size = gm_term_get_size(term);
    IGNORE_ARG(size);

    ssize_t* option_index = get_cache_elem(cache, LOC_SEL_OPTION_INDEX_KEY);
    GArray* locs = get_cache_elem(cache, LOCALES_KEY);
    char** loc_names = get_cache_elem(cache, LOCALE_NAMES_KEY);

    DrawText* selections = get_cache_elem(cache, LOCALE_SELECTION_KEY);

    gm_attroff(term, GM_RESET);

    // ------- Draw Border Box -------
    gm_box(
        term, 
        selections[0]->y - 1, 
        selections[0]->x - 1, 
        selections[locs->len - 1]->y + 1, 
        selections[0]->x + selections[0]->cols
    );

    for (ssize_t i = 0; i < locs->len; i++) {
        if (*option_index == i) gm_attron(term, GM_COLOR_PAIR(COLORPAIR_SELECTED));

        DrawText selection = selections[i];
        gm_printf(term, selection->y, selection->x, selection->text);
        gm_printf(term, selection->y, selection->x, loc_names[i]);

        if (*option_index == i) gm_attroff(term, GM_COLOR_PAIR(COLORPAIR_SELECTED));
    }
}
