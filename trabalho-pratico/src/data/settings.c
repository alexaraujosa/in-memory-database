#include "data/settings.h"
#include "data/data.h"
#include <time.h>

#define DATA_SETTINGS_VERSION 1

#define DS_CATEGORY_MAX_LEN 20
#define DS_CATEGORY_META "meta"
#define DS_CATEGORY_GENERAL "general"

#define DS_KEY_MAX_LEN 20
#define DS_INT_VALUE_MAX_LEN 12
#define DS_KEY_META_VERSION "version"
#define DS_KEY_GENERAL_LOCALE "locale"

#define DS_WRITE_CATEGORY(cat) fprintf(file, "[%s]\n", cat)
#define DS_WRITE_STR_PROP(key, value) fprintf(file, "%s=%s\n", key, value)
#define DS_WRITE_INT_PROP(key, value) fprintf(file, "%s=%d\n", key, value)
#define DS_WRITE_LONG_PROP(key, value) fprintf(file, "%s=%ld\n", key, value)
#define DS_WRITE_NEWLINE() fprintf(file, "\n");

char* ds_get_locale(DataSettings ds) {
    return strdup(ds->locale);
}

void ds_set_locale(DataSettings ds, char* locale) {
    free(ds->locale);
    ds->locale = strdup(locale);
}

DataSettings make_data_settings(char* locale) {
    DataSettings ds = (DataSettings)malloc(sizeof(DATA_SETTINGS));
    ds->locale = strdup(locale);

    return ds;
}

void destroy_data_settings(DataSettings ds) {
    free(ds->locale);
    free(ds);
}

DataSettings default_settings() {
    DataSettings ds = make_data_settings("en_US");

    return ds;
}

void save_data_settings(DataSettings settings) {
    char* asset_path = get_asset_dir(NULL);
    char* settings_path = join_paths(2, asset_path, "settings.ini");

    time_t curtime = time(NULL);

    FILE* file = OPEN_FILE(settings_path, "w");

    DS_WRITE_CATEGORY(DS_CATEGORY_META);
    DS_WRITE_INT_PROP(DS_KEY_META_VERSION, DATA_SETTINGS_VERSION);
    // DS_WRITE_INT_PROP("last_write", curtime);
    DS_WRITE_LONG_PROP("last_write", curtime);
    DS_WRITE_NEWLINE();

    DS_WRITE_CATEGORY(DS_CATEGORY_GENERAL);
    DS_WRITE_STR_PROP(DS_KEY_GENERAL_LOCALE, settings->locale);

    CLOSE_FILE(file);

    free(settings_path);
}

char* _ds_read_category(char* line, ssize_t len) {
    #define SCOPE "_ds_read_category"
    rt_assert((len > 0), trace_msg(SCOPE, "Error trying to read category header: EOF"));

    char category[DS_CATEGORY_MAX_LEN] = { 0 };
    rt_assert(
        (sscanf(line, "[%[^]]", category) == 1), 
        trace_msg(SCOPE, "Error trying to read category header: Unknown error reading the category header.")
    );

    return strdup(category);
    #undef SCOPE
}

int _ds_read_int_prop(char* line, ssize_t len, char** key) {
    #define SCOPE "_ds_read_int_prop"
    rt_assert((len > 0), trace_msg(SCOPE, "Error trying to read integer property: EOF"));

    // char key[DS_KEY_MAX_LEN] = { 0 };
    char* _key = (char*)malloc(DS_KEY_MAX_LEN);
    memset(_key, 0, DS_KEY_MAX_LEN);

    int kind = 0;

    while (kind != DS_KEY_MAX_LEN && kind != len && line != NULL && *line != '=') {
        _key[kind++] = *line;
        line++;
    }

    *key = _key;

    rt_assert(
        (kind != DS_KEY_MAX_LEN && kind != len && line != NULL), 
        trace_msg(SCOPE, "Error trying to read integer property: Reached EOF before value.")
    );

    line++;

    char _value[DS_INT_VALUE_MAX_LEN] = { 0 };
    int vind = kind;
    int _vind = 0;

    while (vind != DS_KEY_MAX_LEN && vind != len && line != NULL && *line != '\n') {
        _value[_vind++] = *line;
        vind++;
        line++;
    }

    rt_assert(
        (vind != len && line != NULL), 
        trace_msg(SCOPE, "Error trying to read integer property: Reached EOF before valid value.")
    );

    int value = string_to_based_int(_value, 10);

    return value;

    #undef SCOPE
}

// Code repetition because I can't be fucked to create a macro for this. DRY be blasted.
long _ds_read_long_prop(char* line, ssize_t len, char** key) {
    #define SCOPE "_ds_read_long_prop"
    rt_assert((len > 0), trace_msg(SCOPE, "Error trying to read long integer property: EOF"));

    // char key[DS_KEY_MAX_LEN] = { 0 };
    char* _key = (char*)malloc(DS_KEY_MAX_LEN);
    memset(_key, 0, DS_KEY_MAX_LEN);

    int kind = 0;

    while (kind != DS_KEY_MAX_LEN && kind != len && line != NULL && *line != '=') {
        _key[kind++] = *line;
        line++;
    }

    *key = _key;

    rt_assert(
        (kind != DS_KEY_MAX_LEN && kind != len && line != NULL), 
        trace_msg(SCOPE, "Error trying to read long integer property: Reached EOF before value.")
    );

    line++;

    char _value[DS_INT_VALUE_MAX_LEN] = { 0 };
    int vind = kind;
    int _vind = 0;

    while (vind != DS_KEY_MAX_LEN && vind != len && line != NULL && *line != '\n') {
        _value[_vind++] = *line;
        vind++;
        line++;
    }

    rt_assert(
        (vind != len && line != NULL), 
        trace_msg(SCOPE, "Error trying to read long integer property: Reached EOF before valid value.")
    );

    long value = string_to_based_long(_value, 10);

    return value;

    #undef SCOPE
}

struct _ds_str_prop {
    char* key;
    char* value;
};
struct _ds_str_prop _ds_read_str_prop(char* line, ssize_t len) {
    #define SCOPE "_ds_read_int_prop"
    rt_assert((len > 0), trace_msg(SCOPE, "Error trying to read string property: EOF"));

    Tokens parts = tokenize_char_delim(line, len, "=");
    rt_assert((parts->len > 0), trace_msg(SCOPE, "Error trying to read string property: Empty line."));

    struct _ds_str_prop prop = {
        .key = strdup(parts->data[0]),
        .value = NULL
    };

    if (parts->len > 1) {
        char* value = join_strings_with_delim_list("=", parts->len - 1, parts->data + 1);
        prop.value = value;
    }

    destroy_tokens(parts);

    return prop;

    #undef SCOPE
}

DataSettings read_data_settings() {
    #define SCOPE "read_data_settings"
    char* asset_path = get_asset_dir(NULL);
    char* settings_path = join_paths(2, asset_path, "settings.ini");

    DataSettings ds = default_settings();

    if (path_exists(settings_path) != PATH_EXISTS) {
        // DataSettings ds = default_settings();
        save_data_settings(ds);

        return ds;
    }

    FILE* file = OPEN_FILE(settings_path, "r");

    // DataSettings ds = make_data_settings(NULL);

    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    
    int step = 0;
    while ((read = getline(&line, &len, file)) != -1) {
        if (step == 0) {
            char* category = _ds_read_category(line, read);
            rt_assert(
                STRING_EQUAL(category, DS_CATEGORY_META),
                trace_msg(SCOPE, "Expected first category to be '"DS_CATEGORY_META"'.")
            );
            free(category);
        } else if (step == 1) {
            char* key;
            int version = _ds_read_int_prop(line, read, &key);

            rt_assert(
                STRING_EQUAL(key, DS_KEY_META_VERSION),
                trace_msg(SCOPE, "Expected first meta property to be '"DS_KEY_META_VERSION"'.")
            );

            if (version > DATA_SETTINGS_VERSION) {
                // Trace?
            } else if (version < DATA_SETTINGS_VERSION) {

            }
        } else if (step == 2) {
            // Ignore last_read
        } else if (step == 3) {
            // Skip empty line
        } else if (step == 4) {
            char* category = _ds_read_category(line, read);
            rt_assert(
                STRING_EQUAL(category, DS_CATEGORY_GENERAL),
                trace_msg(SCOPE, "Expected first category to be '"DS_CATEGORY_GENERAL"'.")
            );
            free(category);
        } else if (step == 5) {
            volatile struct _ds_str_prop prop = _ds_read_str_prop(line, read);
            rt_assert(
                STRING_EQUAL(prop.key, DS_KEY_GENERAL_LOCALE),
                trace_msg(SCOPE, "Expected property #1 of "DS_CATEGORY_GENERAL" to be '"DS_KEY_GENERAL_LOCALE"'.")
            );

            // if (prop.value != NULL) ds->locale = strdup(prop.value);
            if (prop.value != NULL) ds_set_locale(ds, prop.value);

            free(prop.key);
            free(prop.value);
        } else {
            break;
        }

        step++;
    }

    free(line);

    CLOSE_FILE(file);

    free(settings_path);

    return ds;
    #undef SCOPE
}
