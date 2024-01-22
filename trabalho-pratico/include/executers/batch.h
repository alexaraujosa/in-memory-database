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

// /**
//  * @brief Default pre processor for parsing a file.
//  * 
//  * @param stream File to be stored.
//  * @param store Store that connects the outside of the function with the inside.
//  * @param args Variadic arguments.
//  * 
//  * @return void.
// */
// void default_preprocessor(FILE* stream, ParserStore store, va_list args);

// /**
//  * @brief Default pre processor for the reservation catalog.
//  * 
//  * @param stream File to be stored.
//  * @param store Store that connects the outside of the function with the inside.
//  * @param args Variadic arguments.
//  * 
//  * @return void.
// */
// void preprocessor_reservation(FILE* stream, ParserStore store, va_list args);

// /**
//  * @brief Default pre processor for the passenger catalog.
//  * 
//  * @param stream File to be stored.
//  * @param store Store that connects the outside of the function with the inside.
//  * @param args Variadic arguments.
//  * 
//  * @return void.
// */
// void preprocessor_passenger(FILE* stream, ParserStore store, va_list args);

// /**
//  * @brief Default csv destructor for the reservation catalog.
//  * 
//  * @param stream File to be closed.
//  * @param store Store that contains things to be destructed.
//  * 
//  * @return void.
// */
// void default_csv_destructor_reservation(FILE* stream, ParserStore store);

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