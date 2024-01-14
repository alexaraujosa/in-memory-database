#include "data/locale.h"

#define DATA_LOCALE_DIR "lang"
#define DATA_LOCALE_FILE_EXT "lang"
#define DATA_LOCALE_VERSION 1

#define DATA_LOCALE_METAPROP_VERSION "version"
#define DATA_LOCALE_METAPROP_LOC     "loc"
#define DATA_LOCALE_METAPROP_LOCNAME "locname"

typedef struct data_locale {
    char* id;
    char* name;
    GHashTable* translations;
} DATA_LOCALE, *DataLocale;

typedef struct data_locales {
    GHashTable* locales;
    int len;
} DATA_LOCALES, *DataLocales;


enum _ll_metaprop {
    _LL_MP_NONE = 0,
    _LL_MP_VERSION,
    _LL_MP_LOC,
    _LL_MP_LOCNAME
};
typedef struct _ll_data {
    int meta;
    enum _ll_metaprop metaprop;
    char* name;
    char* value;
} _LL_DATA, *_LLData;

// ============== LOAD / DESTROY LOCALE ==============

Tokens _ll_tokenize(char *line, ssize_t len) {
    char* ptr = strdup(line);
    char* ptr_root = ptr;

    if (ptr == NULL) exit(EXIT_FAILURE);

    if (ptr[len - 1] == '\n') {
        ptr[len - 1] = '\0';
    }

    char** arr = (char**)malloc(2 * sizeof(char*));
    memset(arr, 0, 2 * sizeof(char*));

    char* tname = strsep(&ptr, "=");
    arr[0] = strdup(tname);
    arr[1] = strdup(ptr);

    Tokens ret = (Tokens)malloc(sizeof(TOKENS));
    ret->data = arr;
    ret->len = 2;

    free(ptr_root);
    return ret;
}

void _ll_preprocess(FILE* stream, ParserStore store, va_list args) {
    IGNORE_ARG(stream);

    // ------- Line Number -------
    int* line = (int*)malloc(sizeof(int));
    *line = -1;

    g_array_append_vals(store, &line, 1);

    // ------- Locale struct -------
    DataLocale** locale = va_arg(args, DataLocale**);
    DataLocale _loc = (DataLocale)malloc(sizeof(DATA_LOCALE));
    _loc->id = NULL;
    _loc->translations = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);
    **locale = _loc;
    g_array_append_vals(store, &_loc, 1);

    // ------- Locale Path -------
    char* path = va_arg(args, char*);
    char* _path = strdup(path);
    g_array_append_vals(store, &_path, 1);
}

int _ll_verify(Tokens tokens, ParserStore store) {
    #define SCOPE "_ll_verify"
    int* line = g_array_index(store, int*, 0);
    (*line)++;

    DataLocale locale = g_array_index(store, DataLocale, 1);
    char* path = g_array_index(store, char*, 2);

    #define INVALID_LOCALE_FORMAT "%s%s"
    #define INVALID_LOCALE trace_msg(SCOPE, "Invalid locale: "), path
    #define INVALID_LOCALE_LINE_FORMAT "@ l:%d"
    #define INVALID_LOCALE_LINE line

    rt_assert_dyn(
        (tokens->len == 2),
        isnprintf(
            INVALID_LOCALE_FORMAT"%s"INVALID_LOCALE_LINE_FORMAT, 
            INVALID_LOCALE, 
            "\n- Key-Value-Pair incomplete", 
            INVALID_LOCALE_LINE
        ),
        free
    );

    switch (*line) {
        case 0: {
            // TODO: Check Locale Version
            rt_assert_dyn(
                (STRING_EQUAL(tokens->data[0], "#"DATA_LOCALE_METAPROP_VERSION)),
                isnprintf(
                    INVALID_LOCALE_FORMAT"%s%d"INVALID_LOCALE_LINE_FORMAT, 
                    INVALID_LOCALE, 
                    "\n- Expected #"DATA_LOCALE_METAPROP_VERSION" header.", 
                    INVALID_LOCALE_LINE
                ),
                free
            );
            rt_assert_dyn(
                (is_integer_positive(tokens->data[1])),
                isnprintf(
                    INVALID_LOCALE_FORMAT"%s%d"INVALID_LOCALE_LINE_FORMAT, 
                    INVALID_LOCALE, 
                    "\n- Expected #version header to be a positive number.", 
                    INVALID_LOCALE_LINE
                ),
                free
            );
            
            int version = string_to_based_int(tokens->data[1], 10);
            if (version > DATA_LOCALE_VERSION) {
                // TODO: Debug trace: Unsupported locale version
                return 0;
            } else if (version < DATA_LOCALE_VERSION) {
                // TODO: Debug trace: Outdated locale version. Try to parse either way?
                return 0;
            }

            return 1;
            break;
        }
        case 1: {
            rt_assert_dyn(
                (STRING_EQUAL(tokens->data[0], "#"DATA_LOCALE_METAPROP_LOC)),
                isnprintf(
                    INVALID_LOCALE_FORMAT"%s%d"INVALID_LOCALE_LINE_FORMAT, 
                    INVALID_LOCALE, 
                    "\n- Expected #"DATA_LOCALE_METAPROP_LOC" header.", 
                    INVALID_LOCALE_LINE
                ),
                free
            );
            rt_assert_dyn(
                (!IS_STRING_NULL(tokens->data[1])),
                isnprintf(
                    INVALID_LOCALE_FORMAT"%s%d"INVALID_LOCALE_LINE_FORMAT, 
                    INVALID_LOCALE, 
                    "\n- Expected #"DATA_LOCALE_METAPROP_LOC" header to be non-null.", 
                    INVALID_LOCALE_LINE
                ),
                free
            );

            return 1;
            break;
        }
        case 2: {
            rt_assert_dyn(
                (STRING_EQUAL(tokens->data[0], "#"DATA_LOCALE_METAPROP_LOCNAME)),
                isnprintf(
                    INVALID_LOCALE_FORMAT"%s%d"INVALID_LOCALE_LINE_FORMAT, 
                    INVALID_LOCALE, 
                    "\n- Expected #"DATA_LOCALE_METAPROP_LOCNAME" header.", 
                    INVALID_LOCALE_LINE
                ),
                free
            );
            rt_assert_dyn(
                (!IS_STRING_NULL(tokens->data[1])),
                isnprintf(
                    INVALID_LOCALE_FORMAT"%s%d"INVALID_LOCALE_LINE_FORMAT, 
                    INVALID_LOCALE, 
                    "\n- Expected #"DATA_LOCALE_METAPROP_LOCNAME" header to be non-null.", 
                    INVALID_LOCALE_LINE
                ),
                free
            );

            return 1;
            break;
        }
        default: {
            // Handle normal translations.
            return !g_hash_table_contains(locale->translations, tokens->data[0]);
        }
    }

    return 0; // Fallback, if it reaches here, you fucked up.
    #undef SCOPE
}

void* _ll_parse(Tokens tokens) {
    _LLData data = (_LLData)malloc(sizeof(_LL_DATA));
    memset(data, 0, sizeof(_LL_DATA));

    if (tokens->data[0][0] == '#') {
        data->meta = 1;
        
        if (STRING_EQUAL(tokens->data[0], "#"DATA_LOCALE_METAPROP_VERSION)) data->metaprop = _LL_MP_VERSION;
        else if (STRING_EQUAL(tokens->data[0], "#"DATA_LOCALE_METAPROP_LOC)) data->metaprop = _LL_MP_LOC;
        else if (STRING_EQUAL(tokens->data[0], "#"DATA_LOCALE_METAPROP_LOCNAME)) data->metaprop = _LL_MP_LOCNAME;
    } else {
        data->name = tokens->data[0];
    }

    GString* value = g_string_new(tokens->data[1]);
    g_string_replace(value, "\\n", "\n", 0);

    data->value = strdup(value->str);

    g_string_free(value, TRUE);

    return (void*)data;
}

void _ll_write(void* _data, ParserStore store) {
    _LLData data = (_LLData)_data;

    DataLocale locale = g_array_index(store, DataLocale, 1);
    
    if (data->meta) {
        switch (data->metaprop) {
            case _LL_MP_VERSION: {
                // Ignore, not used.
                break;
            }
            case _LL_MP_LOC: {
                locale->id = strdup(data->value);
                break;
            }
            case _LL_MP_LOCNAME: {
                locale->name = strdup(data->value);
                break;
            }
            default:
                // If you reach this part, you fucked up real bad.
        }
    } else {
        g_hash_table_insert(locale->translations, strdup(data->name), strdup(data->value));
    }

    free(data->value);
    free(_data);
}

void _ll_discard(void* data, ParserStore store) {
    IGNORE_ARG(data);
    IGNORE_ARG(store);
    // Do fuck all
}

void _ll_destruct(FILE* stream, ParserStore store) {
    IGNORE_ARG(stream);

    int* line = g_array_index(store, int*, 0);
    free(line);

    g_array_remove_index(store, 1);
    
    char* path = g_array_index(store, char*, 2);
    free(path);

    g_array_free(store, TRUE);
}

DataLocale _load_locale(char* path) {
    // FILE* file = OPEN_FILE(path, "r");

    DataLocale locale = NULL;
    DataLocale* lp = &locale;

    parse_file(
        path, 
        _ll_tokenize, 
        _ll_preprocess, 
        _ll_verify, 
        _ll_parse, 
        _ll_write, 
        _ll_discard, 
        _ll_destruct,
        &lp,
        path
    );

    // Ensure NIL is defined
    if (!has_localized_string(locale, "nil")) {
        g_hash_table_insert(locale->translations, strdup("nil"), strdup(""));
    }

    return locale;
}

void destroy_locale(DataLocale locale) {
    g_hash_table_destroy(locale->translations);
    free(locale->id);
    free(locale->name);
    free(locale);
}

// ============== LOAD / DESTROY ALL LOCALES ==============
DataLocales load_locales() {
    #define SCOPE "load_locales"
    char* asset_dir = get_asset_dir(NULL);
    char* locale_dir = join_paths(2, asset_dir, DATA_LOCALE_DIR);

    rt_assert(
        (path_exists(locale_dir) == PATH_EXISTS),
        trace_msg(SCOPE, "Locale directory does not exist.")
    );
    rt_assert(
        (path_type(locale_dir) == PATH_TYPE_DIR),
        trace_msg(SCOPE, "Locale directory path is not a path.")
    );

    DataLocales locales = (DataLocales)malloc(sizeof(DATA_LOCALES));
    locales->locales = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);
    locales->len = 0;

    GArray* files = get_files(locale_dir);
    for (ssize_t i = 0; i < files->len; i++) {
        char* file = g_array_index(files, char*, i);

        Tokens fname = tokenize_char_delim(file, strlen(file), ".");
        if (fname->len == 1) continue;

        if (STRING_EQUAL(fname->data[fname->len - 1], "lang")) {
            DataLocale locale = _load_locale(file);
            g_hash_table_insert(locales->locales, strdup(locale->id), locale);
            locales->len++;
        }

        destroy_tokens(fname);
        free(file);
    }

    // I swear to god, one day I will commit a hate crime against the mfs over at GLib.
    g_array_unref(files);
    free(locale_dir);

    return locales;
    #undef SCOPE
}

void _destroy_locale_ghr(gpointer key, gpointer value, gpointer user_data) {
    IGNORE_ARG(key);
    IGNORE_ARG(user_data);

    destroy_locale((DataLocale)value);
}

void destroy_locales(DataLocales locales) {
    g_hash_table_foreach(locales->locales, _destroy_locale_ghr, NULL);
    g_hash_table_destroy(locales->locales);
    free(locales);
}

// ============== LOCALE ACCESSORS ==============
inline int has_locale(DataLocales locales, char* id) {
    return g_hash_table_contains(locales->locales, id);
}

DataLocale get_locale(DataLocales locales, char* id) {
    if (!has_locale(locales, id)) return NULL; // TODO: Fucking crash?

    return g_hash_table_lookup(locales->locales, id);
}

// ============== LOCALIZED STRINGS ==============
inline int has_localized_string(DataLocale locale, char* key) {
    return g_hash_table_contains(locale->translations, key);
}

char* get_localized_string(DataLocale locale, char* key) {
    char* _key = key;

    if (!has_localized_string(locale, _key)) {
        _key = "nil";
    }

    char* str = g_hash_table_lookup(locale->translations, _key);
    return strdup(str);
}

char* get_localized_string_formatted(DataLocale locale, char* key, ...) {
    char* format = get_localized_string(locale, key);

    va_list args;
    va_start(args, key);

    int length = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (length < 0) return NULL;

    va_start(args, key);

    char *result = (char *)malloc(length + 1);
    if (result == NULL) {
        va_end(args);
        return NULL;
    }

    vsnprintf(result, length + 1, format, args);
    va_end(args);

    return result;
}