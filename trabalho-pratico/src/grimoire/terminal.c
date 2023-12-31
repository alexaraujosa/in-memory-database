#include "grimoire/terminal.h"
#include "grimoire/attr.h"
#include "grimoire/grimoire_priv.h"

#include "common.h"
#include "util/math.h"
#include "util/misc.h"
#include <stdbool.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <unistd.h>

/* ============== CONSTANTS ============== */
static GList* gm_terminal_resize_queue = NULL;
static bool gm_tui_initialized = false;

/* ============== TYPEDEFS ============== */
struct gm_term_event_index {
    GM_Term term;
    GM_TERMINAL_RESIZE_LISTENER(listener);
};

/* ============== FORWARD DECLARATIONS ============== */
void gm_term_make_buffer(GM_Buf* termbuf, int rows, int cols, int charsize);
void gm_term_free_buffer(GM_Buf buf);

/* ============== TERM ============== */
GM_Term gm_term_init() {
    GM_Term term = (GM_Term)malloc(sizeof(GM_TERM));
    term->size = gm_get_tui_size();
    gm_term_make_buffer(&term->buf, term->size.rows, term->size.cols, MAX_UTF8_SEQ);

    term->color_pairs = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, g_free);
    term->colors = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, g_free);

    term->attr_queue = g_array_new(FALSE, FALSE, sizeof(GM_Attr));

    gm_setup_tui_events();

    return term;
}

void gm_term_end(GM_Term term) {
    gm_term_free_buffer(term->buf);

    g_hash_table_destroy(term->color_pairs);
    g_hash_table_destroy(term->colors);
    g_array_free(term->attr_queue, TRUE);

    gm_close_tui_events();
    free(term);
}

/* ------- TERM BUFFER ------- */
void gm_term_make_buffer(GM_Buf* termbuf, int rows, int cols, int charsize) {
    GM_Buf buf = (GM_Buf)malloc(sizeof(GM_BUF));
    buf->data = (char**)malloc(rows * sizeof(char*));

    for (int i = 0; i < rows; i++) {
        buf->data[i] = (char*)malloc((charsize * cols + 1) * sizeof(char));
        memset(buf->data[i], 32, (charsize * cols) * sizeof(char));
        buf->data[i][charsize * cols + 1] = '\0';
    }

    buf->charsize = charsize;
    buf->rows = rows;
    buf->cols = cols;

    *termbuf = buf;
}

void gm_term_free_buffer(GM_Buf buf) {
    if (buf == NULL) return;

    for (int i = 0; i < buf->rows; i++) {
        if (buf->data[i] != NULL) free(buf->data[i]);
    }

    free(buf->data);
    free(buf);
    buf = NULL;
}

void gm_term_clear_buffer(GM_Buf buf) {
    if (buf == NULL) return;

    for (int i = 0; i < buf->rows; i++) {
        if (buf->data[i] != NULL) {
            memset(buf->data[i], ' ', (buf->charsize * buf->cols + 1) * sizeof(char));
        }
    }
}

int gm_term_buf_truepos(GM_Term term, int row, int col) {
    if (term->buf == NULL) return -1;

    if (row > term->buf->rows) return -1;
    if (col > term->buf->cols) return -1;

    int rcols = 0;
    int truelen;

    #define max_len MAX_CHAR_SEQ_BYTES * term->buf->cols
    for (truelen = 0; truelen < max_len; truelen++) {
        // Check for ANSI escape codes
        if (term->buf->data[row][truelen] == '\x1B' && truelen + 1 < max_len && term->buf->data[row][truelen + 1] == '[') {
            // Skip ANSI escape code
            while (term->buf->data[row][truelen] != 'm' && truelen + 1 < max_len) truelen++;
            truelen++; // Move past 'm'
        } else {
            // Handle UTF-8 multi-byte characters
            uint8_t byte = (uint8_t)term->buf->data[row][truelen];
            if ((byte & 0xC0) != 0x80) {
                // This is the start of a character
                rcols++;
                if (rcols == col) break;
            }

            truelen++;
        }
    }

    return truelen;
}

void gm_term_canvas_newframe(GM_Term term) {
    clear();
    gotoxy(0 , 0);

    // The snippet fills the entire screen with spaces.
    // Can be used if required.

    // gotoxy(0, 0);

    // for (int i = 0; i < term->size.rows; i++) {
    //     for (int j = 0; j < term->size.cols; j++) putchar(' ');
    //     if (i != term->size.rows - 1) putchar('\n');
    // }
}

void gm_refresh(GM_Term term) {
    gm_term_canvas_newframe(term);

    for (int i = 0; i < term->attr_queue->len; i++) {
        GM_Attr attr = gm_term_attr_get(term, i);
        gm_attr_resolve(term, attr);
        #ifdef _DEBUG
        fflush(stdout);
        #endif
    }

    gm_term_attr_reset(term);
    gm_term_clear_buffer(term->buf);
    
    fflush(stdout);
}

/* ============== TERM SIZE ============== */
GM_TERM_SIZE gm_get_tui_size() {
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    return (GM_TERM_SIZE){
        .rows = w.ws_row,
        .cols = w.ws_col
    };
}

/* ============== EVENTS ============== */
static void sigwinch_handler(int sig) {
    if (sig == SIGWINCH) {
        GM_TERM_SIZE new_ts = gm_get_tui_size();

        for (GList* iter = gm_terminal_resize_queue; iter != NULL; iter = g_list_next(iter)) {
            struct gm_term_event_index* teind = iter->data;
            TermResizeListener listener = teind->listener;
            listener(teind->term, new_ts);
        }
    }
}

/* ------- TUI ------- */
void gm_setup_tui_events() {
    if (gm_tui_initialized) return;

    signal(SIGWINCH, sigwinch_handler);
    gm_tui_initialized = true;
}

void gm_close_tui_events() {
    signal(SIGWINCH, SIG_DFL);

    if (gm_terminal_resize_queue != NULL) {
        for (GList* iter = gm_terminal_resize_queue; iter != NULL; iter = g_list_next(iter)) free(iter->data);
        g_list_free(gm_terminal_resize_queue);
    }
}

int gm_term_add_tui_resize_listener(GM_Term term, GM_TERMINAL_RESIZE_LISTENER(listener)) {
    struct gm_term_event_index* teind = (struct gm_term_event_index*)malloc(sizeof(struct gm_term_event_index));
    teind->listener = listener;
    teind->term = term;

    gm_terminal_resize_queue = g_list_append(gm_terminal_resize_queue, teind);
    return 0;
}

int gm_term_remove_tui_resize_listener(GM_Term term, GM_TERMINAL_RESIZE_LISTENER(listener)) {
    for (GList* iter = gm_terminal_resize_queue; iter != NULL; iter = g_list_next(iter)) {
        struct gm_term_event_index* teind = iter->data;

        if (teind->term == term && teind->listener == listener) {
            gm_terminal_resize_queue = g_list_remove(gm_terminal_resize_queue, teind);
            return 0;
        }
    }

    return 1;
}