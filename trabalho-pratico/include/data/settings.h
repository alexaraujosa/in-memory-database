#ifndef TP_DATA_SETTINGS_H
#define TP_DATA_SETTINGS_H

#include "common.h"
#include "string.h"
#include "util/string.h"

// struct _ds_str_prop {
//     char* key;
//     char* value;
// };

// struct _ds_str_prop _ds_read_str_prop(char* line, ssize_t len);

typedef struct data_settings {
    char* locale;
} DATA_SETTINGS, *DataSettings;

// ======= ACCESSORS =======
char* ds_get_locale(DataSettings ds);

// ======= OPERATORS =======
void save_data_settings(DataSettings settings);
DataSettings read_data_settings();

#endif