#include "collections/user.h"

#include "catalog/catalogManager.h"


User make_user(
    char* id,
    char* name,
    bool sex,
    CountryCode(country_code),
    int account_creation,
    bool account_status,
    int birth_date
    ) {

    User user = malloc(sizeof(USER));

    strncpy(user->id, id, (size_t)MAX_USER_ID_LEN);
    strncpy(user->name, name, (size_t)MAX_NAME_LEN);
    user->sex = sex;
    strcpy(user->country_code, country_code);   //TODO: Verify if theres any problem using strcpy instead strncpy
    user->account_creation = account_creation;  // TODO: Pensar melhor no offset
    user->account_status = account_status;
    user->age = get_age(birth_date);

    return user;
}

User parse_user(Tokens tokens) {
    char** parameter = tokens->data;
    bool sex = get_sex(parameter[5]);
    bool account_status = get_account_status(parameter[11]);
    int account_creation = date_string_to_int(parameter[9]);
    int birth_date = date_string_to_int(parameter[4]);
    
    User user = make_user(parameter[0], parameter[1], sex, parameter[7], account_creation, account_status, birth_date);
    return user;
}

void discard_user(void* raw_data, ParserStore store) {
    void** discard_file = &g_array_index(store, void*, 0);
    if (*discard_file == NULL) {
        char* parts[2] = {get_cwd()->str, "Resultados/users_errors.csv"};
        char* full_path = join_paths(parts, 2);
        *discard_file = OPEN_FILE(full_path, "w");

        void** file_header = &g_array_index(store, void*, 1);
        rt_assert(
            *file_header != NULL,
            "Could not open discard file: Dataset header missing."
        );

        fputs(*file_header, *discard_file);
        free(full_path);
    }
    Tokens tokens = (Tokens)raw_data;

    discard_to_file(tokens, store);
}

// USER parseUserFromLine(char* line, int len) {
//     IGNORE_ARG(line);
//     IGNORE_ARG(len);
//     // TODO: Parse User from CSV line
// }

void print_user(void* pt) {
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