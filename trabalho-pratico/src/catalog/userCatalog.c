#include "catalog/catalogManager.h"
#include "catalog/userCatalog.h"
#include "collections/user.h"


gint user_tree_compare_func(gconstpointer a, gconstpointer b, gpointer user_data) {
    const USER* user1 = (const USER*)a;
    const USER* user2 = (const USER*)b;

    int name_comparison = strcasecmp(user1->name, user2->name);
    if (name_comparison != 0) {
        return name_comparison;
    }
    return strcasecmp(user1->id, user2->id);
}

void user_print_tree(gpointer data, gpointer user_data) {
    const USER* user = (const USER*)user_data;
    g_print("Value: %s\n", user->name);
}

void write_user(USER* user, Catalog* user_catalog) {
    catalog_add_to_catalog(user_catalog, user->id, user, user);
}