#ifndef PASSENGERS_CATALOG
#define PASSENGERS_CATALOG

#include "common.h"
#include "collections/passenger.h"

gint passenger_tree_compare_func(gconstpointer a, gconstpointer b);
void passenger_print_tree(gpointer data, gpointer passenger_data);
void write_passenger(Passenger passenger, ParserStore store);

#endif