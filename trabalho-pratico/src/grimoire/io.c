#include "grimoire/io.h"
// #include "grimoire/terminal.h"
#include "cache/cache.h"

#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#include <term.h>

#include "common.h"
#include "util/error.h"
#include "util/string.h"

// Maximum number of codes per key.
#define MAX_KEY_SEQUENCE 100

// Capabilities returned from terminfo
// #define GM_CAP_KEY_ENTER key_enter

// XTerm modifiers from https://invisible-island.net/ncurses/terminfo.src.html#tic-xterm_pcfkeys
#define XTERM_MOD_SHIFT           "2"
#define XTERM_MOD_ALT             "3"
#define XTERM_MOD_SHIFTALT        "4"
#define XTERM_MOD_CTRL            "5"
#define XTERM_MOD_SHIFTCTRL       "6"
#define XTERM_MOD_ALTCTRL         "7"
#define XTERM_MOD_SHIFTALTCTRL    "8"

#define GM_CAP_KEY_F1  key_f1  // 27 91 80
#define GM_CAP_KEY_F2  key_f2  // 27 91 81
#define GM_CAP_KEY_F3  key_f3  // 27 91 82
#define GM_CAP_KEY_F4  key_f4  // 27 91 83
#define GM_CAP_KEY_F5  key_f5  // 27 91 49 53 126
#define GM_CAP_KEY_F6  key_f6  // 27 91 49 55 126
#define GM_CAP_KEY_F7  key_f7  // 27 91 49 56 126
#define GM_CAP_KEY_F8  key_f8  // 27 91 49 57 126
#define GM_CAP_KEY_F9  key_f9  // 27 91 50 48 126
#define GM_CAP_KEY_F10 key_f10 // 27 91 50 49 126
#define GM_CAP_KEY_F11 key_f11 // 27 91 50 50 126
#define GM_CAP_KEY_F12 key_f12 // 27 91 50 52 126

#define GM_CAP_ARROW_HEADER "\033[1;"
#define GM_CAP_ARROW_UP_TAIL    "A"
#define GM_CAP_ARROW_DOWN_TAIL  "B"
#define GM_CAP_ARROW_RIGHT_TAIL "C"
#define GM_CAP_ARROW_LEFT_TAIL  "D"

#define GM_CAP_KEY_ARROW_UP    key_up
#define GM_CAP_KEY_ARROW_DOWN  key_down
#define GM_CAP_KEY_ARROW_LEFT  key_left
#define GM_CAP_KEY_ARROW_RIGHT key_right
#define GM_CAP_KEY_ARROW_LEFT_SHIFT  key_sleft
#define GM_CAP_KEY_ARROW_RIGHT_SHIFT key_sright

#define GM_CAP_KEY_ARROW_UP_CTRL    GM_CAP_ARROW_HEADER XTERM_MOD_CTRL GM_CAP_ARROW_UP_TAIL
#define GM_CAP_KEY_ARROW_DOWN_CTRL  GM_CAP_ARROW_HEADER XTERM_MOD_CTRL GM_CAP_ARROW_DOWN_TAIL
#define GM_CAP_KEY_ARROW_LEFT_CTRL  GM_CAP_ARROW_HEADER XTERM_MOD_CTRL GM_CAP_ARROW_LEFT_TAIL
#define GM_CAP_KEY_ARROW_RIGHT_CTRL GM_CAP_ARROW_HEADER XTERM_MOD_CTRL GM_CAP_ARROW_RIGHT_TAIL

#define GM_CAP_KEY_ARROW_UP_SHIFT_CTRL    GM_CAP_ARROW_HEADER XTERM_MOD_SHIFTCTRL GM_CAP_ARROW_UP_TAIL
#define GM_CAP_KEY_ARROW_DOWN_SHIFT_CTRL  GM_CAP_ARROW_HEADER XTERM_MOD_SHIFTCTRL GM_CAP_ARROW_DOWN_TAIL
#define GM_CAP_KEY_ARROW_LEFT_SHIFT_CTRL  GM_CAP_ARROW_HEADER XTERM_MOD_SHIFTCTRL GM_CAP_ARROW_LEFT_TAIL
#define GM_CAP_KEY_ARROW_RIGHT_SHIFT_CTRL GM_CAP_ARROW_HEADER XTERM_MOD_SHIFTCTRL GM_CAP_ARROW_RIGHT_TAIL

#define GM_CAP_BRACKETED_PASTE_START "\e[200~"
#define GM_CAP_BRACKETED_PASTE_END   "\e[201~"


int gm_kbhit() {
    struct timeval tv = { 0L, 0L };
    fd_set fds;

    FD_ZERO(&fds);
    FD_SET(0, &fds);

    return select(1, &fds, NULL, NULL, &tv) > 0;
}

char gm_getch() {
    int r;
    unsigned char c;

    if ((r = read(STDIN_FILENO, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}

/**
 * @brief Reads and interprets a key sequence from STDIN.
 * 
 * @param readEscape Whenever the escape character from the sequence has already been read.
 */
GM_Key _gm_get_sequence(int readEscape) {
    GString* sequence = g_string_new(readEscape ? "\033" : NULL);

    #define RET_AND_CLEANUP(val) {\
        uint16_t ret = val;\
        g_string_free(sequence, TRUE);\
        return (ret);\
    }

    // #define CHAR_TO_INT(ch) ({int _val = ch; _val;})

    int flush = 0;
    while (gm_kbhit()) {
        char part = gm_getch();
        
        if (flush) continue;
        // if (part == '\033' && sequence->len > 2) {
        //     flush = 1;
        //     continue;
        // }
        if (part == '\033') {
            readEscape = TRUE;
            if (sequence->len > 2) {
                flush = 1;
                continue;
            }
        }

        g_string_append_c(sequence, part);

        if (!readEscape) break;

        // Add preemptive exits for bracketed paste headers.
        if (STRING_EQUAL(sequence->str, GM_CAP_BRACKETED_PASTE_START)) RET_AND_CLEANUP(GM_KEY_PASTE_START);
        if (STRING_EQUAL(sequence->str, GM_CAP_BRACKETED_PASTE_END))   RET_AND_CLEANUP(GM_KEY_PASTE_END);
    }

    if (sequence->len == 1) {
        // For some fucking reason, enter is not the same sequence returned by terminfo. Fuck it ig.
        if (sequence->str[0] == '\r') RET_AND_CLEANUP(GM_KEY_ENTER);

        if (isalpha(sequence->str[0]) && CHAR_IS_UPPER(sequence->str[0])) 
            RET_AND_CLEANUP(char_tolower(sequence->str[0]) | GM_MOD_SHIFT);

        // Required because yes. Valgrind will scream otherwise, and I will cry.
        if (GM_KEY_IS_CTRL(sequence->str[0])) {
            int val = sequence->str[0];
            RET_AND_CLEANUP((int)(val | GM_MOD_CTRL));
        }

        RET_AND_CLEANUP(sequence->str[0]);
    } else {
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_F1))  RET_AND_CLEANUP(GM_KEY_F1);
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_F2))  RET_AND_CLEANUP(GM_KEY_F2);
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_F3))  RET_AND_CLEANUP(GM_KEY_F3);
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_F4))  RET_AND_CLEANUP(GM_KEY_F4);
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_F5))  RET_AND_CLEANUP(GM_KEY_F5);
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_F6))  RET_AND_CLEANUP(GM_KEY_F6);
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_F7))  RET_AND_CLEANUP(GM_KEY_F7);
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_F8))  RET_AND_CLEANUP(GM_KEY_F8);
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_F9))  RET_AND_CLEANUP(GM_KEY_F9);
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_F10)) RET_AND_CLEANUP(GM_KEY_F10);
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_F11)) RET_AND_CLEANUP(GM_KEY_F11);
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_F12)) RET_AND_CLEANUP(GM_KEY_F12);

        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_ARROW_UP))    RET_AND_CLEANUP(GM_KEY_ARROW_UP);
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_ARROW_DOWN))  RET_AND_CLEANUP(GM_KEY_ARROW_DOWN);
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_ARROW_LEFT))  RET_AND_CLEANUP(GM_KEY_ARROW_LEFT);
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_ARROW_RIGHT)) RET_AND_CLEANUP(GM_KEY_ARROW_RIGHT);

        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_ARROW_LEFT_SHIFT))  RET_AND_CLEANUP(GM_KEY_ARROW_LEFT | GM_MOD_SHIFT);
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_ARROW_RIGHT_SHIFT)) RET_AND_CLEANUP(GM_KEY_ARROW_RIGHT | GM_MOD_SHIFT);

        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_ARROW_UP_CTRL))    RET_AND_CLEANUP(GM_KEY_ARROW_UP | GM_MOD_CTRL);
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_ARROW_DOWN_CTRL))  RET_AND_CLEANUP(GM_KEY_ARROW_DOWN | GM_MOD_CTRL);
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_ARROW_LEFT_CTRL))  RET_AND_CLEANUP(GM_KEY_ARROW_LEFT | GM_MOD_CTRL);
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_ARROW_RIGHT_CTRL)) RET_AND_CLEANUP(GM_KEY_ARROW_RIGHT | GM_MOD_CTRL);

        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_ARROW_UP_SHIFT_CTRL))    RET_AND_CLEANUP(GM_KEY_ARROW_UP | GM_MOD_SHIFT);
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_ARROW_DOWN_SHIFT_CTRL))  RET_AND_CLEANUP(GM_KEY_ARROW_DOWN | GM_MOD_SHIFT);
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_ARROW_LEFT_SHIFT_CTRL))  RET_AND_CLEANUP(GM_KEY_ARROW_LEFT | GM_MOD_SHIFT);
        if (STRING_EQUAL(sequence->str, GM_CAP_KEY_ARROW_RIGHT_SHIFT_CTRL)) RET_AND_CLEANUP(GM_KEY_ARROW_RIGHT | GM_MOD_SHIFT);
    }

    RET_AND_CLEANUP(GM_KEY_NUL);
    #undef RET_AND_CLEANUP
}

GM_Key gm_get_key(GM_Term term) {
    IGNORE_ARG(term);

    #define SCOPE "gm_get_key"

    return _gm_get_sequence(FALSE);
    #undef SCOPE
}

GM_Key gm_get_canonical_key(GM_Key key) {
    int ckey = GM_CANON_KEY(key);
    if (isalpha(ckey) && GM_HAS_SHIFT(key)) return char_toupper(ckey);
    
    return ckey;
}

GString* get_bracketed_paste() {
    GString* buf = g_string_new(NULL);

    char ch;
    GM_Key key;
    for (;;) {
        ch = gm_getch();

        if (ch == '\033') {
            key = _gm_get_sequence(TRUE);
            if (key == GM_KEY_PASTE_END) break;
            continue;
        }

        g_string_append_c(buf, ch);
    }

    return buf;
}

/* 
 * The man page for terminfo says: 
 *   "All terminfo strings [including the output of tparm] should be printed with tputs or putp."
 * 
 * I say they can drive off a cliff.
 */

void gm_hide_cursor(GM_Term term, int flush) {
    IGNORE_ARG(term);

    printf("%s", cursor_invisible);
    if (flush) fflush(stdout);
}

void gm_show_cursor(GM_Term term, int flush) {
    IGNORE_ARG(term);

    printf("%s", cursor_normal);
    if (flush) fflush(stdout);
}

void gm_clear(int flush) {
    printf("%s", clear_screen);
    if (flush) fflush(stdout);
}

void gm_gotoxy(int x, int y, int flush) {
    printf("\e[%d;%dH", y, x); // TODO: Use terminfo's cursor_address?
    if (flush) fflush(stdout);
}

void gm_reset_attr(int flush) {
    printf("\e[0m");
    if (flush) fflush(stdout);
}

void enable_bracketed_paste() {
    printf("\e[?2004h");
    fflush(stdout);
}

void disable_bracketed_paste() {
    printf("\e[?2004l");
    fflush(stdout);
}