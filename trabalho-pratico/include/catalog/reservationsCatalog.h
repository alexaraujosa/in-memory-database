#ifndef RESERVATION_CATALOG
#define RESERVATION_CATALOG

#include "common.h"
#include "collections/reservation.h"

gint reservation_tree_compare_func(gconstpointer a, gconstpointer b);
void reservation_print_tree(gpointer data, gpointer reservation_data);
void write_reservation(RESERVATION* reservation, ParserStore store);
void print_reservation_key_value(void* key, void* value);

#endif