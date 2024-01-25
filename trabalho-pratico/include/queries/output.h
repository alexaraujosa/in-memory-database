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

/**
 * @brief Generates and outputs query information to a file.
 *
 * @param query The type of query.
 * @param flag The flag indicating the type of output ('F' for file header, 'D' for data).
 * @param query_info Information related to the query.
 * @param output_file The file to write the output to.
 * @param n_element The element index.
 */
void output_query_to_file(int query, char flag, void* query_info, FILE* output_file, int n_element);
// void output_query_info(int query, char flag, void* query_info, FILE* output_file, int n_element);

/**
 * @brief Generates and outputs query information to a string, and returns it.
 *
 * @param query The type of query.
 * @param flag The flag indicating the type of output ('F' for file header, 'D' for data).
 * @param query_info Information related to the query.
 * @param output_file The file to write the output to.
 * @param n_element The element index.
 * 
 * @returns A string containing the formatted query.
 */
char* output_query_to_str(int query, char flag, void* query_info, FILE* output_file, int n_element);

#endif /* OUTPUT_H */