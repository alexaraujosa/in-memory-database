// NOTE: SO MANY EDGE CASES, WHAT KIND OF DEMON POSSESSED ME TO CREATE THIS SHIT?

#include "executers/interactive/components/textarea.h"
#include "util/math.h"

#define TAIL_OFFSET 3

#define BOX_FILL_KEY "_box_fill"
#define BOX_INSIDE_KEY "_box_inside"
#define BOX_INPUT_KEY "_box_input"
#define BOX_INPUT_IND_KEY "_box_input_ind"
#define BOX_INPUT_LOWER_BOUND_KEY "_box_input_lower_bound"
#define BOX_INPUT_SIZE_KEY "_box_input_size"

Cache make_textarea_cache(GM_Term term, FrameStore store, TextArea text_area, char* header_text);
void _destroy_textarea_cache(Cache cache);

typedef struct textarea {
    Cache cache;
    int x;
    int y;
    int size_cols;
    int size_rows;
    int max_text_len;
    int active;
    int active_colorpair;
    int inactive_colorpair;
} TEXT_AREA, *TextArea;

// ============== CONSTRUCTOR ==============
TextArea make_textarea(
    GM_Term term, 
    FrameStore store, 
    int x, 
    int y, 
    int size_cols, 
    int size_rows, 
    int max_text_len,
    int active_colorpair,
    int inactive_colorpair, 
    char* header_text
) {
    TextArea text_area = (TextArea)malloc(sizeof(TEXT_AREA));
    text_area->x = x;
    text_area->y = y;
    text_area->size_cols = size_cols;
    text_area->size_rows = size_rows;
    text_area->max_text_len = max_text_len;
    text_area->active = TRUE;
    text_area->active_colorpair = active_colorpair;
    text_area->inactive_colorpair = inactive_colorpair;

    text_area->cache = make_textarea_cache(term, store, text_area, header_text);

    text_area->size_rows = 4;

    return text_area;
}


// ------- CACHE CONSTRUCTOR -------
static DrawText make_dt_from_str(char* text, GM_TERM_SIZE size, int x, int y) {
    IGNORE_ARG(size);
    ssize_t text_len = strlen(text);
    MAX_COLS_AND_ROWS text_mcar = get_max_rows_and_cols(text, text_len);

    DrawText text_dt = make_draw_text(text, text_len, x, y, text_mcar.rows, text_mcar.cols);
    return text_dt;
}
Cache make_textarea_cache(GM_Term term, FrameStore store, TextArea text_area, char* header_text) {
    IGNORE_ARG(store);

    GM_TERM_SIZE size = gm_term_get_size(term);
    Cache cache = make_cache(NULL);

    int box_size = text_area->size_cols - text_area->x + 1;

    // TITLE
    DrawText header_text_dt = make_dt_from_str(header_text, size, text_area->x, text_area->y);
    add_cache_elem(cache, LOCALE_SCREEN_DATASET_QUESTION_QUESTION, header_text_dt);

    // TEXT BOX INSIDE
    char* box_fill = (char*)malloc((box_size + 1) * sizeof(char));
    int ind = 0;

    for (int i = 0; i < box_size; i++) box_fill[ind++] = ' ';
    box_fill[ind++] = '\0';

    DrawText box_fill_dt = make_dt_from_str(box_fill, size, text_area->x + 1, header_text_dt->y + 2);
    add_cache_elem(cache, BOX_FILL_KEY, box_fill_dt);

    // BOX INPUT
    // ---- BOX INPUT BUFFER ----
    char* box_input = (char*)malloc(text_area->max_text_len * sizeof(char));
    memset(box_input, 0, text_area->max_text_len);
    add_cache_elem(cache, BOX_INPUT_KEY, box_input);

    // ---- BOX INPUT POSITION ----
    int* box_input_ind = (int*)malloc(sizeof(int));
    *box_input_ind = 0;
    add_cache_elem(cache, BOX_INPUT_IND_KEY, box_input_ind);

    int* box_input_lower_bound = (int*)malloc(sizeof(int));
    *box_input_lower_bound = 0;
    add_cache_elem(cache, BOX_INPUT_LOWER_BOUND_KEY, box_input_lower_bound);

    // ---- BOX INPUT SIZE ----
    int* box_input_size = (int*)malloc(sizeof(int));
    *box_input_size = 0;
    add_cache_elem(cache, BOX_INPUT_SIZE_KEY, box_input_size);

    // ---- DISPLAY BOX INPUT ----
    char* box_input_clamped = (char*)malloc((box_size + 1) * sizeof(char));
    memset(box_input_clamped, 0, box_size);

    DrawText box_input_clamped_dt = make_draw_text(
        box_input_clamped, 
        box_size, 
        text_area->x + 1, 
        header_text_dt->y + 2, 
        1, 
        box_size
    );
    add_cache_elem(cache, BOX_INSIDE_KEY, box_input_clamped_dt);

    return cache;
}


// ============== ACCESSORS ==============
char* get_textarea_input(TextArea text_area) {
    char* box_input = get_cache_elem(text_area->cache, BOX_INPUT_KEY);
    return strdup(box_input);
}

void set_textarea_input(TextArea text_area, char* input) {
    int* box_input_ind = get_cache_elem(text_area->cache, BOX_INPUT_IND_KEY);
    int* box_input_lower_bound = get_cache_elem(text_area->cache, BOX_INPUT_LOWER_BOUND_KEY);
    int* box_input_size = get_cache_elem(text_area->cache, BOX_INPUT_SIZE_KEY);
    char* box_input = get_cache_elem(text_area->cache, BOX_INPUT_KEY);
    DrawText box_input_clamped_dt = get_cache_elem(text_area->cache, BOX_INSIDE_KEY);

    // Replace input contents
    memset(box_input, 0, *box_input_size);
    strncpy(box_input, input, *box_input_size);
    memset(box_input_clamped_dt->text, 0, box_input_clamped_dt->len);

    // Set index back to 0
    *box_input_ind = 0;
    *box_input_lower_bound = 0;
    *box_input_size = 0;
}

int get_textarea_x(TextArea text_area) {
    return text_area->x;
}

int get_textarea_y(TextArea text_area) {
    return text_area->y;
}

int get_textarea_size_cols(TextArea text_area) {
    return text_area->size_cols;
}

int get_textarea_size_rows(TextArea text_area) {
    return text_area->size_rows;
}

int is_textarea_active(TextArea text_area) {
    return text_area->active;
}

void set_textarea_active(TextArea text_area, int active) {
    text_area->active = active;
}

// ============== DESTRUCTOR ==============
void destroy_textarea(TextArea text_area) {
    _destroy_textarea_cache(text_area->cache);
    free(text_area);
}

void _destroy_textarea_cache(Cache cache) {
    DrawText header_text_dt = get_cache_elem(cache, LOCALE_SCREEN_DATASET_QUESTION_QUESTION);
    destroy_draw_text(header_text_dt);

    DrawText box_fill_dt = get_cache_elem(cache, BOX_FILL_KEY);
    destroy_draw_text(box_fill_dt);

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

    destroy_cache(cache, TRUE);
}

// ============== DRAW CALL ==============
void draw_textarea(GM_Term term, FrameStore store, TextArea text_area) {
    IGNORE_ARG(store);

    GM_TERM_SIZE size = gm_term_get_size(term);

    int* box_input_ind = get_cache_elem(text_area->cache, BOX_INPUT_IND_KEY);
    // int* box_input_lower_bound = get_cache_elem(text_area->cache, BOX_INPUT_LOWER_BOUND_KEY);
    int* box_input_size = get_cache_elem(text_area->cache, BOX_INPUT_SIZE_KEY);

    DrawText header_text_dt = get_cache_elem(text_area->cache, LOCALE_SCREEN_DATASET_QUESTION_QUESTION);
    DrawText box_input_clamped_dt = get_cache_elem(text_area->cache, BOX_INSIDE_KEY);
    DrawText box_fill_dt = get_cache_elem(text_area->cache, BOX_FILL_KEY);

    int cursor_pos = (*box_input_ind >= box_input_clamped_dt->len - TAIL_OFFSET) 
        ? box_input_clamped_dt->len - TAIL_OFFSET 
        : *box_input_ind + 1;

    int colorpair = text_area->active ? text_area->active_colorpair : text_area->inactive_colorpair;

    gm_attroff(term, GM_RESET);

    // Print header text
    gm_printf(term, header_text_dt->y, header_text_dt->x, header_text_dt->text);
    
    gm_attron(term, GM_COLOR_PAIR(colorpair));

    // Draw Box
    // gm_box(term, header_text_dt->y + 1, text_area->x, header_text_dt->y + 3, size.cols - text_area->x);
    gm_box(term, header_text_dt->y + 1, text_area->x, header_text_dt->y + 3, text_area->x + text_area->size_cols);

    // Draw box background
    gm_printf(term, box_fill_dt->y, box_fill_dt->x, box_fill_dt->text);

    // Draw text
    gm_printf(term, box_input_clamped_dt->y, box_input_clamped_dt->x, box_input_clamped_dt->text);

    // Draw text clamps
    if (*box_input_ind >= box_input_clamped_dt->len)
        gm_printf(term, box_input_clamped_dt->y, box_input_clamped_dt->x, " < ");
    if (*box_input_size - *box_input_ind >= TAIL_OFFSET)
        gm_printf(term, box_input_clamped_dt->y, box_input_clamped_dt->x + box_input_clamped_dt->len - 3, " > ");

    // Draw cursor
    // if (text_area->size_cols) gm_printf(term, box_input_clamped_dt->y + 1, text_area->x + cursor_pos, "^");
    if (text_area->active) gm_printf(term, box_input_clamped_dt->y + 1, text_area->x + cursor_pos, "^");

    gm_attroff(term, GM_COLOR_PAIR(colorpair));
}

// ============== KEYPRESS HANDLER ==============
// WARN: Input so slow my wheelchaired grandma runs faster.
Keypress_Code keypress_textarea(
    GM_Term term, 
    FrameStore store, 
    TextArea text_area, 
    GM_Key key, 
    TextAreaKeypressCallback callback,
    TextAreaKeypressCallbackUserData user_data
) {
    if (!text_area->active) return KEY_SKIP;

    char* box_input = get_cache_elem(text_area->cache, BOX_INPUT_KEY);
    int* box_input_ind = get_cache_elem(text_area->cache, BOX_INPUT_IND_KEY);
    int* box_input_lower_bound = get_cache_elem(text_area->cache, BOX_INPUT_LOWER_BOUND_KEY);
    int* box_input_size = get_cache_elem(text_area->cache, BOX_INPUT_SIZE_KEY);
    DrawText box_input_clamped_dt = get_cache_elem(text_area->cache, BOX_INSIDE_KEY);

    GM_Key ckey = gm_get_canonical_key(key);
    switch (ckey) {
        case GM_KEY_ARROW_LEFT: {
            // Shift input position to the left

            // OOB check
            if (*box_input_ind == 0) return KEY_SKIP;

            *box_input_ind -= 1 + GM_HAS_CTRL(key) + (GM_HAS_SHIFT(key) ? 5 : 0);
            if (*box_input_ind < 0) *box_input_ind = 0;
            
            // Why the fuck do I need to add 1 here? It doesn't make sense.
            if (*box_input_ind < box_input_clamped_dt->len - (TAIL_OFFSET + 1)) {
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
                    box_input + (*box_input_ind - box_input_clamped_dt->len + TAIL_OFFSET + 1), 
                    box_input_clamped_dt->len
                );

                *box_input_lower_bound = *box_input_ind - box_input_clamped_dt->len + TAIL_OFFSET + 1;
            }
            
            return KEY_RECIEVED;
            break;
        }
        case GM_KEY_ARROW_RIGHT: {
            // Shift input position to the right

            // OOB check
            if (*box_input_ind == *box_input_size) return KEY_SKIP;

            *box_input_ind += 1 + GM_HAS_CTRL(key) + (GM_HAS_SHIFT(key) ? 5 : 0);
            if (*box_input_ind > *box_input_size) *box_input_ind = *box_input_size;

            if (*box_input_ind <= box_input_clamped_dt->len - (TAIL_OFFSET + 1)) {
                // Move cursor, don't move text.
            } else {
                // Don't move cursor, move text.
                strncpy(
                    box_input_clamped_dt->text, 
                    box_input + (*box_input_ind - box_input_clamped_dt->len + TAIL_OFFSET + 1), 
                    box_input_clamped_dt->len
                );

                *box_input_lower_bound = *box_input_ind - box_input_clamped_dt->len + TAIL_OFFSET + 1;
            }

            return KEY_RECIEVED;
            break;
        }
        case GM_KEY_ENTER: {
            // Accept input
            return callback(term, store, text_area, key, user_data);
            break;
        }
        case GM_KEY_BACKSPACE: {
            if (*box_input_ind == 0) return KEY_SKIP;

            memmove(box_input + *box_input_ind - 1, box_input + *box_input_ind, *box_input_size - *box_input_ind + 1);
            (*box_input_ind)--;
            (*box_input_size)--;

            if (*box_input_ind < box_input_clamped_dt->len - (TAIL_OFFSET + 1)) {
                strncpy(
                    box_input_clamped_dt->text, 
                    box_input + imax(0, *box_input_ind - box_input_clamped_dt->len), 
                    box_input_clamped_dt->len
                );

                *box_input_lower_bound = imax(0, *box_input_ind - box_input_clamped_dt->len);
            } else {
                // Don't move cursor, move text.
                strncpy(
                    box_input_clamped_dt->text, 
                    box_input + (*box_input_ind - box_input_clamped_dt->len + TAIL_OFFSET + 1), 
                    box_input_clamped_dt->len
                );

                *box_input_lower_bound = *box_input_ind - box_input_clamped_dt->len + TAIL_OFFSET + 1;
            }

            return KEY_RECIEVED;
            break;
        }
        case GM_KEY_PASTE_START: {
            GString* str = get_bracketed_paste();
            int len = str->len;

            // Clamp pasted
            if (*box_input_size + len >= text_area->max_text_len) len = text_area->max_text_len - *box_input_size;

            add_str_to_str_at(box_input, *box_input_ind, str->str, len);

            *box_input_ind += len;
            *box_input_size += len;

            if (*box_input_ind <= box_input_clamped_dt->len - (TAIL_OFFSET + 1)) {
                strncpy(
                    box_input_clamped_dt->text, 
                    box_input + imax(0, *box_input_ind - box_input_clamped_dt->len), 
                    box_input_clamped_dt->len
                );

                *box_input_lower_bound = imax(0, *box_input_ind - box_input_clamped_dt->len);
            } else {
                strncpy(
                    box_input_clamped_dt->text, 
                    box_input + (*box_input_ind - box_input_clamped_dt->len + TAIL_OFFSET + 1), 
                    box_input_clamped_dt->len
                );

                *box_input_lower_bound = *box_input_ind - box_input_clamped_dt->len + TAIL_OFFSET + 1;
            }

            g_string_free(str, TRUE);

            return KEY_RECIEVED;
            break;
        }
        default: {
            if (*box_input_size == text_area->max_text_len - 1) return KEY_SKIP;

            if (GM_KEY_IS_ALPHANUMERIC(key) || ispunct(ckey)) {
                add_char_to_str_at(box_input, *box_input_ind, ckey);
                (*box_input_ind)++;
                (*box_input_size)++;

                if (*box_input_ind <= box_input_clamped_dt->len - (TAIL_OFFSET + 1)) {  
                    strncpy(
                        box_input_clamped_dt->text, 
                        box_input + imax(0, *box_input_ind - box_input_clamped_dt->len), 
                        box_input_clamped_dt->len
                    );

                    *box_input_lower_bound = imax(0, *box_input_ind - box_input_clamped_dt->len);
                } else {
                    strncpy(
                        box_input_clamped_dt->text, 
                        box_input + (*box_input_ind - box_input_clamped_dt->len + TAIL_OFFSET + 1), 
                        box_input_clamped_dt->len
                    );

                    *box_input_lower_bound = *box_input_ind - box_input_clamped_dt->len + TAIL_OFFSET + 1;
                }

                return KEY_RECIEVED;
            }

            return KEY_SKIP;
        }
    }
}

