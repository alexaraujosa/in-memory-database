// TODO: Turn into TextArea component?

#include "executers/interactive/screens/dataset_question.h"
#include "util/math.h"

#define QUESTION_OFFSET_Y_OFFSET -2
#define BOX_X_BORDER 2
#define BOX_INPUT_DISPLAY_SIZE (size.cols - 2 * BOX_X_BORDER - 1)
#define BOX_INPUT_MAX_SIZE PATH_MAX

#define BOX_INSIDE_SPACE_KEY "_box_inside_space"
#define BOX_INSIDE_KEY "_box_inside"
#define BOX_INPUT_KEY "_box_input"
#define BOX_INPUT_IND_KEY "_box_input_ind"
#define BOX_INPUT_LOWER_BOUND_KEY "_box_input_lower_bound"
#define BOX_INPUT_SIZE_KEY "_box_input_size"

#pragma GCC push_options
#pragma GCC optimize ("O0")
static DrawText make_dt_from_str(char* text, GM_TERM_SIZE size, int offsetX, int offsetY) {
    IGNORE_ARG(size);

    ssize_t text_len = strlen(text);
    MAX_COLS_AND_ROWS text_mcar = get_max_rows_and_cols(text, text_len);
    int x = ((size.cols - text_mcar.cols) / 2) + offsetX;
    int y = ((size.rows - text_mcar.rows) / 2) + offsetY;

    DrawText text_dt = make_draw_text(text, text_len, x, y, text_mcar.rows, text_mcar.cols);
    return text_dt;
}

Cache make_cache_dataset_question(GM_Term term, FrameStore store) {
    IGNORE_ARG(term);
    IGNORE_ARG(store);

    GM_TERM_SIZE size = gm_term_get_size(term);
    Cache cache = make_cache(NULL);

    // TITLE
    char* question = get_localized_string(store->current_locale, LOCALE_SCREEN_DATASET_QUESTION_QUESTION);
    DrawText question_dt = make_dt_from_str(question, size, 0, QUESTION_OFFSET_Y_OFFSET);
    question_dt->x = BOX_X_BORDER;
    add_cache_elem(cache, LOCALE_SCREEN_DATASET_QUESTION_QUESTION, question_dt);

    // TEXT BOX INSIDE
    char* box_inside = (char*)malloc((BOX_INPUT_DISPLAY_SIZE + 1) * sizeof(char));
    int ind = 0;

    for (int i = 0; i < BOX_INPUT_DISPLAY_SIZE; i++) box_inside[ind++] = ' ';
    box_inside[ind++] = '\0';

    DrawText box_inside_dt = make_dt_from_str(box_inside, size, 0, QUESTION_OFFSET_Y_OFFSET);
    box_inside_dt->x = BOX_X_BORDER + 1;
    box_inside_dt->y = question_dt->y + 2;
    add_cache_elem(cache, BOX_INSIDE_SPACE_KEY, box_inside_dt);

    // BOX INPUT
    // ---- BOX INPUT BUFFER ----
    char* box_input = (char*)malloc(BOX_INPUT_MAX_SIZE * sizeof(char));
    memset(box_input, 0, BOX_INPUT_MAX_SIZE);
    add_cache_elem(cache, BOX_INPUT_KEY, box_input);

    // Sample text
    strcpy(box_input, "Lorem ipsum dolor sit amet consectetur, adipisicing elit. Rerum, porro ullam! Nemo corrupti porro voluptatibus veniam, consequatur repellendus, numquam quibusdam quam amet nulla optio neque, similique ea unde! Expedita, voluptatum.");

    // ---- BOX INPUT POSITION ----
    int* box_input_ind = (int*)malloc(sizeof(int));
    *box_input_ind = 50;
    add_cache_elem(cache, BOX_INPUT_IND_KEY, box_input_ind);

    int* box_input_lower_bound = (int*)malloc(sizeof(int));
    *box_input_lower_bound = 0;
    add_cache_elem(cache, BOX_INPUT_LOWER_BOUND_KEY, box_input_lower_bound);

    // ---- BOX INPUT SIZE ----
    int* box_input_size = (int*)malloc(sizeof(int));
    *box_input_size = 232;
    add_cache_elem(cache, BOX_INPUT_SIZE_KEY, box_input_size);

    // ---- DISPLAY BOX INPUT ----
    char* box_input_clamped = (char*)malloc((BOX_INPUT_DISPLAY_SIZE + 1) * sizeof(char));
    memset(box_input_clamped, 0, BOX_INPUT_DISPLAY_SIZE);

    DrawText box_input_clamped_dt = make_draw_text(
        box_input_clamped, 
        BOX_INPUT_DISPLAY_SIZE, 
        BOX_X_BORDER + 1, 
        question_dt->y + 2, 
        1, 
        BOX_INPUT_DISPLAY_SIZE
    );
    add_cache_elem(cache, BOX_INSIDE_KEY, box_input_clamped_dt);

    return cache;
}

void destroy_cache_dataset_question(Cache cache, int force) {
    IGNORE_ARG(cache);
    IGNORE_ARG(force);

    DrawText question_dt = get_cache_elem(cache, LOCALE_SCREEN_DATASET_QUESTION_QUESTION);
    destroy_draw_text(question_dt);

    DrawText box_inside_dt = get_cache_elem(cache, BOX_INSIDE_SPACE_KEY);
    destroy_draw_text(box_inside_dt);

    // BOX INPUT
    // ---- BOX INPUT BUFFER ----
    char* box_input = get_cache_elem(cache, BOX_INPUT_KEY);
    free(box_input);

    // ---- BOX INPUT POSITION ----
    int* box_input_ind = get_cache_elem(cache, BOX_INPUT_IND_KEY);
    free(box_input_ind);

    int* box_input_lower_bound = get_cache_elem(cache, BOX_INPUT_LOWER_BOUND_KEY);
    free(box_input_lower_bound);

    // ---- BOX INPUT SIZE ----
    int* box_input_size = get_cache_elem(cache, BOX_INPUT_SIZE_KEY);
    free(box_input_size);

    // ---- DISPLAY BOX INPUT ----
    DrawText box_input_clamped_dt = get_cache_elem(cache, BOX_INSIDE_KEY);
    destroy_draw_text(box_input_clamped_dt);
}

void draw_dataset_question(GM_Term term, FrameStore store, Cache cache) {
    IGNORE_ARG(term);
    IGNORE_ARG(store);
    IGNORE_ARG(cache);

    GM_TERM_SIZE size = gm_term_get_size(term);

    int* box_input_ind = get_cache_elem(cache, BOX_INPUT_IND_KEY);
    int* box_input_lower_bound = get_cache_elem(cache, BOX_INPUT_LOWER_BOUND_KEY);
    int* box_input_size = get_cache_elem(cache, BOX_INPUT_SIZE_KEY);

    DrawText question_dt = get_cache_elem(cache, LOCALE_SCREEN_DATASET_QUESTION_QUESTION);
    DrawText box_input_clamped_dt = get_cache_elem(cache, BOX_INSIDE_KEY);
    DrawText box_inside_dt = get_cache_elem(cache, BOX_INSIDE_SPACE_KEY);

    int cursor_pos = (*box_input_ind >= box_input_clamped_dt->len) ? box_input_clamped_dt->len : *box_input_ind;

    gm_attroff(term, GM_RESET);

    gm_printf(term, question_dt->y, question_dt->x, question_dt->text);
    
    gm_attron(term, GM_COLOR_PAIR(COLORPAIR_SELECTED));
    // Draw Box
    gm_box(term, question_dt->y + 1, BOX_X_BORDER, question_dt->y + 3, size.cols - BOX_X_BORDER);

    // Draw box background
    gm_printf(term, box_inside_dt->y, box_inside_dt->x, box_inside_dt->text);

    // Draw text
    gm_printf(term, box_input_clamped_dt->y, box_input_clamped_dt->x, box_input_clamped_dt->text);

    // Draw text clamps
    if (*box_input_ind >= box_input_clamped_dt->len) 
        gm_printf(term, box_input_clamped_dt->y, box_input_clamped_dt->x, " < ");
    if (*box_input_lower_bound + box_input_clamped_dt->len < *box_input_size) 
        gm_printf(term, box_input_clamped_dt->y, box_input_clamped_dt->x + box_input_clamped_dt->len - 3, " > ");

    // Draw cursor
    gm_printf(term, box_input_clamped_dt->y + 1, BOX_X_BORDER + cursor_pos, "^");
    gm_attroff(term, GM_COLOR_PAIR(COLORPAIR_SELECTED));

    // gm_show_cursor(term, FALSE);
    // gm_gotoxy(cursor_pos, box_input_clamped_dt->y, FALSE);
}

Keypress_Code keypress_dataset_question(GM_Term term, FrameStore store, Cache cache, GM_Key key) {
    IGNORE_ARG(term);
    IGNORE_ARG(store);
    IGNORE_ARG(key);

    char* box_input = get_cache_elem(cache, BOX_INPUT_KEY);
    int* box_input_ind = get_cache_elem(cache, BOX_INPUT_IND_KEY);
    int* box_input_lower_bound = get_cache_elem(cache, BOX_INPUT_LOWER_BOUND_KEY);
    int* box_input_size = get_cache_elem(cache, BOX_INPUT_SIZE_KEY);
    DrawText box_input_clamped_dt = get_cache_elem(cache, BOX_INSIDE_KEY);

    GM_Key ckey = gm_get_canonical_key(key);
    switch (ckey) {
        case GM_KEY_ARROW_LEFT: {
            // Shift input position to the left

            // OOB check
            if (*box_input_ind == 0) return KEY_SKIP;

            *box_input_ind -= 1 + GM_HAS_CTRL(key) + (GM_HAS_SHIFT(key) ? 5 : 0);
            if (*box_input_ind < 0) *box_input_ind = 0;
            
            if (*box_input_ind < box_input_clamped_dt->len) {
                // Move cursor, don't move text.

                // If moving cursor, but text is still not at head, snap it back.
                if (*box_input_lower_bound != 0) {
                    strncpy(
                        box_input_clamped_dt->text, 
                        box_input, 
                        box_input_clamped_dt->len
                    );
                }
            } else {
                // Don't move cursor, move text.
                strncpy(
                    box_input_clamped_dt->text, 
                    box_input + (*box_input_ind - box_input_clamped_dt->len), 
                    box_input_clamped_dt->len
                );

                *box_input_lower_bound = *box_input_ind - box_input_clamped_dt->len;
            }

            break;
        }
        case GM_KEY_ARROW_RIGHT: {
            // Shift input position to the right

            // OOB check
            if (*box_input_ind == *box_input_size) return KEY_SKIP;

            *box_input_ind += 1 + GM_HAS_CTRL(key) + (GM_HAS_SHIFT(key) ? 5 : 0);
            if (*box_input_ind > *box_input_size) *box_input_ind = *box_input_size;

            if (*box_input_ind < box_input_clamped_dt->len) {
                // Move cursor, don't move text.
            } else {
                // Don't move cursor, move text. (>=)
                strncpy(
                    box_input_clamped_dt->text, 
                    box_input + (*box_input_ind - box_input_clamped_dt->len), 
                    box_input_clamped_dt->len
                );

                *box_input_lower_bound = *box_input_ind - box_input_clamped_dt->len;
            }

            break;
        }
        case GM_KEY_ENTER: {
            // Accept input
            break;
        }
        case GM_KEY_BACKSPACE: {
            if (*box_input_ind == 0) return KEY_SKIP;

            // memmove(box_input + *box_input_ind - 1, box_input + *box_input_ind, *box_input_size - *box_input_ind);
            memmove(box_input + *box_input_ind - 2, box_input + *box_input_ind - 1, *box_input_size - *box_input_ind);
            (*box_input_ind)--;
            (*box_input_size)--;

            strncpy(
                box_input_clamped_dt->text, 
                box_input + imax(0, *box_input_ind - box_input_clamped_dt->len), 
                box_input_clamped_dt->len
            );

            *box_input_lower_bound = imax(0, *box_input_ind - box_input_clamped_dt->len);
            break;
        }
        case GM_KEY_PASTE_START: {
            // char* buf = (char*)malloc((BOX_INPUT_MAX_SIZE - *box_input_size) * sizeof(char));
            // int ind = 0;

            GString* str = get_bracketed_paste();

            // Clamp pasted
            // if (*box_input_size + ind >= BOX_INPUT_MAX_SIZE) ind = BOX_INPUT_MAX_SIZE - *box_input_size;
            if (*box_input_size + str->len >= BOX_INPUT_MAX_SIZE) str->len = BOX_INPUT_MAX_SIZE - *box_input_size;

            // add_str_to_str_at(box_input, *box_input_ind, buf, ind);
            add_str_to_str_at(box_input, *box_input_ind, str->str, str->len);

            *box_input_ind += str->len;
            *box_input_size += str->len;

            strncpy(
                box_input_clamped_dt->text, 
                box_input + imax(0, *box_input_ind - box_input_clamped_dt->len), 
                box_input_clamped_dt->len
            );

            *box_input_lower_bound = imax(0, *box_input_ind - box_input_clamped_dt->len);

            // free(buf);
            g_string_free(str, TRUE);
            break;
        }
        default: {
            if (*box_input_size == BOX_INPUT_MAX_SIZE - 1) return KEY_SKIP;

            if (GM_KEY_IS_ALPHANUMERIC(key) || ispunct(ckey)) {
                // add_char_to_str_at(box_input, *box_input_ind, ckey);
                add_char_to_str_at(box_input, *box_input_ind - 1, ckey);
                (*box_input_ind)++;
                (*box_input_size)++;

                strncpy(
                    box_input_clamped_dt->text, 
                    box_input + imax(0, *box_input_ind - box_input_clamped_dt->len), 
                    box_input_clamped_dt->len
                );

                *box_input_lower_bound = imax(0, *box_input_ind - box_input_clamped_dt->len);
            }
        }
    }
}
#pragma GCC pop_options
