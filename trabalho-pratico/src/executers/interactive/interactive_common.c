#include "executers/interactive/interactive_common.h"

DrawText make_draw_text(char* text, int len, int x, int y) {
    DrawText dt = (DrawText)malloc(sizeof(DRAW_TEXT));
    dt->text = text;
    dt->len = len;
    dt->x = x;
    dt->y = y;

    return dt;
}

void destroy_draw_text(DrawText dt) {
    free(dt->text);
    free(dt);
}