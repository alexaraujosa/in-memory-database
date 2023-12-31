#ifndef TP_GRIMOIRE_PUBLIC_H
#define TP_GRIMOIRE_PUBLIC_H

/* ============== TYPEDEFS ============== */
typedef struct gm_term GM_TERM, *GM_Term;
typedef struct gm_attr GM_ATTR, *GM_Attr;

typedef struct gm_term_size {
    unsigned short int rows;
    unsigned short int cols;
} GM_TERM_SIZE;

#endif