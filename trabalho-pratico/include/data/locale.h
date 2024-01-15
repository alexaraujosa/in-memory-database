#ifndef TP_DATA_LOCALE_H
#define TP_DATA_LOCALE_H

#include "common.h"
#include "util/error.h"
#include "parser/parser.h"
#include "data/data.h"

typedef struct data_locale DATA_LOCALE, *DataLocale;
typedef struct data_locales DATA_LOCALES, *DataLocales;

/**
 * @brief Loads all localization files within the lang asset directory.
 * 
 * @note A call to #get_asset_dir is recommended before calling this function.
 */
DataLocales load_locales();

/**
 * @brief Destroys the loaded locales.
 * 
 * @param locales The locales to destroy.
 */
void destroy_locales(DataLocales locales);

/**
 * @brief Checks whether a locale is loaded.
 * 
 * @param locales The locale collection to search.
 * @param id The Locale ID.
 */
int has_locale(DataLocales locales, char* id);

/**
 * @brief Returns a reference to a locale loaded within a collection.
 * 
 * @param locales The locale collection to search.
 * @param id The Locale ID.
 */
DataLocale get_locale(DataLocales locales, char* id);

/**
 * @brief Returns a GArray containing all DataLocales within a collection.
 * 
 * @param locales The locale collection to search.
 */
GArray* get_all_locales(DataLocales locales);

/**
 * @brief Returns the id of a locale.
 * 
 * @param locale The locale.
 */
char* get_locale_id(DataLocale locale);

/**
 * @brief Returns the name of a locale.
 * 
 * @param locale The locale.
 */
char* get_locale_name(DataLocale locale);

/**
 * @brief Checks whether a localized string is present in a locale.
 * 
 * @param locale The Locale to search.
 * @param key The localized string ID.
 */
int has_localized_string(DataLocale locale, char* key);

/**
 * @brief Fetches a localized string present in a locale.
 * 
 * @param locale The Locale to search.
 * @param key The localized string ID.
 */
char* get_localized_string(DataLocale locale, char* key);

/**
 * @brief Fetches a localized string present in a locale and formats it. Uses the same syntax as printf.
 * 
 * @param locale The Locale to search.
 * @param key The localized string ID.
 * @param[in] ... The arguments to format the string with.
 */
char* get_localized_string_formatted(DataLocale locale, char* key, ...);

#endif