#ifndef TP_EXECUTERS_DATASETS_H
#define TP_EXECUTERS_DATASETS_H

#include "catalog/catalogManager.h"

typedef struct dataset_data *DatasetData;

/**
 * @brief Creates an empty Dataset Data intance.
 * 
 * @param dd The Dataset data.
 */
DatasetData make_dataset_data();

/**
 * @brief Sets the directory of the datasets to be used.
 * 
 * @param dd The Dataset data.
 * @param dir The path of the directory to be used.
 */
void dataset_data_set_dir(DatasetData dd, char* dir);

/**
 * @brief Loads the datasets present at the dataset directory.
 * 
 * @param dd The Dataset data.
 */
int dataset_data_load(DatasetData dd);

/**
 * @brief Returns whether the datasets are loaded and the catalogues are present.
 * 
 * @param dd The Dataset data.
 */
int dataset_data_is_loaded(DatasetData dd);

/**
 * @brief Fetches the directory from the Dataset Data.
 * 
 * @param dd The Dataset data.
 */
char* dataset_data_get_dir(DatasetData dd);

/**
 * @brief Fetches the Users Catalog from the Dataset Data.
 * @warning Will return NULL if the datasets were not loaded.
 * 
 * @param dd The Dataset data.
 */
Catalog* dataset_data_get_users_catalog(DatasetData dd);

/**
 * @brief Fetches the Flights Catalog from the Dataset Data.
 * @warning Will return NULL if the datasets were not loaded.
 * 
 * @param dd The Dataset data.
 */
Catalog* dataset_data_get_flights_catalog(DatasetData dd);

/**
 * @brief Fetches the Passengers Catalog from the Dataset Data.
 * @warning Will return NULL if the datasets were not loaded.
 * 
 * @param dd The Dataset data.
 */
Catalog* dataset_data_get_passengers_catalog(DatasetData dd);

/**
 * @brief Fetches the Reservations Catalog from the Dataset Data.
 * @warning Will return NULL if the datasets were not loaded.
 * 
 * @param dd The Dataset data.
 */
Catalog* dataset_data_get_reservations_catalog(DatasetData dd);

/**
 * @brief Fetches the Catalog Array from the Dataset Data.
 * @warning Will return NULL if the datasets were not loaded.
 * @deprecated This function is only included for backwards compatibility with the current query implementation.
 * It should be removed and the other catalog accessors should be used.
 * 
 * @param dd The Dataset data.
 */
Catalog** dataset_data_get_catalog_array(DatasetData dd);

/**
 * @brief Destroys a given Dataset Data.
 * 
 * @param dd The Dataset data.
 */
void destroy_dataset_data(DatasetData dd);

#endif