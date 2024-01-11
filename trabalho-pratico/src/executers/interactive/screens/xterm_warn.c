#include "executers/interactive/interactive_common.h"

int draw_xterm_warn(GM_Term term) {
    GM_TERM_SIZE size = gm_term_get_size(term);

    gm_attroff(term, GM_RESET);

    gm_box(term, 0, 0, size.rows - 1, size.cols - 2);
    
    char* text = "          The current terminal is not supported.         \n"
                 "There is no guarantee this program will work as expected.\n"
                 "                Press any key to continue.               ";
    int lines = 3;
    int len = 59;

    int x = (size.cols - len) / 2;
    int y = (size.rows - lines - 1) / 2;

    gm_printf(term, y, x, text);
}
