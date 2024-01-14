#include "executers/interactive/screens/screens.h"
#include "executers/interactive/screens/xterm_warn.h"


ScreenDrawFunction manage_screen(ScreenId id, GM_Term term, FrameStore store) {
    static volatile int calls = 0;
    calls++;

    switch (id) {
        case SCREEN_XTERM_WARN: {
            Cache cache = NULL;
            if (!g_hash_table_contains(store->screen_caches, SCREEN_XTERM_WARN)) {
                Cache _cache = make_cache_xterm_warn(term, store);
                g_hash_table_insert(store->screen_caches, SCREEN_XTERM_WARN, _cache);
                cache = _cache;
            } else {
                cache = g_hash_table_lookup(store->screen_caches, SCREEN_XTERM_WARN);
            }

            draw_xterm_warn(term, store, cache);
            break;
        }
    }
}

void _destroy_screen_caches_ghf(gpointer key, gpointer value, gpointer user_data) {
    IGNORE_ARG(key);
    IGNORE_ARG(user_data);

    switch ((ScreenId)key) {
        case SCREEN_XTERM_WARN: { destroy_cache_xterm_warn((Cache)value); break; }
    }

    destroy_cache((Cache)value);
}
void destroy_screens(GM_Term term, FrameStore store) {
    g_hash_table_foreach(store->screen_caches, _destroy_screen_caches_ghf, NULL);
    g_hash_table_destroy(store->screen_caches);
}