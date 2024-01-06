#ifndef TP_GRIMOIRE_PUBLIC_H
#define TP_GRIMOIRE_PUBLIC_H

/* ============== CONSTANTS ============== */
#define MAX_ANSI_SEQ 17
#define MAX_UTF8_SEQ 4 // Max UTF8 char sequence
#define MAX_UTF8_SEQ_CORE (MAX_UTF8_SEQ - 1) // Max UTF8 char sequence without the null-terminator
#define MAX_CHAR_SEQ_BYTES (MAX_ANSI_SEQ + MAX_UTF8_SEQ) // Maximum bytes per character

/* ============== TYPEDEFS ============== */

// NOTE to self: I'll commit a hate crime if I have to use widechars ever again.
#ifdef GM_WIDECHAR
    #include <wchar.h>
    typedef wchar_t GM_Char;
#else
    typedef char GM_Char;
#endif

typedef struct gm_term GM_TERM, *GM_Term;
typedef struct gm_attr GM_ATTR, *GM_Attr;

typedef struct gm_term_size {
    unsigned short int rows;
    unsigned short int cols;
} GM_TERM_SIZE;

#endif