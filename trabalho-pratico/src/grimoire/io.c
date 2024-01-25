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

#define GM_UTF8_ACCENT_BYTE 0xc3


// I live in Spain, without the S.
// #define GM_CAP_UTF8_LATIN_A_GRAVE  "\xC3\x80" // CAPITAL LETTER A WITH GRAVE (U+00C0) 
// #define GM_CAP_UTF8_LATIN_A_ACUTE  "\xC3\x81" // CAPITAL LETTER A WITH ACUTE (U+00C1)
// #define GM_CAP_UTF8_LATIN_A_CIRC   "\xC3\x82" // CAPITAL LETTER A WITH CIRCUMFLEX (U+00C2)
// #define GM_CAP_UTF8_LATIN_A_TILDE  "\xC3\x83" // CAPITAL LETTER A WITH TILDE (U+00C3)
// #define GM_CAP_UTF8_LATIN_A_UMLAUT "\xC3\x84" // CAPITAL LETTER A WITH DIAERESIS (U+00C4)
// #define GM_CAP_UTF8_LATIN_A_RING   "\xC3\x85" // CAPITAL LETTER A WITH RING ABOVE (U+00C5)
// #define GM_CAP_UTF8_LATIN_AE       "\xC3\x86" // LATIN CAPITAL LETTER AE (U+00C6)
// #define GM_CAP_UTF8_LATIN_C_CEDIL  "\xC3\x87" // LATIN CAPITAL LETTER C WITH CEDILLA (U+00C7)
// #define GM_CAP_UTF8_LATIN_E_GRAVE  "\xC3\x88" // CAPITAL LETTER E WITH GRAVE (U+00C8)
// #define GM_CAP_UTF8_LATIN_E_ACUTE  "\xC3\x89" // CAPITAL LETTER E WITH ACUTE (U+00C9)
// #define GM_CAP_UTF8_LATIN_E_CIRC   "\xC3\x8A" // CAPITAL LETTER E WITH CIRCUMFLEX (U+00CA)
// #define GM_CAP_UTF8_LATIN_E_UMLAUT "\xC3\x8B" // CAPITAL LETTER E WITH DIAERESIS (U+00CB)
// #define GM_CAP_UTF8_LATIN_I_GRAVE  "\xC3\x8C" // CAPITAL LETTER I WITH GRAVE (U+00CC)
// #define GM_CAP_UTF8_LATIN_I_ACUTE  "\xC3\x8D" // CAPITAL LETTER I WITH ACUTE (U+00CD)
// #define GM_CAP_UTF8_LATIN_I_CIRC   "\xC3\x8E" // CAPITAL LETTER I WITH CIRCUMFLEX (U+00CE)
// #define GM_CAP_UTF8_LATIN_I_UMLAUT "\xC3\x8F" // CAPITAL LETTER I WITH DIAERESIS (U+00CF)
// #define GM_CAP_UTF8_LATIN_ETH      "\xC3\x90" // LATIN CAPITAL LETTER ETH (U+00D0)
// #define GM_CAP_UTF8_LATIN_N_TILDE  "\xC3\x91" // CAPITAL LETTER N WITH TILDE (U+00D1)
// #define GM_CAP_UTF8_LATIN_O_GRAVE  "\xC3\x92" // CAPITAL LETTER O WITH GRAVE (U+00D2)
// #define GM_CAP_UTF8_LATIN_O_ACUTE  "\xC3\x93" // CAPITAL LETTER O WITH ACUTE (U+00D3)
// #define GM_CAP_UTF8_LATIN_O_CIRC   "\xC3\x94" // CAPITAL LETTER O WITH CIRCUMFLEX (U+00D4)
// #define GM_CAP_UTF8_LATIN_O_TILDE  "\xC3\x95" // CAPITAL LETTER O WITH TILDE (U+00D5)
// #define GM_CAP_UTF8_LATIN_O_UMLAUT "\xC3\x96" // CAPITAL LETTER O WITH DIAERESIS (U+00D6)
// #define GM_CAP_UTF8_LATIN_MULTIPLY "\xC3\x97" // MULTIPLICATION SIGN (U+00D7)
// #define GM_CAP_UTF8_LATIN_O_STROKE "\xC3\x98" // LATIN CAPITAL LETTER O WITH STROKE (U+00D8)
// #define GM_CAP_UTF8_LATIN_U_GRAVE  "\xC3\x99" // CAPITAL LETTER U WITH GRAVE (U+00D9)
// #define GM_CAP_UTF8_LATIN_U_ACUTE  "\xC3\x9A" // CAPITAL LETTER U WITH ACUTE (U+00DA)
// #define GM_CAP_UTF8_LATIN_U_CIRC   "\xC3\x9B" // CAPITAL LETTER U WITH CIRCUMFLEX (U+00DB)
// #define GM_CAP_UTF8_LATIN_U_UMLAUT "\xC3\x9C" // CAPITAL LETTER U WITH DIAERESIS (U+00DC)
// #define GM_CAP_UTF8_LATIN_Y_ACUTE  "\xC3\x9D" // CAPITAL LETTER Y WITH ACUTE (U+00DD)
// #define GM_CAP_UTF8_LATIN_THORN    "\xC3\x9E" // LATIN CAPITAL LETTER THORN (U+00DE)
// #define GM_CAP_UTF8_LATIN_SHARP_S  "\xC3\x9F" // LATIN SMALL LETTER SHARP S (U+00DF)



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

GM_UTF8_Key _gm_UTF8_2Byte_ToInt(char arr[]) {
    GM_UTF8_Key result = 0;
    
    for (int i = 0; i < sizeof(GM_UTF8_Key); i++) {
        result |= (arr[i] & 0xFF) << (8 * i);
    }
    
    return result;
}

void _gm_int_to_UTF(GM_UTF8_Key value, char arr[]) {
    for (int i = 0; i < sizeof(GM_UTF8_Key); i++) {
        arr[i] = (value >> (8 * i)) & 0xFF;
    }
}

/**
 * @brief Reads and interprets a key sequence from STDIN.
 * 
 * @param readEscape Whenever the escape character from the sequence has already been read.
 */
GM_Key _gm_get_sequence(int readEscape) {
    static int has_utf8_byte = FALSE;

    GString* sequence = g_string_new(readEscape ? "\033" : NULL);

    #define RET_AND_CLEANUP(val) {\
        GM_Key ret = val;\
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

        if ((uint8_t)part == GM_UTF8_ACCENT_BYTE) {
            has_utf8_byte = TRUE;
            break;
        }

        if (!readEscape) break;

        // Add preemptive exits for bracketed paste headers.
        if (STRING_EQUAL(sequence->str, GM_CAP_BRACKETED_PASTE_START)) RET_AND_CLEANUP(GM_KEY_PASTE_START);
        if (STRING_EQUAL(sequence->str, GM_CAP_BRACKETED_PASTE_END))   RET_AND_CLEANUP(GM_KEY_PASTE_END);
    }

    while (has_utf8_byte) {
        if (gm_kbhit()) {
            char part = gm_getch();
            g_string_append_c(sequence, part);

            has_utf8_byte = FALSE;
        }
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
        // UTF8 Characters
        if ((uint8_t)(sequence->str[0]) == GM_UTF8_ACCENT_BYTE) {
            GM_UTF8_Key _key = _gm_UTF8_2Byte_ToInt(sequence->str);
            RET_AND_CLEANUP(_key | GM_MOD_UTF8);
        }

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

char* gm_get_utf8_key(GM_Key key) {
    if (!GM_KEY_IS_UTF8(key)) return NULL;

    char* ret = malloc(3 * sizeof(char));
    memset(ret, 0, 3);

    _gm_int_to_UTF(GM_CANON_KEY(key), ret);

    return ret;
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