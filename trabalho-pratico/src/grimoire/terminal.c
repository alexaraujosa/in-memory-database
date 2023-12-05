#include "grimoire/terminal.h"
#include "grimoire/grimoire_priv.h"

#include "common.h"
#include <stdbool.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <unistd.h>
#include "util/math.h"

/* ============== CONSTANTS ============== */
static GList* gm_terminal_resize_queue = NULL;
static bool gm_tui_initialized = false;

/* ============== TYPEDEFS ============== */
// typedef struct gm_term {
//     GM_TERM_SIZE size;
//     char** buf;
// } TERM, *GM_Term;

struct gm_term_event_index {
    GM_Term term;
    GM_TERMINAL_RESIZE_LISTENER(listener);
};

#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

/* ============== FORWARD DECLARATIONS ============== */
void gm_term_make_buffer(GM_Term term, int rows, int cols);
void gm_term_free_buffer(GM_Term term);
void gm_term_flush_buffer(GM_Term term);

/* ============== TERM ============== */
GM_Term gm_term_init() {
    GM_Term term = (GM_Term)malloc(sizeof(TERM));
    term->size = gm_get_tui_size();
    gm_term_make_buffer(term, term->size.rows, term->size.cols);
    term->color_pairs = g_hash_table_new_full(g_direct_hash, g_direct_equal, free, NULL);

    gm_setup_tui_events();

    return term;
}

/* ------- TERM BUFFER ------- */
void gm_term_make_buffer(GM_Term term, int rows, int cols) {
    GM_Buf buf = (GM_Buf)malloc(sizeof(GM_BUF));
    buf->data = (char**)malloc(rows * sizeof(char*));

    for (int i = 0; i < rows; i++) {
        buf->data[i] = (char*)malloc((MAX_CHAR_SEQ_BYTES * cols + 1) * sizeof(char));
        memset(buf->data[i], 0, (MAX_CHAR_SEQ_BYTES * cols + 1) * sizeof(char));
    }

    term->buf = buf;
}

void gm_term_free_buffer(GM_Term term) {
    if (term->buf == NULL) return;

    for (int i = 0; i < term->buf->rows; i++) {
        if (term->buf->data[i] == NULL) free(term->buf->data[i]);
    }

    free(term->buf->data);
    free(term->buf);
    term->buf = NULL;
}

void gm_term_clear_buffer(GM_Term term) {
    if (term->buf == NULL) return;

    for (int i = 0; i < term->buf->rows; i++) {
        if (term->buf->data[i] != NULL) {
            memset(term->buf->data[i], 0, (MAX_CHAR_SEQ_BYTES * term->buf->cols + 1) * sizeof(char));
        }
    }
}

void gm_term_flush_buffer(GM_Term term) {
    if (term->buf == NULL) return;

    int rows = imin(term->size.rows, term->buf->rows);
    int cols = imin(term->size.cols, term->buf->cols);

    gotoxy(0, 0);

    for (int i = 0; i < rows; i++) {
        // if (term->buf->data[i] != NULL) puts(term->buf->data[i]);
        // if (term->buf->data[i] != NULL) printf("%.*s\n", cols, term->buf->data[i]);

        int rcols = 0;
        int truelen;

        #define max_len MAX_CHAR_SEQ_BYTES * term->buf->cols
        for (truelen = 0; truelen < max_len; truelen++) {
            // Check for ANSI escape codes
            if (term->buf->data[i][truelen] == '\x1B' && truelen + 1 < max_len && term->buf->data[i][truelen + 1] == '[') {
                // Skip ANSI escape code
                while (term->buf->data[i][truelen] != 'm' && truelen + 1 < max_len) truelen++;
                truelen++; // Move past 'm'
            } else {
                // Handle UTF-8 multi-byte characters
                uint8_t byte = (uint8_t)term->buf->data[i][truelen];
                if ((byte & 0xC0) != 0x80) {
                    // This is the start of a character
                    rcols++;
                    if (rcols == cols) break;
                }

                truelen++;
            }
        }

        printf("%.*s\n", truelen, term->buf->data[i]);
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

void refresh(GM_Term term) {
    gm_term_flush_buffer(term);
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

    if (gm_terminal_resize_queue != NULL) g_list_free(gm_terminal_resize_queue);
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