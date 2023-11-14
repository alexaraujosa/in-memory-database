#include "catalog/userCatalog.h"

#include "catalog/catalogManager.h"
#include "collections/user.h"

#include "parser/parser.h"

gint user_tree_compare_func(gconstpointer a, gconstpointer b) {
    const User user1 = (const User)a;
    const User user2 = (const User)b;

    const char *name1 = get_user_name(user1);
    const char *name2 = get_user_name(user2);

    int name_comparison = strcasecmp(name1, name2);
    if (name_comparison != 0) {
        free(name1);
        free(name2);
        return name_comparison;
    }

    const char *id1 = get_user_id(user1);
    const char *id2 = get_user_id(user2);
    int id_comparison = strcasecmp(id1, id2);

    free(name1);
    free(name2);
    free(id1);
    free(id2);
    return id_comparison;
}

void user_print_tree(gpointer data, gpointer user_data) {
    const User user = (const User)user_data;
    const char *name = get_user_name(user);
    g_print("Value: %s\n", name);
    free(name);
}

void write_user(User user, ParserStore store) {
    const char *id = get_user_id(user);
    Catalog* user_catalog = g_array_index(store, Catalog*, 2);
    catalog_add_str_to_catalog(user_catalog, id, user, user);
    free(id);
}

void print_user_key_value(void* key, void* value) {
    char *key_str = (char *)key;
    printf("Key:(%s)\t", key_str);
    print_user(value);
}