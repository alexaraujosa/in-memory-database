#include "executers/interactive/screens/dataset_question.h"
#include "executers/interactive/components/textarea.h"

// TODO: Move to catalogManager, won't because of merge conflicts later down the line.
#define USERS_FILE "users.csv"
#define RESERVATIONS_FILE "reservations.csv"
#define FLIGHTS_FILE "flights.csv"
#define PASSENGERS_FILE "passengers.csv"

#define QUESTION_OFFSET_Y_OFFSET -2
#define BOX_X_BORDER 2
#define TEXT_AREA_X BOX_X_BORDER

#define TEXT_AREA_KEY "_text_area"
#define ERROR_PATH_ENOENT "_error_path_enoent"
#define ERROR_PATH_NOT_A_DIR "_error_path_not_a_dir"
#define ERROR_PATH_NOT_A_DATASET "_error_path_not_a_dataset"
#define HAS_ERROR "_has_error"
#define ERROR_MSG "_error_msg"

#pragma GCC push_options
#pragma GCC optimize ("O0")
Cache make_cache_dataset_question(GM_Term term, FrameStore store) {
    GM_TERM_SIZE size = gm_term_get_size(term);

    Cache cache = make_cache(NULL);

    char* header_text = get_localized_string(store->current_locale, LOCALE_SCREEN_DATASET_QUESTION_QUESTION);
    MAX_COLS_AND_ROWS ht_mcar = get_max_rows_and_cols(header_text, strlen(header_text));

    TextArea text_area = make_textarea(
        /* term */               term, 
        /* store */              store,
        /* x */                  TEXT_AREA_X,
        /* y */                  ((size.rows - ht_mcar.rows) / 2) + QUESTION_OFFSET_Y_OFFSET,
        /* size_cols */          size.cols - 2 * TEXT_AREA_X,
        /* size_rows */          0,
        /* max_text_len */       PATH_MAX,
        /* active_colorpair */   COLORPAIR_SELECTED,
        /* inactive_colorpair */ COLORPAIR_DEFAULT,
        /* header_text */        header_text
    );
    add_cache_elem(cache, TEXT_AREA_KEY, text_area);

    char* error_path_enoent = get_localized_string(
        store->current_locale, 
        LOCALE_SCREEN_DATASET_QUESTION_ERROR_PATH_ENOENT
    );
    add_cache_elem(cache, ERROR_PATH_ENOENT, error_path_enoent);

    char* error_path_not_a_dir = get_localized_string(
        store->current_locale, 
        LOCALE_SCREEN_DATASET_QUESTION_ERROR_PATH_NOT_A_DIR
    );
    add_cache_elem(cache, ERROR_PATH_NOT_A_DIR, error_path_not_a_dir);

    char* error_path_not_a_dataset = get_localized_string_formatted(
        store->current_locale, 
        LOCALE_SCREEN_DATASET_QUESTION_ERROR_PATH_NOT_A_DATASET,
        USERS_FILE,
        RESERVATIONS_FILE,
        FLIGHTS_FILE,
        PASSENGERS_FILE
    );
    add_cache_elem(cache, ERROR_PATH_NOT_A_DATASET, error_path_not_a_dataset);

    int* has_error = (int*)malloc(sizeof(int));
    *has_error = FALSE;
    add_cache_elem(cache, HAS_ERROR, has_error);

    char** error_msg = (char**)malloc(sizeof(char*));
    *error_msg = NULL;
    add_cache_elem(cache, ERROR_MSG, error_msg);

    return cache;
}

void destroy_cache_dataset_question(Cache cache, int force) {
    TextArea text_area = get_cache_elem(cache, TEXT_AREA_KEY);
    destroy_textarea(text_area);

    char* error_path_enoent = get_cache_elem(cache, ERROR_PATH_ENOENT);
    free(error_path_enoent);

    char* error_path_not_a_dir = get_cache_elem(cache, ERROR_PATH_NOT_A_DIR);
    free(error_path_not_a_dir);

    char* error_path_not_a_dataset = get_cache_elem(cache, ERROR_PATH_NOT_A_DATASET);
    free(error_path_not_a_dataset);

    int* has_error = get_cache_elem(cache, HAS_ERROR);
    free(has_error);

    char** error_msg = get_cache_elem(cache, ERROR_MSG);
    free(error_msg);

    if (force) destroy_cache(cache, FALSE);
}

void draw_dataset_question(GM_Term term, FrameStore store, Cache cache) {
    TextArea text_area = get_cache_elem(cache, TEXT_AREA_KEY);
    draw_textarea(term, store, text_area);

    int* has_error = get_cache_elem(cache, HAS_ERROR);
    char** error_msg = get_cache_elem(cache, ERROR_MSG);

    if (*has_error) {
        gm_attron(term, GM_COLOR_PAIR(COLORPAIR_ERROR) | GM_PRINT_OVERFLOW_BREAK);
        gm_printf(
            term, 
            get_textarea_y(text_area) + get_textarea_size_rows(text_area) + 1, 
            TEXT_AREA_X, 
            *error_msg
        );
        gm_attroff(term, GM_COLOR_PAIR(COLORPAIR_ERROR) | GM_PRINT_OVERFLOW_BREAK);
    }
}

// Defer dataset loading to next frame, after loading screen is displayed.
void _load_datasets_defer(GM_Term term, FrameStore store) {
    IGNORE_ARG(term);

    dataset_data_load(store->datasets);
    store->current_screen = SCREEN_MAIN_MENU;
}

// Callback for TextArea input enter
static Keypress_Code _cb(
    GM_Term term, 
    FrameStore store, 
    TextArea text_area, 
    GM_Key key, 
    TextAreaKeypressCallbackUserData user_data
) {
    IGNORE_ARG(term);
    IGNORE_ARG(key);

    Keypress_Code ret = KEY_SKIP;

    Cache cache = (Cache)user_data;

    char* input = get_textarea_input(text_area);
    char* path = resolve_to_cwd(input);
    GArray* files = NULL;

    char** error_msg = get_cache_elem(cache, ERROR_MSG);
    int* has_error = get_cache_elem(cache, HAS_ERROR);

    if (path_exists(path) != PATH_EXISTS) {
        char* error_path_enoent = get_cache_elem(cache, ERROR_PATH_ENOENT);
        *error_msg = error_path_enoent;
        *has_error = TRUE;

        // return KEY_SKIP;
        ret = KEY_SKIP;
        goto cleanup;
    }

    if (path_type(path) != PATH_TYPE_DIR) {
        char* error_path_not_a_dir = get_cache_elem(cache, ERROR_PATH_NOT_A_DIR);
        *error_msg = error_path_not_a_dir;
        *has_error = TRUE;

        // return KEY_SKIP;
        ret = KEY_SKIP;
        goto cleanup;
    }

    files = get_files(path);
    if (files->len > 0) {
        int validDatasets = 0;
        for (unsigned int i = 0; i < files->len; i++) {
            char* file = g_array_index(files, char*, i);
            char* fname = strrchr(file, FS_PATH_SEPARATOR);
            if (fname == NULL) continue; // Sanity check
            fname++; // Skip separator

            if (STRING_EQUAL(USERS_FILE, fname))        validDatasets++;
            if (STRING_EQUAL(RESERVATIONS_FILE, fname)) validDatasets++;
            if (STRING_EQUAL(FLIGHTS_FILE, fname))      validDatasets++;
            if (STRING_EQUAL(PASSENGERS_FILE, fname))   validDatasets++;
        }

        if (validDatasets == 4) {
            dataset_data_set_dir(store->datasets, input);
            store->current_screen = SCREEN_LOADING;
            
            defer_load(store->defer_control, (DeferNotify)_load_datasets_defer);
            ret = KEY_SKIP;
            goto cleanup;
        } else {
            char* error_path_not_a_dataset = get_cache_elem(cache, ERROR_PATH_NOT_A_DATASET);
            *error_msg = error_path_not_a_dataset;
            *has_error = TRUE;

            // return KEY_SKIP;
            ret = KEY_SKIP;
            goto cleanup;
        }
    }

// Forgive me, Lord, for I don't give a fuck.
cleanup:
    if (files != NULL) {
        // Why does it need to be done this way? Because GLib refuses to use logic
        // and I can't be fucked to track down the source code and examine why.
        for (unsigned int i = 0; i < files->len; i++) free(g_array_index(files, char*, i));
        g_array_unref(files);
    }
    free(input);
    free(path);
    return ret;
}
Keypress_Code keypress_dataset_question(GM_Term term, FrameStore store, Cache cache, GM_Key key) {
    TextArea text_area = get_cache_elem(cache, TEXT_AREA_KEY);

    int* has_error = get_cache_elem(cache, HAS_ERROR);
    *has_error = FALSE;

    return keypress_textarea(term, store, text_area, key, _cb, cache);
}
#pragma GCC pop_options
