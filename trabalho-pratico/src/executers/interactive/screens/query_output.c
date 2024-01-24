#include "executers/interactive/screens/query_output.h"
#include "queries/queries.h"
#include "util/math.h"

#define CUR_PAGE_PSTR_KEY "cur_page_pstr"
#define CUR_PAGE_KEY "cur_page"
#define MAX_PAGE_PSTR_KEY "max_page_pstr"
#define MAX_PAGE_KEY "max_page"
#define PSTR_LEN_KEY "pstr_len"
#define QUERY_PAGES_KEY "query_pages"

#define PAGE_BREAK_HEADER ">>> "
#define PAGE_BREAK_HEADER_LEN 4

// ======= Control Bar =======
#define BATCH_STEP 5

#define SPACING_BUF "                                        " // Maximum spacing for control bar.
#define PADDING_BUF "00000000000"

// Right button Group
#define START_BTN "<|"
#define START_BTN_LEN 2
#define BACK_BATCH_BTN "<<"
#define BACK_BATCH_BTN_LEN 2
#define BACK_BTN "<"
#define BACK_BTN_LEN 1

// Left button Group
#define END_BTN "|>"
#define END_BTN_LEN 2
#define FORWARD_BATCH_BTN ">>"
#define FORWARD_BATCH_BTN_LEN 2
#define FORWARD_BTN ">"
#define FORWARD_BTN_LEN 1

// The size of the group of buttons
#define GROUP_LEN START_BTN_LEN + BACK_BATCH_BTN_LEN + BACK_BTN_LEN

// ======= Helper Defines =======
#define DISABLE(stmt) {\
    gm_attron(term, GM_COLOR_PAIR(COLORPAIR_DISABLED));\
    (stmt);\
    gm_attroff(term, GM_COLOR_PAIR(COLORPAIR_DISABLED));\
}
#define ENABLE(stmt) { stmt; }
#define COND_ENABLE(cond, stmt) {\
    if (cond) ENABLE(stmt)\
    else DISABLE(stmt)\
}

// Forward declarations
static void calculate_pages(Cache cache, int cur_page, int max_page);
static void prepare_pages(GM_Term term, Cache cache, GArray* query_pages_source);

#pragma GCC push_options
#pragma GCC optimize ("O0")

Cache make_cache_query_output(GM_Term term, FrameStore store) {
    IGNORE_ARG(term);
    IGNORE_ARG(store);

    Cache cache = make_cache(NULL);

    // ----- Current page -----
    int* cur_page = (int*)malloc(sizeof(int));
    *cur_page = 0;
    add_cache_elem(cache, CUR_PAGE_KEY, cur_page);

    char** cur_page_pstr = (char**)malloc(sizeof(char*));
    *cur_page_pstr = NULL;
    add_cache_elem(cache, CUR_PAGE_PSTR_KEY, cur_page_pstr);

    // ----- Maximum page -----
    int* max_page = (int*)malloc(sizeof(int));
    *max_page = 0;
    add_cache_elem(cache, MAX_PAGE_KEY, max_page);

    char** max_page_pstr = (char**)malloc(sizeof(char*));
    *max_page_pstr = NULL;
    add_cache_elem(cache, MAX_PAGE_PSTR_KEY, max_page_pstr);

    // ----- Page string length -----
    int* pstr_len = (int*)malloc(sizeof(int));
    *pstr_len = 0;
    add_cache_elem(cache, PSTR_LEN_KEY, pstr_len);

    // ----- Query pages -----
    GArray** query_pages = (GArray**)malloc(sizeof(GArray*));
    *query_pages = NULL;
    add_cache_elem(cache, QUERY_PAGES_KEY, query_pages);

    // TODO: DEV ONLY
    // GArray* query_pages_source = query_run_single("1 JéssiTavares910", 0, store->datasets);
    GArray* query_pages_source = query_run_single(store->current_query, 0, store->datasets);

    prepare_pages(term, cache, query_pages_source);
    calculate_pages(cache, 0, query_pages_source->len - 1);

    for (guint i = 0; i < query_pages_source->len; i++) free(g_array_index(query_pages_source, void*, i));
    g_array_free(query_pages_source, TRUE);

    return cache;
}

void destroy_cache_query_output(Cache cache, int force) {
    IGNORE_ARG(force);
    
    // ----- Current page -----
    int* cur_page = get_cache_elem(cache, CUR_PAGE_KEY);
    free(cur_page);

    char** cur_page_pstr = get_cache_elem(cache, CUR_PAGE_PSTR_KEY);
    if (*cur_page_pstr != NULL) free(*cur_page_pstr);
    free(cur_page_pstr);

    // ----- Maximum page -----
    int* max_page = get_cache_elem(cache, MAX_PAGE_KEY);
    free(max_page);

    char** max_page_pstr = get_cache_elem(cache, MAX_PAGE_PSTR_KEY);
    if (*max_page_pstr != NULL) free(*max_page_pstr);
    free(max_page_pstr);

    // ----- Page string length -----
    int* pstr_len = get_cache_elem(cache, PSTR_LEN_KEY);
    free(pstr_len);

    // ----- Query pages -----
    GArray** query_pages = get_cache_elem(cache, QUERY_PAGES_KEY);
    if (*query_pages != NULL) {
        for (guint i = 0; i < (*query_pages)->len; i++) {
            Tokens tokens = g_array_index((*query_pages), Tokens, i);
            destroy_tokens(tokens);
        }

        g_array_free(*query_pages, TRUE);
    }
    free(query_pages);
}

void draw_query_output(GM_Term term, FrameStore store, Cache cache) {
    IGNORE_ARG(term);
    IGNORE_ARG(store);
    IGNORE_ARG(cache);

    GM_TERM_SIZE size = gm_term_get_size(term);

    int* cur_page = get_cache_elem(cache, CUR_PAGE_KEY);
    char** cur_page_pstr = get_cache_elem(cache, CUR_PAGE_PSTR_KEY);

    int* max_page = get_cache_elem(cache, MAX_PAGE_KEY);
    char** max_page_pstr = get_cache_elem(cache, MAX_PAGE_PSTR_KEY);

    int* pstr_len = get_cache_elem(cache, PSTR_LEN_KEY);
    GArray** query_pages = get_cache_elem(cache, QUERY_PAGES_KEY);

    gm_attroff(term, GM_RESET);

    // Draw surrounding box
    gm_box(term, 0, 0, size.rows - 1, size.cols - 1);

    // ======= Draw Query Data =======
    gm_attron(term, GM_PRINT_OVERFLOW_BREAK);
    
    int start_row = 1;
    Tokens page = g_array_index(*query_pages, Tokens, *cur_page);
    for (int i = 0; i < page->len; i++) {
        if (page->data[i][0] == '\0') continue;
        gm_printf(term, start_row++, 1, page->data[i]);

        if (STRING_BEGIN_EQUAL(page->data[i], PAGE_BREAK_HEADER, PAGE_BREAK_HEADER_LEN)) {
            gm_attron(term, GM_COLOR_PAIR(COLORPAIR_SELECTED));
            gm_printf(term, start_row - 1, 1, PAGE_BREAK_HEADER);
            gm_attroff(term, GM_COLOR_PAIR(COLORPAIR_SELECTED));
        }
    }

    gm_attroff(term, GM_PRINT_OVERFLOW_BREAK);

    // ======= Draw Pagination Controls =======
    // ----- Draw controls -----
    int start_col = (size.cols / 2) - (GROUP_LEN + (*pstr_len) + 4);

    #define SECOND_GROUP_COL start_col + GROUP_LEN + (*pstr_len) + (*pstr_len) + 6
    #define MAX_COL SECOND_GROUP_COL + FORWARD_BTN_LEN + FORWARD_BATCH_BTN_LEN + 3

    gm_printf(term, size.rows - 1, start_col - 1, "%.*s", MAX_COL - start_col + 4, SPACING_BUF);

    // Draw left button group
    COND_ENABLE((*cur_page) > 0, gm_printf(term, size.rows - 1, start_col, START_BTN));
    COND_ENABLE((*cur_page) > 0, gm_printf(term, size.rows - 1, start_col + START_BTN_LEN + 1, BACK_BATCH_BTN));
    COND_ENABLE((*cur_page) > 0, gm_printf(term, size.rows - 1, start_col + START_BTN_LEN + BACK_BATCH_BTN_LEN + 2, BACK_BTN));

    // Draw center page selector
    gm_printf(term, size.rows - 1, start_col + GROUP_LEN + 3, *cur_page_pstr);
    gm_printf(term, size.rows - 1, start_col + GROUP_LEN + (*pstr_len) + 4, "/");
    gm_printf(term, size.rows - 1, start_col + GROUP_LEN + (*pstr_len) + 6, *max_page_pstr);

    // Draw right button group
    COND_ENABLE((*cur_page) < (*max_page), gm_printf(term, size.rows - 1, SECOND_GROUP_COL + 1, FORWARD_BTN));
    COND_ENABLE(
        (*cur_page) < (*max_page), 
        gm_printf(term, size.rows - 1, SECOND_GROUP_COL + FORWARD_BTN_LEN + 2, FORWARD_BATCH_BTN)
    );
    COND_ENABLE(
        (*cur_page) < (*max_page), 
        gm_printf(term, size.rows - 1, SECOND_GROUP_COL + FORWARD_BTN_LEN + FORWARD_BATCH_BTN_LEN + 3, END_BTN)
    );

    #undef SECOND_GROUP_COL
    #undef MAX_COL
}

Keypress_Code keypress_query_output(GM_Term term, FrameStore store, Cache cache, GM_Key key) {
    IGNORE_ARG(term);
    IGNORE_ARG(store);
    IGNORE_ARG(cache);
    IGNORE_ARG(key);

    int* cur_page = get_cache_elem(cache, CUR_PAGE_KEY);
    int* max_page = get_cache_elem(cache, MAX_PAGE_KEY);

    GM_Key ckey = gm_get_canonical_key(key);
    switch (ckey) {
        case GM_KEY_ESCAPE: {
            // TODO: Point to query insertion system.
            store->current_screen = SCREEN_MAIN_MENU;
            return KEY_RECIEVED;
            break;
        }
        case GM_KEY_ARROW_LEFT: {
            if (GM_HAS_SHIFT(key)) { // Start Button
                *cur_page = 0;
            } else if (GM_HAS_CTRL(key)) { // Back Batch Button
                *cur_page -= BATCH_STEP;
                if ((*cur_page) < 0) *cur_page = 0;
            } else { // Back Button
                *cur_page -= 1;
                if ((*cur_page) < 0) *cur_page = 0;
            }
            
            calculate_pages(cache, *cur_page, *max_page);

            return KEY_RECIEVED;
            break;
        }
        case GM_KEY_ARROW_RIGHT: {
            if (GM_HAS_SHIFT(key)) { // End Button
                *cur_page = (*max_page) - 1;
            } else if (GM_HAS_CTRL(key)) { // Forward Batch Button
                *cur_page += BATCH_STEP;
                if ((*cur_page) >= (*max_page)) *cur_page = (*max_page) - 1;
            } else { // Forward Button
                *cur_page += 1;
                if ((*cur_page) >= (*max_page)) *cur_page = (*max_page) - 1;
            }
            
            calculate_pages(cache, *cur_page, *max_page);

            return KEY_RECIEVED;
            break;
        }
    }

    return KEY_SKIP;
}

// ============== HELPER FUNCTIONS ==============
static void calculate_pages(Cache cache, int cur_page, int max_page) {
    int max_num = cur_page > max_page ? cur_page : max_page;
    int max_len = snprintf(NULL, 0, "%d", max_num);

    char* cur_page_str = isnprintf("%d", cur_page);
    int cps_len = max_len - strlen(cur_page_str);

    char* max_page_str = isnprintf("%d", max_page - 1);
    int mps_len = max_len - strlen(max_page_str);

    char* cur_page_pstr = isnprintf("%*.*s%s", cps_len, cps_len, PADDING_BUF, cur_page_str);
    char* max_page_pstr = isnprintf("%*.*s%s", mps_len, mps_len, PADDING_BUF, max_page_str);
    int pstr_len = strlen(cur_page_pstr); // Same length

    // Update cache keys
    char** cpp = get_cache_elem(cache, CUR_PAGE_PSTR_KEY);
    char** mpp = get_cache_elem(cache, MAX_PAGE_PSTR_KEY);
    int* pl = get_cache_elem(cache, PSTR_LEN_KEY);

    if (*cpp != NULL) free(*cpp);
    if (*mpp != NULL) free(*mpp);

    *cpp = cur_page_pstr;
    *mpp = max_page_pstr;
    *pl = pstr_len;

    free(cur_page_str);
    free(max_page_str);
}

static void prepare_pages(GM_Term term, Cache cache, GArray* query_pages_source) {
    GM_TERM_SIZE size = gm_term_get_size(term);

    int* max_page = get_cache_elem(cache, MAX_PAGE_KEY);
    GArray** query_pages_cache = get_cache_elem(cache, QUERY_PAGES_KEY);
    *query_pages_cache = g_array_new(FALSE, FALSE, sizeof(Tokens));

    for (guint i = 0; i < query_pages_source->len; i++) {
        char* page = g_array_index(query_pages_source, char*, i);
        Tokens lines = get_lines(page, strlen(page));
        Tokens nlines = break_lines(lines, size.cols - 2, PAGE_BREAK_HEADER);

        g_array_append_val((*query_pages_cache), nlines);
        destroy_tokens(lines);
    }

    *max_page = query_pages_source->len;
}

#pragma GCC pop_options
