#ifndef FLIGHTS_CATALOG
#define FLIGHTS_CATALOG

#include "common.h"
#include "collections/flight.h"

gint flight_tree_compare_func(gconstpointer a, gconstpointer b);
void flight_print_tree(gpointer data, gpointer flight_data);
void write_flight(FLIGHT* fligh, ParserStore store);
void print_flights_key_value(void* key, void* value);

#endif