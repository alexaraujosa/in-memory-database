#include "catalog/catalogManager.h"
#include "collections/user.h"


gint userTreeCompareFunc(gconstpointer a, gconstpointer b, gpointer user_data) {
    const USER* user1 = (const USER*)a;
    const USER* user2 = (const USER*)b;

    if (strcoll(user1->name, user2->name) < 0) return -1;
    if (strcoll(user1->name, user2->name) > 0) return 1;
    if (strcoll(user1->id, user2->id) < 0) return -1;
    if (strcoll(user1->id, user2->id) > 0) return 1;
    return 0;
}

void writeUser(USER* user, Catalog* userCatalog) {
    catalog_add_to_catalog(userCatalog, user->id, user, user);
}

void printUser(void* pt) {
    USER* user = (USER*)pt;
    char sex = 'F';
    char status[10] = "Inactive";

    if (user->sex) sex = 'M';
    if (user->account_status) strcpy(status, "Active");

    printf(
        "{Id:%s; "
        "Nome:%s; "
        "Sex:%c; "
        "CountryCode:%s; "
        "Account_Creation:%d; "
        "Account_status:%s; "
        "Idade:%d}\n",
        user->id, user->name, sex, user->country_code, user->account_creation, status, user->age);
}
