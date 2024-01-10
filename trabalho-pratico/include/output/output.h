#include "parser/parser.h"
#include "queries/queries.h"
#include "common.h"

#define MAX_BUFFER_SIZE 1024

void output_query_info(int query, char flag, void* query_info, FILE* output_file, int n_element);
char* build_query_buffer(int query, void* query_info, char *output_buffer);