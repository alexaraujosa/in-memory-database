#include "grimoire/attr.h"
#include "grimoire/grimoire_priv.h"
#include "grimoire/color.h"

#include "common.h"
#include "util/error.h"

#define ANSI_TRUECOLOR_FG(r, g, b) "\x1B[38;2;" #r ";" #g ";" #b "m"
#define ANSI_TRUECOLOR_FG_CHAINABLE(r, g, b) "38;2;" #r ";" #g ";" #b ";"
#define ANSI_TRUECOLOR_BG(r, g, b) "\x1B[48;2;" #r ";" #g ";" #b "m"
#define ANSI_TRUECOLOR_BG_CHAINABLE(r, g, b) "48;2;" #r ";" #g ";" #b ";"
// #define ANSI_TRUECOLOR_FULL(fr, fg, fb, br, bg, bb) "\x1B[48;2;" #fr ";" #fg ";" #fb ";38;2;" #br ";" #bg ";" #bb "m"

#define ANSI_TRUECOLOR_FULL(fr, fg, fb, br, bg, bb) isnprintf("\x1B[48;2;%d;%d;%d;38;2;%d;%d;%dm", fr, fg, fb, br, bg, bb)
#define ANSI_TRUECOLOR_FULL_CHAINABLE(fr, fg, fb, br, bg, bb) isnprintf("48;2;%d;%d;%d;38;2;%d;%d;%d;", fr, fg, fb, br, bg, bb)


// typedef enum {
//     GM_ATTR_TYPE_LINE,
//     GM_ATTR_TYPE_length
// } GM_AttrTypes;

// ======= ATTRON/OFF =======
void gm_attron(GM_Term term, int attr) {
    #define SCOPE "gm_attron"
    if (attr == GM_RESET) {
        term->attr = GM_RESET;
        return;
    }

    uint8_t color = attr >> _GM_COLOR_OFFSET;

    // if (color != 0) {
        // GM_ColorPair cp = g_hash_table_lookup(term->color_pairs, GINT_TO_POINTER(color));
        // if (cp == NULL) {
        //     // TODO: Trace?
        //     return;
        // }

        // rt_assert(
        //     gm_has_color_pair(term, color),
        //     trace_msg(SCOPE, "Attempted to set attribute flag for non-existent color pair.")
        // );

        // term->attr |= (color & 0xFF) << _GM_COLOR_OFFSET;
    // }

    if (color > 0) {
        gm_init_color_defaults(term);

        rt_assert(
            gm_has_color_pair(term, color),
            trace_msg(SCOPE, "Attempted to set attribute flag for non-existent color pair.")
        );
        
        term->attr &= ~(0xFF << _GM_COLOR_OFFSET); // Reset previous color
        term->attr |= (color & 0xFF) << _GM_COLOR_OFFSET;
    }

    if (attr & GM_BOLD) term->attr |= GM_BOLD;
    if (attr & GM_ITALIC) term->attr |= GM_ITALIC;
    if (attr & GM_UNDERLINE) term->attr |= GM_UNDERLINE;
    #undef SCOPE
}

void gm_attroff(GM_Term term, int attr) {
    #define SCOPE "gm_attroff"
    uint8_t color = attr >> _GM_COLOR_OFFSET;

    // if (color != 0) {
        // GM_ColorPair cp = g_hash_table_lookup(term->color_pairs, GINT_TO_POINTER(color));
        // if (cp == NULL) {
        //     // TODO: Trace?
        //     return;
        // }

        // term->attr &= ~(0xFF << _GM_COLOR_OFFSET);
    // }

    if (color > 0) {
        term->attr &= ~(0xFF << _GM_COLOR_OFFSET);
    }

    if (attr & GM_BOLD) term->attr ^= GM_BOLD;
    if (attr & GM_ITALIC) term->attr ^= GM_ITALIC;
    if (attr & GM_UNDERLINE) term->attr ^= GM_UNDERLINE;
    #undef SCOPE
}
// ======= END ATTRON/OFF =======

// ======= ATTRIBUTE FACTORY =======
GM_Attr gm_attr_make(
    GM_Term term, 
    GM_AttrTypes type,
    int row_start,
    int col_start,
    int row_end,
    int col_end,
    GM_ATTR_RESOLVER(resolver)
) {
    GM_Attr attr = (GM_Attr)malloc(sizeof(GM_ATTR));
    
    if (type < 0 || type >= GM_ATTR_TYPE_length) {
        puts(trace_msg("gm_attr_make", "Attribute type out-of-bounds."));
        return NULL;
    }
    attr->type = type;

    if (row_start < 0 || row_start > term->size.rows) {
        puts(trace_msg("gm_attr_make", "Row start out of terminal bounds."));
        return NULL;
    }
    attr->row_start = row_start;

    if (row_end < 0 || row_end > term->size.rows) {
        puts(trace_msg("gm_attr_make", "Row end out of terminal bounds."));
        return NULL;
    }
    attr->row_end = row_end;
    
    if (col_start < 0 || col_start > term->size.cols) {
        puts(trace_msg("gm_attr_make", "Column start out of terminal bounds."));
        return NULL;
    }
    attr->col_start = col_start;

    if (col_end < 0 || col_end > term->size.cols) {
        puts(trace_msg("gm_attr_make", "Column start out of terminal bounds."));
        return NULL;
    }
    attr->col_end = col_end;

    if (resolver == NULL) {
        puts(trace_msg("gm_attr_make", "Resolver is a required parameter, but got NULL."));
        return NULL;
    }
    attr->resolver = resolver;

    attr->data = term->attr;

    return attr;
}

void gm_attr_destroy(GM_Attr attr) {
    free(attr);
} 

inline void gm_term_attr_add(GM_Term term, GM_Attr attr) {
    g_array_append_val(term->attr_queue, attr);
}

inline GM_Attr gm_term_attr_get(GM_Term term, int ind) {
    return g_array_index(term->attr_queue, GM_Attr, ind);
}

// Is memory even freed here?
inline void gm_term_attr_reset(GM_Term term) {
    for (int i = 0; i <= term->attr_queue->len; i++) g_array_remove_index(term->attr_queue, 0);
}
// ======= END ATTRIBUTE FACTORY =======

// ======= RESOLVERS =======
char* gm_attr_int_resolve(GM_Term term, GM_AttrInt attr) {
    if (attr & GM_RESET) return "";

    uint8_t color_int = (attr >> _GM_COLOR_OFFSET) - 1;
    GM_ColorPair pair = gm_get_color_pair(term, color_int);
    RGBColor fg = gm_get_color(term, pair->fg_color);
    RGBColor bg = gm_get_color(term, pair->bg_color);
    char* color_str = ANSI_TRUECOLOR_FULL_CHAINABLE(fg->red, fg->green, fg->blue, bg->red, bg->green, bg->blue);

    uint8_t chars = 
        strlen(color_str)
        + !!(GM_AttrInt)(attr & GM_BOLD) 
        + !!(GM_AttrInt)(attr & GM_ITALIC) 
        + !!(GM_AttrInt)(attr & GM_UNDERLINE) 
        + !!(GM_AttrInt)(attr & GM_BLINK);
    
    int ind = 2;
    char* str = (char*)calloc(chars * 2 + 1, sizeof(char));
    strcpy(str, "\e[");

    strcpy(str + ind, color_str);
    ind += strlen(color_str);

    if (attr & GM_BOLD)      { strcpy(str + ind, "1;"); ind += 2; }
    if (attr & GM_ITALIC)    { strcpy(str + ind, "3;"); ind += 2; }
    if (attr & GM_UNDERLINE) { strcpy(str + ind, "4;"); ind += 2; }
    if (attr & GM_BLINK)     { strcpy(str + ind, "5;"); ind += 2; }

    if (ind > 2) ind--;

    strcpy(str + ind, "m");

    return str;
}

void gm_attr_resolve_line(GM_Term term, GM_Attr attr) {
    gotoxy(attr->row_start + 1, attr->col_start + 1);

    #ifdef _DEBUG
    fflush(stdout);
    #endif

    char* attr_str = gm_attr_int_resolve(term, attr->data);
    printf("%s%.*s", attr_str, attr->col_end - attr->col_start, term->buf->data[attr->row_start] + attr->col_start);
    clear_attr();
}

inline void gm_attr_resolve(GM_Term term, GM_Attr attr) {
    attr->resolver(term, attr);
}
// ======= END RESOLVERS =======