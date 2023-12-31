#include "grimoire/draw.h"
#include "grimoire/attr.h"
#include "grimoire/grimoire_priv.h"

#include "common.h"
#include "util/error.h"

size_t gm_str_visible_len(const char* str) {
    size_t length = 0;

    for (size_t i = 0; str[i] != '\0';) {
        // Check for ANSI escape codes
        if (str[i] == '\x1B' && str[i + 1] == '[') {
            // Skip ANSI escape code
            while (str[i] != 'm') {
                i++;
            }

            i++; // Move past 'm'
        } else {
            // Handle UTF-8 multi-byte characters
            uint8_t byte = (uint8_t)str[i];
            if ((byte & 0xC0) != 0x80) {
                // This is the start of a character
                length++;
            }

            i++;
        }
    }

    return length;
}

// ======= DRAWING FUNCTIONS =======
void gm_printf(GM_Term term, int row, int col, const char *format, ...) {
    va_list args;
    va_start(args, format);

    // TODO: Trace?

    int truepos = gm_term_buf_truepos(term, row, col);

    // Determine the length of the formatted string
    int length = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (length < 0) return;

    va_start(args, format);

    char *result = (char *)malloc(length + 1);
    if (result == NULL) {
        va_end(args);
        return;
    }

    // Format the string into the allocated memory
    vsnprintf(result, length + 1, format, args);
    va_end(args);

    if (row > term->buf->rows) {
        return;
    }
    if (col + length > term->buf->cols * MAX_CHAR_SEQ_BYTES) {
        return;
    }

    int vislen = gm_str_visible_len(result);
    
    memcpy(term->buf->data[row] + col, result, length);
    free(result);
    
    GM_Attr attr = gm_attr_make(
        term, 
        0, 
        row, 
        col, 
        row,
        col + vislen, 
        &gm_attr_resolve_line
    );
    gm_term_attr_add(term, attr);
}
// ======= END DRAWING FUNCTIONS =======
