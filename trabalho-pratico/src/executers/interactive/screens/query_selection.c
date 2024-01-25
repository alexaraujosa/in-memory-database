// TODO: This shit

#include "executers/interactive/screens/query_selection.h"
#include "executers/interactive/screens/query_output.h"
#include "executers/interactive/components/textarea.h"
#include "queries/queries.h"

#define QUERY_NUM 10
#define QUERY_LIST_START_COL 3
#define QUERY_LIST_START_ROW 2

#define QUERY_ARGS_NUM 4
#define QUERY_TA_NUM 3
#define QUERY_ARGS_SIZE_COLS 40
#define QUERY_ARGS_TEXT_MAX 300
#define QUERY_ARGS_START_COL 2
#define QUERY_ARG1_START_ROW 2
#define QUERY_ARG2_START_ROW 6
#define QUERY_ARG3_START_ROW 10

#define ON_HELP_KEY "on_help"
#define HELP_FILL_KEY "help_fill"
#define HELP_PROMPTS_KEY "help_prompts"
#define HELP_PROMPT_FILLS_KEY "help_prompts_fills"
#define CUR_QUERY_KEY "_cur_query"
#define CUR_FLAG_KEY "_cur_flag"
#define STRINGS_KEY "_strings"
#define ON_QUERY_ARGS_KEY "on_query_args"
#define QUERY_ARG1_TEXT_AREA "_arg1_text_area"
#define QUERY_ARG2_TEXT_AREA "_arg2_text_area"
#define QUERY_ARG3_TEXT_AREA "_arg3_text_area"
#define CUR_QUERY_TEXT_AREA "_cur_query_text_area"
#define HAS_ERROR_KEY "_has_error"
#define ERROR_MSG_KEY "_error_msg"
#define ERROR_MSGS_KEY "_error_msgs"

#define STRINGS_QUERY_SELECT_TITLE 0
#define STRINGS_QUERY_1 1
#define STRINGS_QUERY_2 2
#define STRINGS_QUERY_3 3
#define STRINGS_QUERY_4 4
#define STRINGS_QUERY_5 5
#define STRINGS_QUERY_6 6
#define STRINGS_QUERY_7 7
#define STRINGS_QUERY_8 8
#define STRINGS_QUERY_9 9
#define STRINGS_QUERY_10 10
#define STRINGS_ARGS_1 11
#define STRINGS_ARGS_2 12
#define STRINGS_ARGS_3 13
#define STRINGS_ACTIVE 14
#define STRINGS_INACTIVE 15
#define STRINGS_PRETTY_PRINT 16
#define _STRINGS_LEN 16

// Forward declarations
static void draw_help(GM_Term term, FrameStore store, Cache cache);
static void draw_query_help(GM_Term term, FrameStore store, Cache cache);
static void make_query_help_prompts(GM_Term term, FrameStore store, Cache cache);
static void destroy_query_help_prompts(Cache cache);
static void draw_query_arg(GM_Term term, FrameStore store, Cache cache);
static TextArea get_textarea(Cache cache, int ind);

#pragma GCC push_options
#pragma GCC optimize ("O0")

Cache make_cache_query_selection(GM_Term term, FrameStore store) {
    IGNORE_ARG(term);
    IGNORE_ARG(store);
    
    GM_TERM_SIZE size = gm_term_get_size(term);

    Cache cache = make_cache(NULL);

    // ----- Help Tooltip -----
    // Top corner tooltip
    char* help_tooltip_text = get_localized_string(store->current_locale, LOCALE_SCREEN_HELP_TOOLTIP);
    int help_tooltip_len = strlen(help_tooltip_text);
    MAX_COLS_AND_ROWS ht_mcar = get_max_rows_and_cols(help_tooltip_text, help_tooltip_len);
    DrawText help_tooltip_dt = make_draw_text(
        help_tooltip_text, 
        help_tooltip_len, 
        size.cols - help_tooltip_len - 1, 
        0,
        ht_mcar.rows,
        ht_mcar.cols
    );
    add_cache_elem(cache, LOCALE_SCREEN_HELP_TOOLTIP, help_tooltip_dt);
    
    // Textbox
    char* help_text = get_localized_string(store->current_locale, LOCALE_SCREEN_QUERY_SELECTION_HELP);
    int help_len = strlen(help_text);
    MAX_COLS_AND_ROWS h_mcar = get_max_rows_and_cols(help_text, help_len);
    DrawText help_dt = make_draw_text(
        help_text, 
        help_len, 
        (size.cols / 2) - (h_mcar.cols / 2), 
        (size.rows / 2) - (h_mcar.rows / 2), 
        h_mcar.rows,
        h_mcar.cols
    );
    add_cache_elem(cache, LOCALE_SCREEN_QUERY_SELECTION_HELP, help_dt);

    // Empty space fill
    char* help_fill = (char*)malloc(((h_mcar.rows + 1) * h_mcar.cols + 1) * sizeof(char));
    memset(help_fill, 0, (h_mcar.rows * h_mcar.cols + h_mcar.rows));

    int hf_ind = 0;
    for (int i = 0; i < h_mcar.rows; i++) {
        for (int j = 0; j < h_mcar.cols; j++) {
            help_fill[hf_ind++] = ' ';
        }
        help_fill[hf_ind++] = '\n';
    }
    help_fill[hf_ind] = '\0';
    add_cache_elem(cache, HELP_FILL_KEY, help_fill);

    // Load help prompts for each query.
    make_query_help_prompts(term, store, cache);

    // On help control
    int* on_help = (int*)malloc(sizeof(int));
    *on_help = FALSE;
    add_cache_elem(cache, ON_HELP_KEY, on_help);

    // ----- Current query selection -----
    int* cur_query = (int*)malloc(sizeof(int));
    *cur_query = 0;
    add_cache_elem(cache, CUR_QUERY_KEY, cur_query);

    char* cur_flag = (char*)malloc(sizeof(char));
    *cur_flag = 0;
    add_cache_elem(cache, CUR_FLAG_KEY, cur_flag);

    // ----- String storage -----
    char** strings = (char**)malloc(_STRINGS_LEN * sizeof(char*));
    // strings[STRINGS_QUERY_SELECT_TITLE] = strdup("Select query:");
    // strings[STRINGS_QUERY_1]  = strdup("1  - List summary.");
    // strings[STRINGS_QUERY_2]  = strdup("2  - List user reservations/flights.");
    // strings[STRINGS_QUERY_3]  = strdup("3  - List hotel rating.");
    // strings[STRINGS_QUERY_4]  = strdup("4  - List hotel reservations.");
    // strings[STRINGS_QUERY_5]  = strdup("5  - List flights from origin between dates A and B.");
    // strings[STRINGS_QUERY_6]  = strdup("6  - List top N airports with most passengers.");
    // strings[STRINGS_QUERY_7]  = strdup("7  - List top N airports with greatest delays.");
    // strings[STRINGS_QUERY_8]  = strdup("8  - List hotel revenue between dates A and B.");
    // strings[STRINGS_QUERY_9]  = strdup("9  - List users with name starting with prefix.");
    // strings[STRINGS_QUERY_10] = strdup("10 - List general metrics.");
    // strings[STRINGS_ARGS_1] = strdup("Argument 1:");
    // strings[STRINGS_ARGS_2] = strdup("Argument 2:");
    // strings[STRINGS_ARGS_3] = strdup("Argument 3:");
    strings[STRINGS_QUERY_SELECT_TITLE] = get_localized_string(store->current_locale, LOCALE_SCREEN_QUERY_SELECTION_TITLE);
    strings[STRINGS_QUERY_1]  = get_localized_string(store->current_locale, LOCALE_SCREEN_QUERY_SELECTION_QUERY1);
    strings[STRINGS_QUERY_2]  = get_localized_string(store->current_locale, LOCALE_SCREEN_QUERY_SELECTION_QUERY2);
    strings[STRINGS_QUERY_3]  = get_localized_string(store->current_locale, LOCALE_SCREEN_QUERY_SELECTION_QUERY3);
    strings[STRINGS_QUERY_4]  = get_localized_string(store->current_locale, LOCALE_SCREEN_QUERY_SELECTION_QUERY4);
    strings[STRINGS_QUERY_5]  = get_localized_string(store->current_locale, LOCALE_SCREEN_QUERY_SELECTION_QUERY5);
    strings[STRINGS_QUERY_6]  = get_localized_string(store->current_locale, LOCALE_SCREEN_QUERY_SELECTION_QUERY6);
    strings[STRINGS_QUERY_7]  = get_localized_string(store->current_locale, LOCALE_SCREEN_QUERY_SELECTION_QUERY7);
    strings[STRINGS_QUERY_8]  = get_localized_string(store->current_locale, LOCALE_SCREEN_QUERY_SELECTION_QUERY8);
    strings[STRINGS_QUERY_9]  = get_localized_string(store->current_locale, LOCALE_SCREEN_QUERY_SELECTION_QUERY9);
    strings[STRINGS_QUERY_10] = get_localized_string(store->current_locale, LOCALE_SCREEN_QUERY_SELECTION_QUERY10);
    strings[STRINGS_ARGS_1]   = get_localized_string(store->current_locale, LOCALE_SCREEN_QUERY_SELECTION_ARG1);
    strings[STRINGS_ARGS_2]   = get_localized_string(store->current_locale, LOCALE_SCREEN_QUERY_SELECTION_ARG2);
    strings[STRINGS_ARGS_3]   = get_localized_string(store->current_locale, LOCALE_SCREEN_QUERY_SELECTION_ARG3);
    strings[STRINGS_ACTIVE]   = get_localized_string(store->current_locale, LOCALE_ACTIVE_UPPERCASE);
    strings[STRINGS_INACTIVE] = get_localized_string(store->current_locale, LOCALE_INACTIVE_UPPERCASE);
    strings[STRINGS_PRETTY_PRINT] = get_localized_string(store->current_locale, LOCALE_SCREEN_QUERY_SELECTION_PRETTY_FORMAT);
    add_cache_elem(cache, STRINGS_KEY, strings);

    // ======= ARGUMENT TEXTAREAS ======
    TextArea ta_arg1 = make_textarea(
        /* term */               term, 
        /* store */              store,
        /* x */                  QUERY_ARGS_START_COL,
        /* y */                  QUERY_ARG1_START_ROW,
        /* size_cols */          QUERY_ARGS_SIZE_COLS,
        /* size_rows */          0,
        /* max_text_len */       QUERY_ARGS_TEXT_MAX,
        /* active_colorpair */   COLORPAIR_SELECTED,
        /* inactive_colorpair */ COLORPAIR_DEFAULT,
        /* header_text */        strdup(strings[STRINGS_ARGS_1])
    );
    add_cache_elem(cache, QUERY_ARG1_TEXT_AREA, ta_arg1);

    TextArea ta_arg2 = make_textarea(
        /* term */               term, 
        /* store */              store,
        /* x */                  QUERY_ARGS_START_COL,
        /* y */                  QUERY_ARG2_START_ROW,
        /* size_cols */          QUERY_ARGS_SIZE_COLS,
        /* size_rows */          0,
        /* max_text_len */       QUERY_ARGS_TEXT_MAX,
        /* active_colorpair */   COLORPAIR_SELECTED,
        /* inactive_colorpair */ COLORPAIR_DEFAULT,
        /* header_text */        strdup(strings[STRINGS_ARGS_2])
    );
    set_textarea_active(ta_arg2, 0);
    add_cache_elem(cache, QUERY_ARG2_TEXT_AREA, ta_arg2);

    TextArea ta_arg3 = make_textarea(
        /* term */               term, 
        /* store */              store,
        /* x */                  QUERY_ARGS_START_COL,
        /* y */                  QUERY_ARG3_START_ROW,
        /* size_cols */          QUERY_ARGS_SIZE_COLS,
        /* size_rows */          0,
        /* max_text_len */       QUERY_ARGS_TEXT_MAX,
        /* active_colorpair */   COLORPAIR_SELECTED,
        /* inactive_colorpair */ COLORPAIR_DEFAULT,
        /* header_text */        strdup(strings[STRINGS_ARGS_3])
    );
    set_textarea_active(ta_arg3, 0);
    add_cache_elem(cache, QUERY_ARG3_TEXT_AREA, ta_arg3);

    // Current text area
    int* cur_ta = (int*)malloc(sizeof(int));
    *cur_ta = FALSE;
    add_cache_elem(cache, CUR_QUERY_TEXT_AREA, cur_ta);

    // On query argument
    int* on_args = (int*)malloc(sizeof(int));
    *on_args = FALSE;
    add_cache_elem(cache, ON_QUERY_ARGS_KEY, on_args);

    // ======= ERROR DISPLAY =======
    int* has_error = (int*)malloc(sizeof(int));
    *has_error = FALSE;
    add_cache_elem(cache, HAS_ERROR_KEY, has_error);

    char** error_msg = (char**)malloc(sizeof(char*));
    *error_msg = NULL;
    add_cache_elem(cache, ERROR_MSG_KEY, error_msg);

    Cache error_msg_cache = make_cache(free);
    add_cache_elem(cache, ERROR_MSGS_KEY, error_msg_cache);

    return cache;
}

void destroy_cache_query_selection(Cache cache, int force) {
    IGNORE_ARG(force);

    // ----- Help Tooltip -----
    DrawText help_tooltip_dt = get_cache_elem(cache, LOCALE_SCREEN_HELP_TOOLTIP);
    destroy_draw_text(help_tooltip_dt);

    DrawText help_dt = get_cache_elem(cache, LOCALE_SCREEN_QUERY_SELECTION_HELP);
    destroy_draw_text(help_dt);

    destroy_query_help_prompts(cache);

    int* on_help = get_cache_elem(cache, ON_HELP_KEY);
    free(on_help);

    char* help_fill = get_cache_elem(cache, HELP_FILL_KEY);
    free(help_fill);

    // ----- Current query selection -----
    int* cur_query = get_cache_elem(cache, CUR_QUERY_KEY);
    free(cur_query);

    char* cur_flag = get_cache_elem(cache, CUR_FLAG_KEY);
    free(cur_flag);

    // ----- String storage -----
    char** strings = get_cache_elem(cache, STRINGS_KEY);
    for (int i = 0; i < _STRINGS_LEN; i++) free(strings[i]);
    free(strings);

    // ======= ARGUMENT TEXTAREAS ======
    TextArea ta_arg1 = get_cache_elem(cache, QUERY_ARG1_TEXT_AREA);
    destroy_textarea(ta_arg1);

    TextArea ta_arg2 = get_cache_elem(cache, QUERY_ARG2_TEXT_AREA);
    destroy_textarea(ta_arg2);

    TextArea ta_arg3 = get_cache_elem(cache, QUERY_ARG3_TEXT_AREA);
    destroy_textarea(ta_arg3);

    int* cur_ta = get_cache_elem(cache, CUR_QUERY_TEXT_AREA);
    free(cur_ta);

    int* on_args = get_cache_elem(cache, ON_QUERY_ARGS_KEY);
    free(on_args);

    // ======= ERROR DISPLAY =======
    int* has_error = get_cache_elem(cache, HAS_ERROR_KEY);
    free(has_error);

    char** error_msg = get_cache_elem(cache, ERROR_MSG_KEY);
    free(error_msg);

    Cache error_msg_cache = get_cache_elem(cache, ERROR_MSGS_KEY);
    destroy_cache(error_msg_cache, TRUE);
}

void draw_query_selection(GM_Term term, FrameStore store, Cache cache) {
    IGNORE_ARG(term);
    IGNORE_ARG(store);
    IGNORE_ARG(cache);

    DrawText help_tooltip_dt = get_cache_elem(cache, LOCALE_SCREEN_HELP_TOOLTIP);
    int* on_help = get_cache_elem(cache, ON_HELP_KEY);

    int* on_args = get_cache_elem(cache, ON_QUERY_ARGS_KEY);

    if (*on_args) {
        draw_query_arg(term, store, cache);
    } else {
        int* cur_query = get_cache_elem(cache, CUR_QUERY_KEY);
        char** strings = get_cache_elem(cache, STRINGS_KEY);

        gm_printf(term, 1, 1, strings[STRINGS_QUERY_SELECT_TITLE]);

        for (int i = 0; i < QUERY_NUM; i++) {
            if (*cur_query == i) gm_attron(term, GM_COLOR_PAIR(COLORPAIR_SELECTED));
            gm_printf(term, QUERY_LIST_START_ROW + i, QUERY_LIST_START_COL, strings[STRINGS_QUERY_1 + i]);
            if (*cur_query == i) gm_attroff(term, GM_COLOR_PAIR(COLORPAIR_SELECTED));
        }

    }

    // ======= Draw Help Tooltip =======
    gm_printf(term, help_tooltip_dt->y, help_tooltip_dt->x, help_tooltip_dt->text);
    if (*on_help == 1) draw_help(term, store, cache);
    else if (*on_help == 2) draw_query_help(term, store, cache);
}

/**
 * @internal
 * 
 * @brief Callback function for the argument textareas.
 */
static Keypress_Code _cb(
    GM_Term term, 
    FrameStore store, 
    TextArea text_area, 
    GM_Key key, 
    TextAreaKeypressCallbackUserData user_data
) {
    IGNORE_ARG(term);
    IGNORE_ARG(text_area);
    IGNORE_ARG(key);

    Cache cache = (Cache)user_data;

    int* cur_query = get_cache_elem(cache, CUR_QUERY_KEY);
    char* cur_flag = get_cache_elem(cache, CUR_FLAG_KEY);

    int* cur_ta = get_cache_elem(cache, CUR_QUERY_TEXT_AREA);
    TextArea ta_arg1 = get_textarea(cache, 0);
    TextArea ta_arg2 = get_textarea(cache, 1);
    TextArea ta_arg3 = get_textarea(cache, 2);

    char** error_msg = get_cache_elem(cache, ERROR_MSG_KEY);
    int* has_error = get_cache_elem(cache, HAS_ERROR_KEY);
    Cache error_msg_cache = get_cache_elem(cache, ERROR_MSGS_KEY);

    Query query = (Query)malloc(sizeof(QUERY));
    memset(query, 0, sizeof(QUERY));

    snprintf(query->id, sizeof(query->id), "%d", (*cur_query) + 1);
    query->flag = *cur_flag;

    char* arg1 = get_textarea_input(ta_arg1);
    if (!IS_STRING_NULL(arg1)) {
        query->argv[0] = arg1;
        query->argc++;
    }

    char* arg2 = get_textarea_input(ta_arg2);
    if (!IS_STRING_NULL(arg2)) {
        if (query->argc < 1) {
            char* err = NULL;
            if (has_cache_elem(error_msg_cache, LOCALE_QUERIES_ARG2_SKIP)) {
                err = get_cache_elem(error_msg_cache, LOCALE_QUERIES_ARG2_SKIP);
            } else {
                char* nerr = get_localized_string(store->current_locale, LOCALE_QUERIES_ARG2_SKIP);
                set_cache_elem(error_msg_cache, LOCALE_QUERIES_ARG2_SKIP, nerr);
                err = nerr;
            }

            *error_msg = err;
            *has_error = TRUE;

            free(arg1);
            free(arg2);
            query->argc = 0;
            destroy_query(query);

            return KEY_SKIP;
        }
            
        query->argv[1] = arg2;
        query->argc++;
    } else {
        // free(arg2);
    }

    char* arg3 = get_textarea_input(ta_arg3);
    if (!IS_STRING_NULL(arg3)) {
        if (query->argc < 2) {
            char* err = NULL;
            if (has_cache_elem(error_msg_cache, LOCALE_QUERIES_ARG3_SKIP)) {
                err = get_cache_elem(error_msg_cache, LOCALE_QUERIES_ARG3_SKIP);
            } else {
                char* nerr = get_localized_string(store->current_locale, LOCALE_QUERIES_ARG3_SKIP);
                set_cache_elem(error_msg_cache, LOCALE_QUERIES_ARG3_SKIP, nerr);
                err = nerr;
            }

            *error_msg = err;
            *has_error = TRUE;
          
            free(arg1);
            free(arg2);
            free(arg3);
            query->argc = 0;
            destroy_query(query);
            
            return KEY_SKIP;
        }

        query->argv[2] = arg3;
        query->argc++;
    } else {
        // free(arg3);
    }

    char* error;
    if (query_verify_raw(query, store->datasets, &error)) {
        // if (*error_msg != NULL) free(*error_msg);

        char* err = NULL;
        if (has_cache_elem(error_msg_cache, error)) {
            err = get_cache_elem(error_msg_cache, error);
        } else {
            char* nerr = get_localized_string(store->current_locale, error);
            set_cache_elem(error_msg_cache, error, nerr);
            err = nerr;
        }

        *error_msg = err;
        *has_error = TRUE;

        free(error);

        free(arg1);
        free(arg2);
        free(arg3);
        query->argc = 0;
        destroy_query(query);

        return KEY_SKIP;
    }

    *error_msg = NULL;
    *has_error = FALSE;

    *cur_ta = 0;

    if (ds_get_ara(store->settings) == 1) {
        set_textarea_input(ta_arg1, "");
        set_textarea_input(ta_arg2, "");
        set_textarea_input(ta_arg3, "");
        *cur_flag = 0;
    }

    for (int i = query->argc + 1; i <= 3; i++) {
        switch (i) {
            case 1: { free(arg1); break; }
            case 2: { free(arg2); break; }
            case 3: { free(arg3); break; }
        }
    }

    store->current_query = query;

    store->current_screen = SCREEN_QUERY_OUTPUT;
    // invalidate_screen_cache(term, store, SCREEN_QUERY_OUTPUT);
    if (store->query_pages == NULL) make_query_page_cache(store);
    else reset_query_page_cache(store);

    load_query_page_cache(store);

    return KEY_RECIEVED;
}

Keypress_Code keypress_query_selection(GM_Term term, FrameStore store, Cache cache, GM_Key key) {
    IGNORE_ARG(term);
    IGNORE_ARG(store);

    int* on_help = get_cache_elem(cache, ON_HELP_KEY);
    int* cur_query = get_cache_elem(cache, CUR_QUERY_KEY);
    char* cur_flag = get_cache_elem(cache, CUR_FLAG_KEY);
    int* on_args = get_cache_elem(cache, ON_QUERY_ARGS_KEY);
    int* cur_ta = get_cache_elem(cache, CUR_QUERY_TEXT_AREA);

    GM_Key ckey = gm_get_canonical_key(key);
    switch(ckey) {
        case GM_KEY_ESCAPE: {
            if (*on_help) {
                *on_help = 0;
                return KEY_RECIEVED; 
            }

            if (*on_args) {
                *on_args = 0;
                return KEY_RECIEVED; 
            }

            store->current_screen = SCREEN_MAIN_MENU;
            return KEY_RECIEVED;
            break;
        }
        case GM_KEY_F1: {
            // *on_help = !(*on_help);
            if (*on_help) *on_help = 0;
            else *on_help = 1;

            return KEY_RECIEVED;
            break;
        }
        // case GM_KEY_F2: {
        //     if (*on_help) *on_help = 0;
        //     else *on_help = 2;

        //     return KEY_RECIEVED;
        //     break;
        // }
        case GM_KEY_ARROW_UP: {
            if (*on_help) return KEY_SKIP;

            if (*on_args) {
                if (*cur_ta == 0) return KEY_SKIP;
                
                if (*cur_ta < QUERY_ARGS_NUM - 1) set_textarea_active(get_textarea(cache, *cur_ta), FALSE);
                (*cur_ta)--;
                set_textarea_active(get_textarea(cache, *cur_ta), TRUE);
            } else {
                if (*cur_query == 0) return KEY_SKIP;

                (*cur_query)--;
            }

            return KEY_RECIEVED;
            break;
        }
        case GM_KEY_ARROW_DOWN: {
            if (*on_help) return KEY_SKIP;

            if (*on_args) {
                if (*cur_ta >= QUERY_ARGS_NUM - 1) {
                    *cur_ta = QUERY_ARGS_NUM - 1;
                    return KEY_SKIP;
                }

                set_textarea_active(get_textarea(cache, *cur_ta), FALSE);
                (*cur_ta)++;
                if (*cur_ta < QUERY_ARGS_NUM - 1) set_textarea_active(get_textarea(cache, *cur_ta), TRUE);
            } else {
                if (*cur_query >= QUERY_NUM - 1) {
                    *cur_query = QUERY_NUM - 1;
                    return KEY_SKIP;
                }

                (*cur_query)++;
            }
            
            return KEY_RECIEVED;
            break;
        }
        case GM_KEY_ENTER: {
            if (*on_help) return KEY_SKIP;

            if (*on_args) {
                if (*cur_ta == 3) {
                    if (*cur_flag == 'F') *cur_flag = 0;
                    else *cur_flag = 'F';

                    return KEY_RECIEVED;
                } else {
                    TextArea ta = get_textarea(cache, *cur_ta);
                    return keypress_textarea(term, store, ta, key, _cb, cache);
                }
            } else {
                *on_args = TRUE;
                if (ds_get_ara(store->settings) == 1) {
                    TextArea ta_arg1 = get_textarea(cache, 0);
                    TextArea ta_arg2 = get_textarea(cache, 1);
                    TextArea ta_arg3 = get_textarea(cache, 2);

                    set_textarea_input(ta_arg1, "");
                    set_textarea_input(ta_arg2, "");
                    set_textarea_input(ta_arg3, "");
                    *cur_flag = 0;
                }
            }

            return KEY_RECIEVED;
            break;
        }
        default: {
            if (*on_help) return KEY_SKIP;
            
            if (*on_args) {
                TextArea ta = get_textarea(cache, *cur_ta);
                return keypress_textarea(term, store, ta, key, _cb, cache);
            } else {
                return KEY_SKIP;
            }
        }
    }

    return KEY_SKIP;
}

// ============== HELPER FUNCTIONS ==============
static TextArea get_textarea(Cache cache, int ind) {
    // int* cur_ta = get_cache_elem(cache, CUR_QUERY_TEXT_AREA);

    switch (ind) {
        case 0: {
            TextArea ta_arg1 = get_cache_elem(cache, QUERY_ARG1_TEXT_AREA);
            return ta_arg1;
            break;
        }
        case 1: {
            TextArea ta_arg2 = get_cache_elem(cache, QUERY_ARG2_TEXT_AREA);
            return ta_arg2;
            break;
        }
        case 2: {
            TextArea ta_arg3 = get_cache_elem(cache, QUERY_ARG3_TEXT_AREA);
            return ta_arg3;
            break;
        }
    }

    return NULL;
}

static void draw_query_arg(GM_Term term, FrameStore store, Cache cache) {
    int* cur_query = get_cache_elem(cache, CUR_QUERY_KEY);
    char* cur_flag = get_cache_elem(cache, CUR_FLAG_KEY);
    char** strings = get_cache_elem(cache, STRINGS_KEY);

    int* cur_ta = get_cache_elem(cache, CUR_QUERY_TEXT_AREA);
    TextArea ta_arg1 = get_cache_elem(cache, QUERY_ARG1_TEXT_AREA);
    TextArea ta_arg2 = get_cache_elem(cache, QUERY_ARG2_TEXT_AREA);
    TextArea ta_arg3 = get_cache_elem(cache, QUERY_ARG3_TEXT_AREA);

    char** error_msg = get_cache_elem(cache, ERROR_MSG_KEY);
    int* has_error = get_cache_elem(cache, HAS_ERROR_KEY);

    gm_printf(term, 0, 0, strings[STRINGS_QUERY_1 + (*cur_query)]);

    // gm_printf(term, QUERY_ARG1_START_ROW - 1, QUERY_ARGS_START_COL, strings[STRINGS_ARGS_1])
    draw_textarea(term, store, ta_arg1);

    draw_textarea(term, store, ta_arg2);

    draw_textarea(term, store, ta_arg3);

    gm_printf(
        term, 
        QUERY_ARG3_START_ROW + get_textarea_size_rows(ta_arg3) + 1, 
        QUERY_ARGS_START_COL,
        strings[STRINGS_PRETTY_PRINT]
    );

    if (*cur_ta == 3) gm_attron(term, GM_COLOR_PAIR(COLORPAIR_SELECTED));
    gm_printf(
        term, 
        QUERY_ARG3_START_ROW + get_textarea_size_rows(ta_arg3) + 1, 
        QUERY_ARGS_START_COL + strlen(strings[STRINGS_PRETTY_PRINT]) + 1,
        (*cur_flag == 'F') ? strings[STRINGS_ACTIVE] : strings[STRINGS_INACTIVE]
    );
    if (*cur_ta == 3) gm_attroff(term, GM_COLOR_PAIR(COLORPAIR_SELECTED));
        
    if (*has_error) {
        gm_attron(term, GM_COLOR_PAIR(COLORPAIR_ERROR) | GM_PRINT_OVERFLOW_BREAK);
        gm_printf(
            term, 
            QUERY_ARG3_START_ROW + get_textarea_size_rows(ta_arg3) + 3, 
            QUERY_ARGS_START_COL, 
            *error_msg
        );
        gm_attroff(term, GM_COLOR_PAIR(COLORPAIR_ERROR) | GM_PRINT_OVERFLOW_BREAK);
    }
}

static void draw_help(GM_Term term, FrameStore store, Cache cache) {
    DrawText help_dt = get_cache_elem(cache, LOCALE_SCREEN_QUERY_SELECTION_HELP);
    char* help_fill = get_cache_elem(cache, HELP_FILL_KEY);

    gm_printf(term, help_dt->y - 1, help_dt->x - 1, help_fill);

    gm_box(
        term, 
        help_dt->y - 1, 
        help_dt->x - 1, 
        help_dt->y + help_dt->rows, 
        help_dt->x + help_dt->cols // + 1
    );

    gm_printf(term, help_dt->y, help_dt->x, help_dt->text);
}

// TODO: MAYBE THIS, CAGA NISSO
static void draw_query_help(GM_Term term, FrameStore store, Cache cache) {
    // int* cur_query = get_cache_elem(cache, CUR_QUERY_KEY);
    // DrawText* help_prompts = get_cache_elem(cache, HELP_PROMPTS_KEY);
    // char** help_prompt_fills = get_cache_elem(cache, HELP_PROMPT_FILLS_KEY);

    // gm_attron(term, GM_PRINT_OVERFLOW_BREAK);
    // gm_printf(term, help_prompts[*cur_query]->y - 1, help_prompts[*cur_query]->x - 1, help_prompt_fills[*cur_query]);

    // gm_box(
    //     term, 
    //     help_prompts[*cur_query]->y - 1, 
    //     help_prompts[*cur_query]->x - 1, 
    //     help_prompts[*cur_query]->y + help_prompts[*cur_query]->rows, 
    //     help_prompts[*cur_query]->x + help_prompts[*cur_query]->cols + 1
    // );

    // gm_printf(term, help_prompts[*cur_query]->y, help_prompts[*cur_query]->x, help_prompts[*cur_query]->text);
    // gm_attroff(term, GM_PRINT_OVERFLOW_BREAK);
}

static void make_query_help_prompts(GM_Term term, FrameStore store, Cache cache) {
    // GM_TERM_SIZE size = gm_term_get_size(term);
    
    // DrawText* help_prompts = (DrawText*)malloc(QUERY_NUM * sizeof(DrawText));
    // memset(help_prompts, 0, QUERY_NUM * sizeof(DrawText));

    // char** help_prompt_fills = (char**)malloc(QUERY_NUM * sizeof(char*));
    // memset(help_prompt_fills, 0, QUERY_NUM * sizeof(char));

    // char* keys[QUERY_NUM] = {
    //     LOCALE_SCREEN_QUERY_SELECTION_HELP_QUERY_1,
    //     LOCALE_SCREEN_QUERY_SELECTION_HELP_QUERY_2,
    //     LOCALE_SCREEN_QUERY_SELECTION_HELP_QUERY_3,
    //     LOCALE_SCREEN_QUERY_SELECTION_HELP_QUERY_4,
    //     LOCALE_SCREEN_QUERY_SELECTION_HELP_QUERY_5,
    //     LOCALE_SCREEN_QUERY_SELECTION_HELP_QUERY_6,
    //     LOCALE_SCREEN_QUERY_SELECTION_HELP_QUERY_7,
    //     LOCALE_SCREEN_QUERY_SELECTION_HELP_QUERY_8,
    //     LOCALE_SCREEN_QUERY_SELECTION_HELP_QUERY_9,
    //     LOCALE_SCREEN_QUERY_SELECTION_HELP_QUERY_10,
    // };

    // for (int i = 0; i < QUERY_NUM; i++) {
    //     char* help_text = get_localized_string(store->current_locale, keys[i]);
    //     int help_len = strlen(help_text);

    //     Tokens lines = get_lines(help_text, help_len);
    //     Tokens nlines = break_lines(lines, size.cols - 4, NULL);

    //     int max_col = 0;
    //     for (int j = 0; j < nlines->len; j++) {
    //         int len = strlen(nlines->data[j]);
    //         if (max_col < len) max_col = len; 
    //     }

    //     MAX_COLS_AND_ROWS h_mcar = (MAX_COLS_AND_ROWS){
    //         .cols = max_col,
    //         .rows = nlines->len
    //     };

    //     // MAX_COLS_AND_ROWS h_mcar = get_max_rows_and_cols(help_text, help_len);
    //     DrawText help_dt = make_draw_text(
    //         help_text, 
    //         help_len, 
    //         (size.cols / 2) - (h_mcar.cols / 2), 
    //         (size.rows / 2) - (h_mcar.rows / 2), 
    //         h_mcar.rows,
    //         h_mcar.cols
    //     );

    //     destroy_tokens(lines);
    //     destroy_tokens(nlines);

    //     char* help_fill = (char*)malloc((h_mcar.rows * h_mcar.cols + h_mcar.rows) * sizeof(char));
    //     memset(help_fill, 0, (h_mcar.rows * h_mcar.cols + h_mcar.rows));

    //     int hf_ind = 0;
    //     for (int i = 0; i < h_mcar.rows; i++) {
    //         for (int j = 0; j < h_mcar.cols; j++) {
    //             help_fill[hf_ind++] = ' ';
    //         }
    //         help_fill[hf_ind++] = '\n';
    //     }
    //     help_fill[hf_ind - 1] = '\0';

    //     help_prompts[i] = help_dt;
    //     help_prompt_fills[i] = help_fill;
    //     // free(help_text);
    // }

    // add_cache_elem(cache, HELP_PROMPTS_KEY, help_prompts);
    // add_cache_elem(cache, HELP_PROMPT_FILLS_KEY, help_prompt_fills);
}

static void destroy_query_help_prompts(Cache cache) {
    // DrawText* help_prompts = get_cache_elem(cache, HELP_PROMPTS_KEY);
    // char** help_prompt_fills = get_cache_elem(cache, HELP_PROMPT_FILLS_KEY);

    // for (int i = 0; i < QUERY_NUM; i++) destroy_draw_text(help_prompts[i]);
    // free(help_prompts);

    // for (int i = 0; i < QUERY_NUM; i++) free(help_prompt_fills[i]);
    // free(help_prompt_fills);
}

#pragma GCC pop_options
