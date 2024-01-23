#include "executers/interactive/interactive_common.h"

DrawText make_draw_text(char* text, int len, int x, int y, int rows, int cols) {
    DrawText dt = (DrawText)malloc(sizeof(DRAW_TEXT));
    dt->text = text;
    dt->len = len;
    dt->x = x;
    dt->y = y;
    dt->rows = rows;
    dt->cols = cols;

    return dt;
}

void destroy_draw_text(DrawText dt) {
    free(dt->text);
    free(dt);
}


typedef struct defer_control {
    int has_work;
    DeferNotify notify_func;
} DEFER_CONTROL, *DeferControl;

DeferControl make_defer_control() {
    DeferControl df = (DeferControl)malloc(sizeof(DEFER_CONTROL));
    df->has_work = FALSE;
    df->notify_func = NULL;

    return df;
}

void destroy_defer_control(DeferControl df) {
    free(df);
}

void defer_load(DeferControl defer_control, DeferNotify notify_func) {
    if (defer_control->has_work) return;

    defer_control->notify_func = notify_func;
    defer_control->has_work = TRUE;
}

void defer_try(DeferControl defer_control, GM_Term term, FrameStore store) {
    if (!defer_control->has_work) return;

    defer_control->notify_func(term, store);
    defer_control->notify_func = NULL;
    defer_control->has_work = FALSE;
}