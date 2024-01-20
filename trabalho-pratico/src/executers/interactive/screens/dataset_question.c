#include "executers/interactive/screens/dataset_question.h"
#include "executers/interactive/components/textarea.h"

#define QUESTION_OFFSET_Y_OFFSET -2
#define BOX_X_BORDER 2

#define TEXT_AREA_KEY "_text_area"

#pragma GCC push_options
#pragma GCC optimize ("O0")
Cache make_cache_dataset_question(GM_Term term, FrameStore store) {
    GM_TERM_SIZE size = gm_term_get_size(term);

    Cache cache = make_cache(NULL);

    char* header_text = get_localized_string(store->current_locale, LOCALE_SCREEN_DATASET_QUESTION_QUESTION);
    MAX_COLS_AND_ROWS ht_mcar = get_max_rows_and_cols(header_text, strlen(header_text));

    TextArea text_area = make_textarea(
        term, 
        store,
        BOX_X_BORDER,
        ((size.rows - ht_mcar.rows) / 2) + QUESTION_OFFSET_Y_OFFSET,
        size.cols - 2 * BOX_X_BORDER,
        0,
        PATH_MAX,
        COLORPAIR_SELECTED,
        COLORPAIR_DEFAULT,
        header_text
    );
    add_cache_elem(cache, TEXT_AREA_KEY, text_area);

    return cache;
}

void destroy_cache_dataset_question(Cache cache, int force) {
    TextArea text_area = get_cache_elem(cache, TEXT_AREA_KEY);
    destroy_textarea(text_area);

    if (force) destroy_cache(cache, FALSE);
}

void draw_dataset_question(GM_Term term, FrameStore store, Cache cache) {
    TextArea text_area = get_cache_elem(cache, TEXT_AREA_KEY);
    draw_textarea(term, store, text_area);
}

Keypress_Code keypress_dataset_question(GM_Term term, FrameStore store, Cache cache, GM_Key key) {
    TextArea text_area = get_cache_elem(cache, TEXT_AREA_KEY);
    return keypress_textarea(text_area, key);
}
#pragma GCC pop_options
