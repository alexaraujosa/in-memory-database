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
    int ara; // Automatically Reset Arguments
} DATA_SETTINGS, *DataSettings;

// ======= DESTRUCTOR =======
void destroy_data_settings(DataSettings ds);

// ======= ACCESSORS =======
char* ds_get_locale(DataSettings ds);
void ds_set_locale(DataSettings ds, char* locale);

int ds_get_ara(DataSettings ds);
void ds_set_ara(DataSettings ds, int ara);

// ======= OPERATORS =======
void save_data_settings(DataSettings settings);
DataSettings read_data_settings();

#endif