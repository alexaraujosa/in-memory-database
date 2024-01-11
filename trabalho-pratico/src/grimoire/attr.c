#include "grimoire/attr.h"
#include "grimoire/grimoire_priv.h"
#include "grimoire/color.h"
#include "grimoire/io.h"

#include "common.h"
#include "util/error.h"
#include "util/string.h"

#define ANSI_TRUECOLOR_FG(r, g, b) isnprintf("38;2;%d;%d;%d;", r, g, b)
#define ANSI_TRUECOLOR_FG_CHAINABLE(r, g, b) isnprintf("\x1B[38;2;%d;%d;%d;m", r, g, b)

#define ANSI_TRUECOLOR_BG(r, g, b) isnprintf("\x1B[48;2;%d;%d;%d;m", r, g, b)
#define ANSI_TRUECOLOR_BG_CHAINABLE(r, g, b) isnprintf("48;2;%d;%d;%d;", r, g, b)

#define ANSI_TRUECOLOR_FULL(fr, fg, fb, br, bg, bb) isnprintf("\x1B[48;2;%d;%d;%d;38;2;%d;%d;%dm", fr, fg, fb, br, bg, bb)
#define ANSI_TRUECOLOR_FULL_CHAINABLE(fr, fg, fb, br, bg, bb) isnprintf("48;2;%d;%d;%d;38;2;%d;%d;%d;", fr, fg, fb, br, bg, bb)

// ======= ATTRON/OFF =======
void gm_attron(GM_Term term, int attr) {
    #define SCOPE "gm_attron"
    if (attr == GM_RESET) {
        term->attr = GM_RESET;
        return;
    }

    uint8_t color = attr >> _GM_COLOR_OFFSET;

    if (color > 0) {
        gm_init_color_defaults(term);

        rt_assert(
            gm_has_color_pair(term, color - 1),
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
    if (attr == GM_RESET) {
        term->attr = GM_RESET;
        return;
    }

    uint8_t color = attr >> _GM_COLOR_OFFSET;

    if (color > 0) {
        term->attr &= ~(0xFF << _GM_COLOR_OFFSET);
    }

    if (attr & GM_BOLD) term->attr ^= GM_BOLD;
    if (attr & GM_ITALIC) term->attr ^= GM_ITALIC;
    if (attr & GM_UNDERLINE) term->attr ^= GM_UNDERLINE;
    #undef SCOPE
}
// ======= END ATTRON/OFF =======

// ======= BOX DRAWING CHARS =======
// void gm_set_box_top_left_corner(GM_Term term, GM_Char ch)     { term->box_chars.tlc = ch; }
// void gm_set_box_top_right_corner(GM_Term term, GM_Char ch)    { term->box_chars.trc = ch; }
// void gm_set_box_bottom_left_corner(GM_Term term, GM_Char ch)  { term->box_chars.blc = ch; }
// void gm_set_box_bottom_right_corner(GM_Term term, GM_Char ch) { term->box_chars.brc = ch; }
// void gm_set_box_left_intersection(GM_Term term, GM_Char ch)   { term->box_chars.il = ch; }
// void gm_set_box_right_intersection(GM_Term term, GM_Char ch)  { term->box_chars.ir = ch; }
// void gm_set_box_top_intersection(GM_Term term, GM_Char ch)    { term->box_chars.it = ch; }
// void gm_set_box_bottom_intersection(GM_Term term, GM_Char ch) { term->box_chars.ib = ch; }
// void gm_set_box_center_intersection(GM_Term term, GM_Char ch) { term->box_chars.ic = ch; }
void gm_set_box_top_left_corner(GM_Term term, char ch[MAX_UTF8_SEQ])     { memcpy(term->box_chars.tlc, ch, MAX_UTF8_SEQ); }
void gm_set_box_top_right_corner(GM_Term term, char ch[MAX_UTF8_SEQ])    { memcpy(term->box_chars.trc, ch, MAX_UTF8_SEQ); }
void gm_set_box_bottom_left_corner(GM_Term term, char ch[MAX_UTF8_SEQ])  { memcpy(term->box_chars.blc, ch, MAX_UTF8_SEQ); }
void gm_set_box_bottom_right_corner(GM_Term term, char ch[MAX_UTF8_SEQ]) { memcpy(term->box_chars.brc, ch, MAX_UTF8_SEQ); }
void gm_set_box_left_intersection(GM_Term term, char ch[MAX_UTF8_SEQ])   { memcpy(term->box_chars.il, ch, MAX_UTF8_SEQ); }
void gm_set_box_right_intersection(GM_Term term, char ch[MAX_UTF8_SEQ])  { memcpy(term->box_chars.ir, ch, MAX_UTF8_SEQ); }
void gm_set_box_top_intersection(GM_Term term, char ch[MAX_UTF8_SEQ])    { memcpy(term->box_chars.it, ch, MAX_UTF8_SEQ); }
void gm_set_box_bottom_intersection(GM_Term term, char ch[MAX_UTF8_SEQ]) { memcpy(term->box_chars.ib, ch, MAX_UTF8_SEQ); }
void gm_set_box_center_intersection(GM_Term term, char ch[MAX_UTF8_SEQ]) { memcpy(term->box_chars.ic, ch, MAX_UTF8_SEQ); }

// ======= END BOX DRAWING CHARS =======

// ======= ATTRIBUTE FACTORY =======
GM_Attr gm_attr_make(
    GM_Term term, 
    GM_AttrTypes type,
    int row_start,
    int col_start,
    int row_end,
    int col_end,
    GM_ATTR_RESOLVER(resolver),
    void* print_data
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
    attr->print_data = print_data;

    return attr;
}

void gm_attr_destroy(GM_Attr attr) {
    free(attr);
} 

void _gm_attr_destroy_notify(gpointer data) {
    gm_attr_destroy((GM_Attr)data);
} 

inline void gm_term_attr_add(GM_Term term, GM_Attr attr) {
    g_queue_push_tail(term->attr_queue, attr);
}

inline GM_Attr gm_term_attr_get(GM_Term term) {
    return (GM_Attr)g_queue_pop_head(term->attr_queue);
}

inline void gm_term_attr_reset(GM_Term term) {
    g_queue_clear_full(term->attr_queue, _gm_attr_destroy_notify);
}
// ======= END ATTRIBUTE FACTORY =======

// ======= RESOLVERS =======
char* gm_attr_int_resolve(GM_Term term, GM_AttrInt attr) {
    if (attr & GM_RESET) return "";

    uint8_t color_int = (attr >> _GM_COLOR_OFFSET) - 1;
    GM_ColorPair pair = gm_get_color_pair(term, color_int);
    RGBColor fg = gm_get_color(term, pair->fg_color);
    RGBColor bg = gm_get_color(term, pair->bg_color);
    // char* color_str = ANSI_TRUECOLOR_FULL_CHAINABLE(fg->red, fg->green, fg->blue, bg->red, bg->green, bg->blue);

    char* color_str;
    if (fg->mod == GM_CMOD_RESET_FG && bg->mod == GM_CMOD_RESET_BG) {
        color_str = isnprintf("%s", "39;49;"); // Add literal to heap to be freed below
    } else if (fg->mod == GM_CMOD_RESET_FG) {
        char* bg_str = ANSI_TRUECOLOR_BG_CHAINABLE(bg->red, bg->green, bg->blue);
        color_str = isnprintf("39;%s", bg_str);
        free(bg_str);
    } else if (bg->mod == GM_CMOD_RESET_BG) {
        char* fg_str = ANSI_TRUECOLOR_FG_CHAINABLE(bg->red, bg->green, bg->blue);
        color_str = isnprintf("%s49", fg_str);
        free(fg_str);
    } else {
        color_str = ANSI_TRUECOLOR_FULL_CHAINABLE(fg->red, fg->green, fg->blue, bg->red, bg->green, bg->blue);
    }

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
    free(color_str);

    if (attr & GM_BOLD)      { strcpy(str + ind, "1;"); ind += 2; }
    if (attr & GM_ITALIC)    { strcpy(str + ind, "3;"); ind += 2; }
    if (attr & GM_UNDERLINE) { strcpy(str + ind, "4;"); ind += 2; }
    if (attr & GM_BLINK)     { strcpy(str + ind, "5;"); ind += 2; }

    if (ind > 2) ind--;

    strcpy(str + ind, "m");

    return str;
}

void gm_attr_resolve_line(GM_Term term, GM_Attr attr) {
    gm_gotoxy(attr->col_start + 1, attr->row_start + 1, FALSE);

    #ifdef _DEBUG
    fflush(stdout);
    #endif

    char* attr_str = gm_attr_int_resolve(term, attr->data);

    // char* print_data = attr->print_data;
    // _gm_printf("%s%.*s", attr_str, attr->col_end - attr->col_start, print_data);
    // gm_reset_attr(FALSE);
    
    Tokens print_data = attr->print_data;
    for (int i = 0; i < print_data->len; i++) {
        gm_gotoxy(attr->col_start + 1, attr->row_start + i + 1, FALSE);
        _gm_printf("%s%.*s", attr_str, attr->col_end - attr->col_start, print_data->data[i]);
        gm_reset_attr(FALSE);
    }

    free(attr_str);
    // free(print_data);
    destroy_tokens(print_data);
}

void gm_attr_resolve_box(GM_Term term, GM_Attr attr) {
    char* attr_str = gm_attr_int_resolve(term, attr->data);

    gm_gotoxy(attr->col_start + 1, attr->row_start + 1, FALSE);
    _gm_printf("%s", attr_str);
    _gm_printf("%s", term->box_chars.tlc);
    for (int i = attr->col_start + 1; i < attr->col_end; i++) _gm_printf("%s", term->box_chars.hl);
    _gm_printf("%s", term->box_chars.trc);

    for (int i = attr->row_start + 1; i < attr->row_end; i++) {
        gm_gotoxy(attr->col_start + 1, i + 1, FALSE);
        _gm_printf("%s%s", attr_str, term->box_chars.vl);
        // gm_clear_attr();
        gm_reset_attr(FALSE);

        gm_gotoxy(attr->col_end + 1, i + 1, FALSE);
        _gm_printf("%s%s", attr_str, term->box_chars.vl);
        // gm_clear_attr();
        gm_reset_attr(FALSE);
    }

    gm_gotoxy(attr->col_start + 1, attr->row_end + 1, FALSE);
    _gm_printf("%s", attr_str);
    _gm_printf("%s", term->box_chars.blc);
    for (int i = attr->col_start + 1; i < attr->col_end; i++) _gm_printf("%s", term->box_chars.hl);
    _gm_printf("%s", term->box_chars.brc);

    gm_reset_attr(FALSE);

    free(attr_str);
}

inline void gm_attr_resolve(GM_Term term, GM_Attr attr) {
    attr->resolver(term, attr);
}
// ======= END RESOLVERS =======