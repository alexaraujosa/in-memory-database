#include "executers/interactive/screens/screens.h"
#include "executers/interactive/screens/xterm_warn.h"
#include "executers/interactive/screens/settings.h"
#include "executers/interactive/screens/main_menu.h"
#include "executers/interactive/screens/dataset_question.h"
#include "executers/interactive/screens/loading.h"

void _invalidate_screen_caches(GM_Term term, FrameStore store);

Cache _ensure_screen_cache(ScreenId id, GM_Term term, FrameStore store) {
    Cache cache = NULL;

    if (!g_hash_table_contains(store->screen_caches, GINT_TO_POINTER(id))) {
        switch (id) {
            case SCREEN_XTERM_WARN:        { cache = make_cache_xterm_warn(term, store); break; }
            case SCREEN_SETTINGS:          { cache = make_cache_settings(term, store); break; }
            case SCREEN_MAIN_MENU:         { cache = make_cache_main_menu(term, store); break; }
            case SCREEN_DATASET_QUESTION:  { cache = make_cache_dataset_question(term, store); break; }
            case SCREEN_LOADING:           { cache = make_cache_loading(term, store); break; }
        }

        g_hash_table_insert(store->screen_caches, GINT_TO_POINTER(id), cache);
    } else {
        cache = g_hash_table_lookup(store->screen_caches, GINT_TO_POINTER(id));
        if (is_cache_destroyed(cache)) {
            g_hash_table_remove(store->screen_caches, GINT_TO_POINTER(id));
            free(cache);
            return _ensure_screen_cache(id, term, store);
        }
    }

    return cache;
}

ScreenDrawFunction manage_screen(ScreenId id, GM_Term term, FrameStore store) {
    Cache cache = _ensure_screen_cache(id, term, store);

    switch (id) {
        case SCREEN_XTERM_WARN:         { draw_xterm_warn(term, store, cache); break; }
        case SCREEN_SETTINGS:           { draw_settings(term, store, cache); break; }
        case SCREEN_MAIN_MENU:          { draw_main_menu(term, store, cache); break; }
        case SCREEN_DATASET_QUESTION:   { draw_dataset_question(term, store, cache); break; }
        case SCREEN_LOADING:            { draw_loading(term, store, cache); break; }
        default: {
            // Do fuck all
        }
    }

    return NULL;
}

Keypress_Code keypress_screen(ScreenId id, GM_Term term, FrameStore store, GM_Key key) {
    Cache cache = _ensure_screen_cache(id, term, store);

    switch (id) {
        case SCREEN_XTERM_WARN: { return keypress_xterm_warn(term, store, cache, key); break; }
        case SCREEN_SETTINGS:   { 
            Keypress_Code code = keypress_settings(term, store, cache, key); 
            if (code == KEY_SPECIAL) {
                _invalidate_screen_caches(term, store);
                return KEY_RECIEVED;
            }

            return code;
            break; 
        }
        case SCREEN_MAIN_MENU: { return keypress_main_menu(term, store, cache, key); break; }
        // case SCREEN_DATASET_QUESTION: { 
        //     Keypress_Code code = keypress_dataset_question(term, store, cache, key); 
        //     if (code == KEY_SPECIAL) {
        //         return KEY_RECIEVED;
        //     }

        //     return code;
        //     break; 
        // }
        case SCREEN_DATASET_QUESTION: { return keypress_dataset_question(term, store, cache, key); break; }
        case SCREEN_LOADING: { return keypress_loading(term, store, cache, key); break; }
        default: {
            // Do fuck all
            return KEY_SKIP;
        }
    }
}

void _destroy_screen_caches_ghf(gpointer key, gpointer value, gpointer user_data) {
    IGNORE_ARG(key);
    IGNORE_ARG(user_data);

    switch ((ScreenId)key) {
        case SCREEN_XTERM_WARN:         { destroy_cache_xterm_warn((Cache)value, FALSE); break; }
        case SCREEN_SETTINGS:           { destroy_cache_settings((Cache)value, FALSE); break; }
        case SCREEN_MAIN_MENU:          { destroy_cache_main_menu((Cache)value, FALSE); break; }
        case SCREEN_DATASET_QUESTION:   { destroy_cache_dataset_question((Cache)value, FALSE); break; }
        case SCREEN_LOADING:            { destroy_cache_loading((Cache)value, FALSE); break; }
    }

    destroy_cache((Cache)value, TRUE);
}
void destroy_screens(GM_Term term, FrameStore store) {
    IGNORE_ARG(term);

    g_hash_table_foreach(store->screen_caches, _destroy_screen_caches_ghf, NULL);
    g_hash_table_destroy(store->screen_caches);
}

gboolean _isc_ghr(gpointer key, gpointer value, gpointer user_data) {
    IGNORE_ARG(key);
    IGNORE_ARG(value);
    IGNORE_ARG(user_data);
    return TRUE;
}
void _invalidate_screen_caches(GM_Term term, FrameStore store) {
    IGNORE_ARG(term);

    g_hash_table_foreach(store->screen_caches, _destroy_screen_caches_ghf, NULL);
    g_hash_table_foreach_remove(store->screen_caches, _isc_ghr, NULL);
}