#include "grimoire/terminal.h"
#include "grimoire/attr.h"
#include "grimoire/io.h"
#include "grimoire/grimoire_priv.h"

#include <stdbool.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <term.h>

#include "common.h"
#include "util/error.h"
#include "util/math.h"
#include "util/misc.h"

/* ============== CONSTANTS ============== */
static GList* gm_terminal_resize_queue = NULL;
static bool gm_tui_initialized = false;
static struct termios gm_original_terminal;
static struct termios gm_terminal;

/* ============== TYPEDEFS ============== */
struct gm_term_event_index {
    GM_Term term;
    GM_TERMINAL_RESIZE_LISTENER(listener);
};

/* ============== TERM ============== */
GM_Term gm_term_init() {
    GM_Term term = (GM_Term)malloc(sizeof(GM_TERM));
    term->size = gm_get_tui_size();

    term->color_pairs = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, g_free);
    term->colors = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, g_free);

    term->box_chars = (GM_BOX_CHARS){ 
        .tlc = "┌", 
        .trc = "┐", 
        .blc = "└", 
        .brc = "┘", 
        .hl = "─", 
        .vl = "│",
        .il = "├",
        .ir = "┤",
        .it = "┬",
        .ib = "┴",
        .ic = "┼"
    };

    term->attr_queue = g_queue_new();

    term->term_name = getenv("TERM");

    gm_setup_tui_events();

    return term;
}

void gm_term_end(GM_Term term) {
    gm_clear(FALSE);

    gm_show_cursor(term);

    g_hash_table_destroy(term->color_pairs);
    g_hash_table_destroy(term->colors);

    gm_term_attr_reset(term);
    g_queue_free(term->attr_queue);

    gm_close_tui_events();
    free(term);
}

/* ------- TERM ACCESSORS ------- */
int gm_term_is_xterm(GM_Term term) {
    return strncmp(term->term_name, "xterm", 5) == 0;
}

GM_TERM_SIZE gm_term_get_size(GM_Term term) {
    return term->size;
}

void gm_term_canvas_newframe(GM_Term term) {
    gm_clear(FALSE);
    gm_gotoxy(0 , 0, FALSE);

    // The snippet fills the entire screen with spaces.
    // Can be used if required.

    // gm_gotoxy(0, 0);

    // for (int i = 0; i < term->size.rows; i++) {
    //     for (int j = 0; j < term->size.cols; j++) putchar(' ');
    //     if (i != term->size.rows - 1) putchar('\n');
    // }
}

void gm_refresh(GM_Term term) {
    gm_term_canvas_newframe(term);

    while (!g_queue_is_empty(term->attr_queue)) {
        GM_Attr attr = gm_term_attr_get(term);
        gm_attr_resolve(term, attr);
        gm_attr_destroy(attr);
    }

    gm_term_attr_reset(term);
    
    gm_gotoxy(term->size.rows, term->size.cols, FALSE);

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
/* ------- RESIZE ------- */
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

/* ------- TERMINAL ATTRIBUTES ------- */
void gm_setup_terminal_attributes() {
    #define SCOPE "gm_setup_terminal_attributes"
    setupterm(NULL, STDOUT_FILENO, (int*)0);
    printf(keypad_xmit);

    if (tcgetattr(STDIN_FILENO, &gm_original_terminal) == -1) {
        printf("%s\n", trace_msg(SCOPE, "Error trying to read terminal attributes."));
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    gm_terminal = gm_original_terminal;
    gm_terminal.c_iflag &= ~(ICRNL | IXON);
    gm_terminal.c_lflag &= ~(ICANON | ECHO | IEXTEN | ISIG);

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &gm_terminal) == -1) {
        printf("%s\n", trace_msg(SCOPE, "Error trying to set new terminal attributes."));
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    // Set non-blocking mode for stdin
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    #undef SCOPE
}

void gm_restore_terminal_attributes() {
    #define SCOPE "gm_setup_terminal_attributes"
    
    printf(keypad_local);
    // After literal HOURS during New Year's Eve reading man pages, the NCurses source code
    // and a lot of mental sanity completely obliterated, I found out that THIS FUCKER SOLVES the TERMINFO MEMLEAKS.
    del_curterm(cur_term);

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &gm_original_terminal) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags ^ O_NONBLOCK);
    #undef SCOPE
}

/* ------- TUI ------- */
void gm_setup_tui_events() {
    if (gm_tui_initialized) return;

    gm_setup_terminal_attributes();

    signal(SIGWINCH, sigwinch_handler);
    gm_tui_initialized = true;
}

void gm_close_tui_events() {
    gm_restore_terminal_attributes();

    signal(SIGWINCH, SIG_DFL);

    if (gm_terminal_resize_queue != NULL) {
        for (GList* iter = gm_terminal_resize_queue; iter != NULL; iter = g_list_next(iter)) free(iter->data);
        g_list_free(gm_terminal_resize_queue);
    }
}
