#include "executers/interactive/screens/screens.h"
#include "executers/interactive/screens/xterm_warn.h"
#include "executers/interactive/screens/settings.h"


ScreenDrawFunction manage_screen(ScreenId id, GM_Term term, FrameStore store) {
    static volatile int calls = 0;
    calls++;

    Cache cache = NULL;
    if (!g_hash_table_contains(store->screen_caches, GINT_TO_POINTER(id))) {
        switch (id) {
            case SCREEN_XTERM_WARN: { cache = make_cache_xterm_warn(term, store); break; }
            case SCREEN_SETTINGS:   { cache = make_cache_settings(term, store); break; }
        }

        g_hash_table_insert(store->screen_caches, GINT_TO_POINTER(id), cache);
    } else {
        cache = g_hash_table_lookup(store->screen_caches, GINT_TO_POINTER(id));
    }

    switch (id) {
        case SCREEN_XTERM_WARN: { draw_xterm_warn(term, store, cache); break; }
        case SCREEN_SETTINGS:   { draw_settings(term, store, cache); break; }
        default: {
            // Do fuck all
        }
    }
}

void _destroy_screen_caches_ghf(gpointer key, gpointer value, gpointer user_data) {
    IGNORE_ARG(key);
    IGNORE_ARG(user_data);

    switch ((ScreenId)key) {
        case SCREEN_XTERM_WARN: { destroy_cache_xterm_warn((Cache)value); break; }
        case SCREEN_SETTINGS:   { destroy_cache_settings((Cache)value); break; }
    }

    destroy_cache((Cache)value);
}
void destroy_screens(GM_Term term, FrameStore store) {
    IGNORE_ARG(term);

    g_hash_table_foreach(store->screen_caches, _destroy_screen_caches_ghf, NULL);
    g_hash_table_destroy(store->screen_caches);
}