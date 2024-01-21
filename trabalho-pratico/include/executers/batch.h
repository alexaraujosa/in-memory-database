#ifndef TP_EXECUTERS_BATCH_H
#define TP_EXECUTERS_BATCH_H

#include "queries/queries.h"
#include "stats/stats.h"
#include <locale.h>
#include "catalog/usersCatalog.h"
#include "catalog/genericCatalog.h"
#include "catalog/reservationsCatalog.h"
#include "catalog/flightsCatalog.h"
#include "catalog/passengersCatalog.h"
#include "catalog/catalogManager.h"

/**
 * @brief Program execution in batch mode.
 * 
 * @param arg1 Path to the csv files.
 * @param arg2 Path to the file that contain the commands to execute queries.
 * 
 * @return void.
*/
void batch(const char* arg1, const char* arg2);

#endif