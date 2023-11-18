#include "catalog/usersCatalog.h"

#include "catalog/catalogManager.h"
#include "collections/user.h"
#include "parser/parser.h"


gint users_full_compare_func(gconstpointer a, gconstpointer b) {
    const User user1 = *(const User *)a;
    const User user2 = *(const User *)b;

    char *name1 = get_user_name(user1);
    char *name2 = get_user_name(user2);

    int name_comparison = strcoll(name1, name2);
    if (name_comparison != 0) {
        free(name1);
        free(name2);
        return name_comparison;
    }

    char *id1 = get_user_id(user1);
    char *id2 = get_user_id(user2);
    int id_comparison = strcoll(id1, id2);

    free(name1);
    free(name2);
    free(id1);
    free(id2);
    return id_comparison;
}

gint user_username_compare_func(gconstpointer a, gconstpointer b) {
    const User *user1 = (const User*)a;
    const char* user_NAME2 = (const char*) b;
    char* user_NAME1 = get_user_name(*user1);   // TODO: falta dar free
    int length = strlen(user_NAME2);
    
    int name_comparison = strncasecmp(user_NAME1, user_NAME2, length);
    if (name_comparison != 0) {
        free(user_NAME1);
        return name_comparison;
    }
}

gint usersCatalog_full_compare_func(gconstpointer a, gconstpointer b) {
    const User user1 = *(const User *)a;
    const User user2 = *(const User *)b;

    char *name1 = get_user_name(user1);
    char *name2 = get_user_name(user2);

    int name_comparison = strcasecmp(name1, name2);
    if (name_comparison != 0) {
        free(name1);
        free(name2);
        return name_comparison;
    }

    char *id1 = get_user_id(user1);
    char *id2 = get_user_id(user2);
    int id_comparison = strcasecmp(id1, id2);

    free(name1);
    free(name2);
    free(id1);
    free(id2);
    return id_comparison;
}

void usersCatalog_print_array(gpointer data, gpointer user_data) {
    const User user = (const User)user_data;
    char *name = get_user_name(user);
    g_print("Value: %s\n", name);
    free(name);
}

void usersCatalog_write_to_catalog(User user, ParserStore store) {
    const char *id = get_user_id(user);
    Catalog *user_catalog = g_array_index(store, Catalog *, 2);
    catalog_add_str_to_catalog(user_catalog, id, user);
    free(id);
}

void usersCatalog_print_user_key_value(void *key, void *value) {
    char *key_str = (char *)key;
    printf("Key:(%s)\t", key_str);
    print_user(value);
}