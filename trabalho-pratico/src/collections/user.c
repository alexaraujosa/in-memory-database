#include "collections/user.h"

#include "catalog/catalogManager.h"

// USER makeUser(
//     UserId(id),
//     char name[MAX_NAME_LEN + 1],
//     bool sex,
//     CountryCode(country_code),
//     int account_creation,
//     bool account_status
// ) {
USER makeUser(
    char* id,
    char* name,
    bool sex,
    CountryCode(country_code),
    int account_creation,
    bool account_status) {
    rt_assert(
        strlen(id) == MAX_USER_ID_LEN,
        isnprintf("User Id must be a string with length %d (got %d)", MAX_USER_ID_LEN, strlen(id)));
    rt_assert(
        strlen(name) == MAX_NAME_LEN,
        isnprintf("User Name must be a string with length %d (got %d)", MAX_NAME_LEN, strlen(name)));

    USER user = {
        // .id = id,
        // .name = name,
        .sex = sex,
        // .country_code = country_code,
        .account_creation = account_creation,
        .account_status = account_status};

    strncpy(user.id, id, (size_t)MAX_USER_ID_LEN);
    strncpy(user.name, name, (size_t)MAX_NAME_LEN);
    strncpy(user.country_code, country_code, 2);

    // TODO: Calculate age
    user.age = -1;

    return user;
}

USER parseUserFromLine(char* line, int len) {
    IGNORE_ARG(line);
    IGNORE_ARG(len);
    // TODO: Parse User from CSV line
}

