#include "catalog/userCatalog.h"

#include "catalog/catalogManager.h"
#include "collections/user.h"

gint user_tree_compare_func(gconstpointer a, gconstpointer b, gpointer user_data) {
    const USER *user1 = (const USER*)a;
    const USER *user2 = (const USER*)b;

    const char *name1 = get_user_name(user1);
    const char *name2 = get_user_name(user2);

    int name_comparison = strcasecmp(name1, name2);
    if (name_comparison != 0) {
        free(name1);
        free(name2);
        return name_comparison;
    }

    const char *id1 = get_user_name(id1);
    const char *id2 = get_user_name(id2);
    int id_comparison = strcasecmp(id1, id2);

    free(name1);
    free(name2);
    free(id1);
    free(id2);
    return id_comparison;
}

void user_print_tree(gpointer data, gpointer user_data) {
    const USER *user = (const USER*)user_data;
    const char *name = get_user_name(user);
    g_print("Value: %s\n", name);
    free(name);
}

void write_user(USER *user, Catalog *user_catalog) {
    const char *id = get_user_id(user);
    catalog_add_to_catalog(user_catalog, id, user, user);
    free(id);
}