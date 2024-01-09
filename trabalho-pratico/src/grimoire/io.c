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

// Cache capabilities to reduce memory usage
// #define MEMOIZE_CAP(cache, key) memoize_cache_elem(cache, key, tigetstr(key))

// Capabilities returned from terminfo
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
    #define SCOPE "gm_get_key"

    // TODO: Move this out, cannot be freed
    // static Cache key_caps = NULL;
    // if (key_caps == NULL) key_caps = make_cache(free);

    char sequence[MAX_KEY_SEQUENCE] = { 0 };
    uint8_t ind = 0;

    while (gm_kbhit()) {
        sequence[ind++] = gm_getch();
    }

    if (ind == 1) {
        if (isalpha(sequence[0]) && CHAR_IS_UPPER(sequence[0])) return char_tolower(sequence[0]) | GM_MOD_SHIFT;
        if (GM_IS_CTRL(sequence[0])) return sequence[0] | GM_MOD_CTRL;

        return sequence[0];
    } else {
        // if (STRING_EQUAL(sequence, MEMOIZE_CAP(term->key_caps, GM_CAP_KEY_F1)))  return GM_KEY_F1;
        // if (STRING_EQUAL(sequence, MEMOIZE_CAP(term->key_caps, GM_CAP_KEY_F2)))  return GM_KEY_F2;
        // if (STRING_EQUAL(sequence, MEMOIZE_CAP(term->key_caps, GM_CAP_KEY_F3)))  return GM_KEY_F3;
        // if (STRING_EQUAL(sequence, MEMOIZE_CAP(term->key_caps, GM_CAP_KEY_F4)))  return GM_KEY_F4;
        // if (STRING_EQUAL(sequence, MEMOIZE_CAP(term->key_caps, GM_CAP_KEY_F5)))  return GM_KEY_F5;
        // if (STRING_EQUAL(sequence, MEMOIZE_CAP(term->key_caps, GM_CAP_KEY_F6)))  return GM_KEY_F6;
        // if (STRING_EQUAL(sequence, MEMOIZE_CAP(term->key_caps, GM_CAP_KEY_F7)))  return GM_KEY_F7;
        // if (STRING_EQUAL(sequence, MEMOIZE_CAP(term->key_caps, GM_CAP_KEY_F8)))  return GM_KEY_F8;
        // if (STRING_EQUAL(sequence, MEMOIZE_CAP(term->key_caps, GM_CAP_KEY_F9)))  return GM_KEY_F9;
        // if (STRING_EQUAL(sequence, MEMOIZE_CAP(term->key_caps, GM_CAP_KEY_F10))) return GM_KEY_F10;
        // if (STRING_EQUAL(sequence, MEMOIZE_CAP(term->key_caps, GM_CAP_KEY_F11))) return GM_KEY_F11;
        // if (STRING_EQUAL(sequence, MEMOIZE_CAP(term->key_caps, GM_CAP_KEY_F12))) return GM_KEY_F12;

        // if (STRING_EQUAL(sequence, gm_term_get_key_cap(term, GM_CAP_KEY_F1)))  return GM_KEY_F1;
        // if (STRING_EQUAL(sequence, gm_term_get_key_cap(term, GM_CAP_KEY_F2)))  return GM_KEY_F2;
        // if (STRING_EQUAL(sequence, gm_term_get_key_cap(term, GM_CAP_KEY_F3)))  return GM_KEY_F3;
        // if (STRING_EQUAL(sequence, gm_term_get_key_cap(term, GM_CAP_KEY_F4)))  return GM_KEY_F4;
        // if (STRING_EQUAL(sequence, gm_term_get_key_cap(term, GM_CAP_KEY_F5)))  return GM_KEY_F5;
        // if (STRING_EQUAL(sequence, gm_term_get_key_cap(term, GM_CAP_KEY_F6)))  return GM_KEY_F6;
        // if (STRING_EQUAL(sequence, gm_term_get_key_cap(term, GM_CAP_KEY_F7)))  return GM_KEY_F7;
        // if (STRING_EQUAL(sequence, gm_term_get_key_cap(term, GM_CAP_KEY_F8)))  return GM_KEY_F8;
        // if (STRING_EQUAL(sequence, gm_term_get_key_cap(term, GM_CAP_KEY_F9)))  return GM_KEY_F9;
        // if (STRING_EQUAL(sequence, gm_term_get_key_cap(term, GM_CAP_KEY_F10))) return GM_KEY_F10;
        // if (STRING_EQUAL(sequence, gm_term_get_key_cap(term, GM_CAP_KEY_F11))) return GM_KEY_F11;
        // if (STRING_EQUAL(sequence, gm_term_get_key_cap(term, GM_CAP_KEY_F12))) return GM_KEY_F12;
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

void gm_hide_cursor(GM_Term term) {
    // char* civis = gm_term_get_key_cap(term, "civis");

    printf(cursor_invisible);
    fflush(stdout);
}

void gm_show_cursor(GM_Term term) {
    // char* cnorm = gm_term_get_key_cap(term, "cnorm");

    printf(cursor_normal);
    fflush(stdout);
}

void gm_clear(int flush) {
    printf(clear_screen);
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