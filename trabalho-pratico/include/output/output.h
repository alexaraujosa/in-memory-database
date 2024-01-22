/**
 * @file output.h
 * @brief Header file for handling output-related functionalities.
 *
 * Detailed description of the file (if needed).
 */

#ifndef OUTPUT_H
#define OUTPUT_H

#include "parser/parser.h"
#include "queries/queries.h"
#include "common.h"

#define MAX_BUFFER_SIZE 1024

/**
 * @brief Generates and outputs query information to a file.
 *
 * Detailed description of the function.
 * @param query The type of query.
 * @param flag The flag indicating the type of output ('F' for file header, 'D' for data).
 * @param query_info Information related to the query.
 * @param output_file The file to write the output to.
 * @param n_element The element index.
 */
void output_query_info(int query, char flag, void* query_info, FILE* output_file, int n_element);

#endif /* OUTPUT_H */