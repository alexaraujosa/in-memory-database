#include "catalog/usersCatalog.h"

gint usersCatalog_full_compare_func(gconstpointer user_A, gconstpointer user_B) {
    const User user1 = *(const User *)user_A;
    const User user2 = *(const User *)user_B;

    char *name1 = get_user_name(user1);
    char *name2 = get_user_name(user2);

    int name_comparison = strcmp(name1, name2);
    free(name1);
    free(name2);
    if (name_comparison != 0) {
        return name_comparison;
    }

    char *id1 = get_user_id(user1);
    char *id2 = get_user_id(user2);
    int id_comparison = strcmp(id1, id2);  // se der break alterei de strcoll

    free(id1);
    free(id2);
    return id_comparison;
}

gint usersCatalog_strcoll_compare_func(gconstpointer user_A, gconstpointer user_B) {
    const User user1 = *(const User *)user_A;
    const User user2 = *(const User *)user_B;

    char *name1 = get_user_name(user1);
    char *name2 = get_user_name(user2);

    int name_comparison = strcoll(name1, name2);
    free(name1);
    free(name2);
    if (name_comparison != 0) {
        return name_comparison;
    }

    char *id1 = get_user_id(user1);
    char *id2 = get_user_id(user2);
    int id_comparison = strcoll(id1, id2);

    free(id1);
    free(id2);
    return id_comparison;
}

gint usersCatalog_name_compare_func(gconstpointer user_A, gconstpointer user_name) {
    const User *user1 = (const User *)user_A;
    const char *user_NAME2 = (const char *)user_name;
    int length = strlen(user_NAME2);

    char *user_NAME1 = get_user_name(*user1);

    int name_comparison = strncmp(user_NAME1, user_NAME2, length);
    free(user_NAME1);
    return name_comparison;
}

void usersCatalog_write_to_catalog(void *_user, ParserStore store) {
    User user = (User)_user;

    char *id = get_user_id(user);
    Catalog *user_catalog = g_array_index(store, Catalog *, 2);
    catalog_add_str_to_catalog(user_catalog, id, user);
    free(id);
}

void usersCatalog_print_user_key_value(void *key, void *value) {
    char *key_str = (char *)key;
    printf("Key:(%s)\t", key_str);
    print_user(value);
}