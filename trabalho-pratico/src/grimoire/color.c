#include "grimoire/color.h"
#include "grimoire/grimoire_priv.h"

#include "common.h"
#include "util/error.h"

// ======= ASSERTS =======
int gm_has_color(GM_Term term, int id) {
    #define SCOPE "gm_has_color"
    rt_assert((id >= GM_COLOR_ID_MIN && id <= GM_COLOR_ID_MAX), trace_msg(SCOPE, "Color ID out-of-bounds."));
    return g_hash_table_contains(term->colors, GINT_TO_POINTER(id));
    #undef SCOPE
}

int gm_has_color_pair(GM_Term term, int id) {
    #define SCOPE "gm_has_color_pair"
    rt_assert((id >= GM_COLOR_ID_MIN && id <= GM_COLOR_ID_MAX), trace_msg(SCOPE, "Color ID out-of-bounds."));
    return g_hash_table_contains(term->color_pairs, GINT_TO_POINTER(id));
    #undef SCOPE
}
// ======= END ASSERTS =======

// ======= CONSTRUCTORS =======
RGBColor _gm_make_color(uint8_t r, uint8_t g, uint8_t b) {
    RGBColor color = (RGBColor)malloc(sizeof(RGB_COLOR));
    color->red = r;
    color->green = g;
    color->blue = b;

    return color;
}

GM_ColorPair _gm_make_color_pair(int id, int fg, int bg) {
    GM_ColorPair pair = (GM_ColorPair)malloc(sizeof(GM_COLOR_PAIR_STRUCT));
    pair->id = id;
    pair->fg_color = fg;
    pair->bg_color = bg;

    return pair;
}

void gm_init_color(GM_Term term, int id, uint8_t r, uint8_t g, uint8_t b) {
    #define SCOPE "gm_init_color"
    rt_assert(!gm_has_color(term, id), trace_msg(SCOPE, "Attempted to define already existent color."));
    rt_assert((id >= GM_COLOR_ID_MIN && id <= GM_COLOR_ID_MAX), trace_msg(SCOPE, "Color ID out-of-bounds."));
    // rt_assert((r >= 0 && r <= 255), trace_msg(SCOPE, "Red hue out-of-bounds."));
    // rt_assert((g >= 0 && g <= 255), trace_msg(SCOPE, "Green hue out-of-bounds."));
    // rt_assert((b >= 0 && b <= 255), trace_msg(SCOPE, "Blue hue out-of-bounds."));

    RGBColor color = _gm_make_color(r, g, b);

    g_hash_table_insert(term->colors, GINT_TO_POINTER(id), color);
    #undef SCOPE
}

void gm_init_color_pair(GM_Term term, int id, int fg, int bg) {
    #define SCOPE "gm_init_color"
    rt_assert(!gm_has_color_pair(term, id), trace_msg(SCOPE, "Attempted to define already existent color pair."));
    rt_assert((id >= GM_COLOR_ID_MIN && id <= GM_COLOR_ID_MAX), trace_msg(SCOPE, "Color ID out-of-bounds."));
    rt_assert(gm_has_color(term, fg), trace_msg(SCOPE, "Foreground color not initialized."));
    rt_assert(gm_has_color(term, bg), trace_msg(SCOPE, "Background color not initialized."));

    GM_ColorPair pair = _gm_make_color_pair(id, fg, bg);

    g_hash_table_insert(term->color_pairs, GINT_TO_POINTER(id), pair);
    #undef SCOPE
}

void gm_init_color_defaults(GM_Term term) {
    // Function is a default initializer. If already initialized, the system uses the defined color pair 0.
    if (gm_has_color_pair(term, 0)) return; 

    if (!gm_has_color(term, 0)) gm_init_color(term, 0, 0, 0, 0);
    gm_init_color_pair(term, 0, 0, 0);
}
// ======= END CONSTRUCTORS =======

// ======= FETCHERS =======
RGBColor gm_get_color(GM_Term term, int id) {
    #define SCOPE "gm_get_color"
    rt_assert(gm_has_color(term, id), trace_msg(SCOPE, "Attempted to fetch non-existent color."));
    rt_assert((id >= GM_COLOR_ID_MIN && id <= GM_COLOR_ID_MAX), trace_msg(SCOPE, "Color ID out-of-bounds."));

    RGBColor color = g_hash_table_lookup(term->colors, GINT_TO_POINTER(id));
    return color;
    #undef SCOPE
}

GM_ColorPair gm_get_color_pair(GM_Term term, int id) {
    #define SCOPE "gm_get_color"
    rt_assert(gm_has_color_pair(term, id), trace_msg(SCOPE, "Attempted to fetch non-existent color pair."));
    rt_assert((id >= GM_COLOR_ID_MIN && id <= GM_COLOR_ID_MAX), trace_msg(SCOPE, "Color ID out-of-bounds."));

    GM_ColorPair pair = g_hash_table_lookup(term->color_pairs, GINT_TO_POINTER(id));
    return pair;
    #undef SCOPE
}
// ======= END FETCHERS =======
