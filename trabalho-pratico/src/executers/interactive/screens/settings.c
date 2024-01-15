// May God have mercy on my soul, for what I'm about to do lays beyond the limits of hell.

#include "executers/interactive/screens/xterm_warn.h"
#include "util/math.h"

#define OPTION_INDEX "_option_index"
#define MAX_OPTION_INDEX 2

#define LANGUAGE_NAME_KEY "language_name"
#define RIGHT_OFFSET 2
#define TOP_OFFSET 1

#pragma GCC push_options
#pragma GCC optimize ("O0")
DrawText make_dt_from_str(char* text, GM_TERM_SIZE size, int offsetX, int offsetY) {
    ssize_t text_len = strlen(text);
    MAX_COLS_AND_ROWS text_mcar = get_max_rows_and_cols(text, text_len);
    // int x = (size.cols - text_mcar.cols) / 2 + offsetX;
    // int y = (size.rows - text_mcar.rows - 1) / 2 + offsetY;
    int x = offsetX;
    int y = offsetY;

    DrawText text_dt = make_draw_text(text, text_len, x, y, text_mcar.rows, text_mcar.cols);
    return text_dt;
}

Cache make_cache_settings(GM_Term term, FrameStore store) {
    Cache cache = make_cache(NULL);
    GM_TERM_SIZE size = gm_term_get_size(term);

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


    // ------- Align Values -------
    DrawText keys[] = { language_dt, fuckery_dt };
    DrawText values[] = { language_name_dt, fuckery2_dt };
    int pairs = 2;

    int max_x = 0;
    for (int i = 0; i < pairs; i++) {
        if (max_x < keys[i]->x + keys[i]->len) max_x = keys[i]->x + keys[i]->cols;
    }

    for (int i = 0; i < pairs; i++) values[i]->x = max_x + 2;

    DrawText* selections = (char**)malloc(pairs * sizeof(DrawText));
    int sel_ind = 0;

    for (int i = 0; i < pairs; i++) {
        int lines = imax(keys[i]->rows, values[i]->rows);
        int xs = keys[i]->x;
        int xf = values[i]->x + values[i]->cols;
        int length = lines * (xf - xs + 1) + 1;

        char* text = (char*)malloc(length * sizeof(char));
        // for (int j = 0; j < length; j++) text[j] = ' ';
        int text_ind = 0;
        for (int j = 0; j < lines; j++) {
            for (int k = 0; k < xf - xs; k++) text[text_ind++] = ' ';
            if (j != lines - 1) text[text_ind++] = '\n';
        }

        text[text_ind] = '\0';

        DrawText dt = make_draw_text(text, length, xs, keys[i]->y, keys[i]->y + lines, xf - xs);
        selections[sel_ind++] = dt;
    }
    add_cache_elem(cache, "_selections", selections);





    // char* title = get_localized_string(store->current_locale, LOCALE_SCREEN_SETTINGS_TITLE);
    // DrawText title_dt = make_dt_from_str(title, size, RIGHT_OFFSET, TOP_OFFSET);

    // char* language = get_localized_string(store->current_locale, LOCALE_SCREEN_SETTINGS_LANGUAGE);
    // DrawText language_dt = make_dt_from_str(language, size, RIGHT_OFFSET, TOP_OFFSET + 2);

    // char* language_name = get_locale_name(store->current_locale);
    // DrawText language_name_dt = make_dt_from_str(language_name, size, 0, 0);

    // char* fuckery = strdup("12345:");
    // DrawText fuckery_dt = make_dt_from_str(fuckery, size, RIGHT_OFFSET, TOP_OFFSET + 3);

    // char* fuckery2 = strdup("67890");
    // DrawText fuckery2_dt = make_dt_from_str(fuckery2, size, 0, 0);

    // // ------- Align Values -------
    // DrawText keys[] = { language_dt, fuckery_dt };
    // DrawText value[] = { language_name_dt, fuckery2_dt };
    // int pairs = 2;

    // int max_x = 0;
    // for (int i = 0; i < pairs; i++) {
    //     if (max_x < keys[i]->x + keys[i]->len) max_x = keys[i]->x + keys[i]->len;
    // }

    // for (int i = 0; i < pairs; i++) {
    //     char* completeText = (char*)malloc(max_x * sizeof(char*));
    //     strcpy(completeText)
    // }



    // ------- Option Select -------
    int* option_index = (int*)malloc(sizeof(int));
    *option_index = 0;
    add_cache_elem(cache, OPTION_INDEX, option_index);

    return cache;
}

void destroy_cache_settings(Cache cache) {
    DrawText title_dt = get_cache_elem(cache, LOCALE_SCREEN_SETTINGS_TITLE);
    destroy_draw_text(title_dt);

    DrawText language_dt = get_cache_elem(cache, LOCALE_SCREEN_SETTINGS_LANGUAGE);
    destroy_draw_text(language_dt);

    DrawText language_name_dt = get_cache_elem(cache, LANGUAGE_NAME_KEY);
    destroy_draw_text(language_name_dt);

    int* option_index = get_cache_elem(cache, OPTION_INDEX);
    free(option_index);

    DrawText* selections = get_cache_elem(cache, "_selections");
    for (int i = 0; i < MAX_OPTION_INDEX; i++) destroy_draw_text(selections[i]);
    free(selections);

    DrawText fuckery = get_cache_elem(cache, "fuckery");
    destroy_draw_text(fuckery);

    DrawText fuckery2 = get_cache_elem(cache, "fuckery2");
    destroy_draw_text(fuckery2);
}

void draw_settings(GM_Term term, FrameStore store, Cache cache) {
    GM_TERM_SIZE size = gm_term_get_size(term);

    int* option_index = get_cache_elem(cache, OPTION_INDEX);

    // ------- Localized Strings -------
    DrawText title_dt = get_cache_elem(cache, LOCALE_SCREEN_SETTINGS_TITLE);
    DrawText language_dt = get_cache_elem(cache, LOCALE_SCREEN_SETTINGS_LANGUAGE);
    DrawText language_name_dt = get_cache_elem(cache, LANGUAGE_NAME_KEY);
    DrawText fuckery = get_cache_elem(cache, "fuckery");
    DrawText fuckery2 = get_cache_elem(cache, "fuckery2");

    DrawText* selections = get_cache_elem(cache, "_selections");

    DrawText dts[] = { title_dt, language_dt, language_name_dt, fuckery, fuckery2 };
    int dts_len = 5;

    // ------- Calculate box width -------
    int max_col = 0;
    // if (max_col < title_dt->len) max_col = title_dt->len; 
    // if (max_col < language_dt->len + language_name_dt->len + 1) max_col = language_dt->len + language_name_dt->len + 1;
    for (int i = 0; i < dts_len; i++) {
        if (max_col < dts[i]->x + dts[i]->len) max_col = dts[i]->x + dts[i]->len;
    }

    gm_attroff(term, GM_RESET);

    // ------- Draw Border Box -------
    gm_box(term, 0, 0, TOP_OFFSET + 4, RIGHT_OFFSET + max_col);

    // ------- Draw Options -------
    for (int i = 0; i < 5; i++) {
        // if (*option_index == (i + 1) / 2 &&  ((i + 1) / 2) == ((i + 2) / 2) && i > 0) {
        if (*option_index == (i + 1) / 2 - 1 && (i + 1) / 2 == (i + 2) / 2) {
            gm_attron(term, GM_COLOR_PAIR(COLORPAIR_SELECTED));

            DrawText selection = selections[(i / 2)];
            gm_printf(term, selection->y, selection->x, selection->text);
        }

        // if (((i + 1) / 2) - 1 == *option_index) gm_attron(term, GM_COLOR_PAIR(COLORPAIR_SELECTED));
        gm_printf(term, dts[i]->y, dts[i]->x, dts[i]->text);
        // if (((i + 1) / 2) - 1 == *option_index) gm_attroff(term, GM_COLOR_PAIR(COLORPAIR_SELECTED));

        if (*option_index == (i + 1) / 2 - 1 && (i + 1) / 2 != (i + 2) / 2) 
            gm_attroff(term, GM_COLOR_PAIR(COLORPAIR_SELECTED));
    }
}

Keypress_Code keypress_settings(GM_Term term, FrameStore store, Cache cache, GM_Key key) {
    GM_Key ckey = gm_get_canonical_key(key);
    int* option_index = get_cache_elem(cache, OPTION_INDEX);

    if (ckey == GM_KEY_ARROW_DOWN) {
        (*option_index)++;

        if (*option_index == MAX_OPTION_INDEX) *option_index = 0;
    }

    if (ckey == GM_KEY_ARROW_UP) {
        (*option_index)--;

        if (*option_index == -1) *option_index = MAX_OPTION_INDEX - 1;
    }
}
#pragma GCC pop_options
