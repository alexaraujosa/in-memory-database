#ifndef USER_CATALOG
#define USER_CATALOG

#include "common.h"
#include "collections/user.h"

gint user_tree_compare_func(gconstpointer a, gconstpointer b, gpointer user_data);
void user_print_tree(gpointer data, gpointer user_data);
void write_user(USER* user, Catalog* userCatalog);

#endif