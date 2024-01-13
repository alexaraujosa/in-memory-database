#ifndef TP_DATA_LOCALE_H
#define TP_DATA_LOCALE_H

#include "common.h"
#include "util/error.h"
#include "parser/parser.h"
#include "data/data.h"

#define DATA_LOCALE_DIR "lang"
#define DATA_LOCALE_FILE_EXT "lang"
#define DATA_LOCALE_VERSION 1

#define DATA_LOCALE_METAPROP_VERSION "version"
#define DATA_LOCALE_METAPROP_LOC     "loc"
#define DATA_LOCALE_METAPROP_LOCNAME "locname"

typedef struct data_locale DATA_LOCALE, *DataLocale;
typedef struct data_locales DATA_LOCALES, *DataLocales;

/**
 * @brief Loads all localization files within the lang asset directory.
 * 
 * @note A call to #get_asset_dir is recommended before calling this function.
 */
DataLocales load_locales();
void destroy_locales(DataLocales locales);

int has_locale(DataLocales locales, char* id);
DataLocale get_locale(DataLocales locales, char* id);

int has_localized_string(DataLocale locale, char* key);
char* get_localized_string(DataLocale locale, char* key);
char* get_localized_string_formatted(DataLocale locale, char* key, ...);

#endif