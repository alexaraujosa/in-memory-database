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
#define MAX_KEY_SEQUENCE 10

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

    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}

GM_Key gm_get_key(GM_Term term) {
    IGNORE_ARG(term);

    #define SCOPE "gm_get_key"

    char sequence[MAX_KEY_SEQUENCE] = { 0 };
    uint8_t ind = 0;

    int flush = 0;
    while (gm_kbhit()) {
        char part = gm_getch();
        
        if (flush) continue;
        if (part == '\033' && ind > 2) {
            flush = 1;
            continue;
        }

        sequence[ind++] = part;

        // Add preemptive exits for bracketed paste headers.
        if (STRING_EQUAL(sequence, GM_CAP_BRACKETED_PASTE_START)) return GM_KEY_PASTE_START;
        if (STRING_EQUAL(sequence, GM_CAP_BRACKETED_PASTE_END))   return GM_KEY_PASTE_END;
    }

    // Ambiguous sequences
    // if (STRING_EQUAL(sequence, GM_CAP_KEY_ENTER)) return GM_KEY_ENTER;

    if (ind == 1) {
        // For some fucking reason, enter is not the same sequence returned by terminfo. Fuck it ig.
        if (sequence[0] == '\r') return GM_KEY_ENTER;

        if (isalpha(sequence[0]) && CHAR_IS_UPPER(sequence[0])) return char_tolower(sequence[0]) | GM_MOD_SHIFT;
        if (GM_IS_CTRL(sequence[0])) return sequence[0] | GM_MOD_CTRL;

        return sequence[0];
    } else {
        if (STRING_EQUAL(sequence, GM_CAP_KEY_F1))  return GM_KEY_F1;
        if (STRING_EQUAL(sequence, GM_CAP_KEY_F2))  return GM_KEY_F2;
        if (STRING_EQUAL(sequence, GM_CAP_KEY_F3))  return GM_KEY_F3;
        if (STRING_EQUAL(sequence, GM_CAP_KEY_F4))  return GM_KEY_F4;
        if (STRING_EQUAL(sequence, GM_CAP_KEY_F5))  return GM_KEY_F5;
        if (STRING_EQUAL(sequence, GM_CAP_KEY_F6))  return GM_KEY_F6;
        if (STRING_EQUAL(sequence, GM_CAP_KEY_F7))  return GM_KEY_F7;
        if (STRING_EQUAL(sequence, GM_CAP_KEY_F8))  return GM_KEY_F8;
        if (STRING_EQUAL(sequence, GM_CAP_KEY_F9))  return GM_KEY_F9;
        if (STRING_EQUAL(sequence, GM_CAP_KEY_F10)) return GM_KEY_F10;
        if (STRING_EQUAL(sequence, GM_CAP_KEY_F11)) return GM_KEY_F11;
        if (STRING_EQUAL(sequence, GM_CAP_KEY_F12)) return GM_KEY_F12;

        if (STRING_EQUAL(sequence, GM_CAP_KEY_ARROW_UP))    return GM_KEY_ARROW_UP;
        if (STRING_EQUAL(sequence, GM_CAP_KEY_ARROW_DOWN))  return GM_KEY_ARROW_DOWN;
        if (STRING_EQUAL(sequence, GM_CAP_KEY_ARROW_LEFT))  return GM_KEY_ARROW_LEFT;
        if (STRING_EQUAL(sequence, GM_CAP_KEY_ARROW_RIGHT)) return GM_KEY_ARROW_RIGHT;

        if (STRING_EQUAL(sequence, GM_CAP_KEY_ARROW_LEFT_SHIFT))  return GM_KEY_ARROW_LEFT | GM_MOD_SHIFT;
        if (STRING_EQUAL(sequence, GM_CAP_KEY_ARROW_RIGHT_SHIFT)) return GM_KEY_ARROW_RIGHT | GM_MOD_SHIFT;

        if (STRING_EQUAL(sequence, GM_CAP_KEY_ARROW_UP_CTRL))    return GM_KEY_ARROW_UP | GM_MOD_CTRL;
        if (STRING_EQUAL(sequence, GM_CAP_KEY_ARROW_DOWN_CTRL))  return GM_KEY_ARROW_DOWN | GM_MOD_CTRL;
        if (STRING_EQUAL(sequence, GM_CAP_KEY_ARROW_LEFT_CTRL))  return GM_KEY_ARROW_LEFT | GM_MOD_CTRL;
        if (STRING_EQUAL(sequence, GM_CAP_KEY_ARROW_RIGHT_CTRL)) return GM_KEY_ARROW_RIGHT | GM_MOD_CTRL;

        if (STRING_EQUAL(sequence, GM_CAP_KEY_ARROW_UP_SHIFT_CTRL))    return GM_KEY_ARROW_UP | GM_MOD_SHIFT;
        if (STRING_EQUAL(sequence, GM_CAP_KEY_ARROW_DOWN_SHIFT_CTRL))  return GM_KEY_ARROW_DOWN | GM_MOD_SHIFT;
        if (STRING_EQUAL(sequence, GM_CAP_KEY_ARROW_LEFT_SHIFT_CTRL))  return GM_KEY_ARROW_LEFT | GM_MOD_SHIFT;
        if (STRING_EQUAL(sequence, GM_CAP_KEY_ARROW_RIGHT_SHIFT_CTRL)) return GM_KEY_ARROW_RIGHT | GM_MOD_SHIFT;
    }

    return GM_KEY_NUL;
    #undef SCOPE
}

GM_Key gm_get_canonical_key(GM_Key key) {
    int ckey = GM_CANON_KEY(key);
    if (isalpha(ckey) && GM_HAS_SHIFT(key)) return char_toupper(ckey);
    
    return ckey;
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

void enable_bracketed_paste(GM_Term term) {
    printf("\e[?2004h");
    fflush(stdout);
}

void disable_bracketed_paste(GM_Term term) {
    printf("\e[?2004l");
    fflush(stdout);
}