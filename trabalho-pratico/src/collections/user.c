#include "collections/user.h"

#include "catalog/catalogManager.h"

typedef struct user {
    UserId(id);
    char name[MAX_NAME_LEN + 1];
    bool sex;
    // char country_code[2];
    CountryCode(country_code);
    int account_creation;  // Offset from Base Date
    bool account_status;

    // Statistics
    uint8_t age;
} USER, *User;


// SEE devemos usar copias com memoria dinamica? ou o chamador usa como input o buffer?
const char *get_user_id(const User user) {
    return strdup(user->id);
}

void set_user_id(User user, const char *id) {
    strncpy(user->id, id, sizeof(user->id) - 1);
    user->id[sizeof(user->id) - 1] = '\0'; 
}

const char *get_user_name(const User user){
    return strdup(user->name);
}

void set_user_name(User user, const char *name){
    strncpy(user->name, name, sizeof(user->name) - 1);
    user->name[sizeof(user->name) - 1] = '\0';
}

bool get_user_sex(User user){
    bool user_sex = user->sex;
    return user_sex;
}

void set_user_sex(User user, bool sex){
    user->sex = sex;
}

const char *get_user_country_code(const User user){
    return strdup(user->country_code);
}

void set_user_country_code(User user, const char* country_code){
    strncpy(user->country_code, country_code, sizeof(user->country_code) - 1);
    user->country_code[sizeof(user->country_code) - 1] = '0';
}

int get_user_account_creation(const User user){
    int account_status = user->account_status;
    return account_status;
}

void set_user_account_creation(User user, int account_creation){
    user->account_creation = account_creation;
}

bool get_user_account_status(const User user){
    bool account_status = user->account_status;
    return account_status;
}

void set_user_account_status(User user, bool account_status){
    user->account_status = account_status;
}

int get_user_age(const User user){
    int age = user->age;
    return age;
}

void set_user_age(User user, int age){
    user->age = age;
}

int verify_user_tokens(Tokens tokens) {
    char** parameter = tokens->data;

    // Whitespace verifier
    if(have_whitespace(parameter, 12) == 0)  return 0;
    
    // Email verifier
    int email_length = strlen(parameter[2]);
    if(!isalpha(parameter[2][email_length - 1]) || !is_email(parameter[2], email_length))  return 0;

    // Country Code verifier
    if(!is_length(parameter[7], 2) || !isalpha(parameter[7][0]) || !isalpha(parameter[7][1]))  return 0;

    // Account Status verifier
    if(strcasecmp(parameter[11], "active") && strcasecmp(parameter[11], "inactive"))  return 0; 

    // Date with(out) time verifier (Syntax)
    if(!is_date(parameter[4]) || !is_date_with_time(parameter[9]))  return 0;

    // Date verifier (Semantic)
    // TODO: Testar a performance entre o strcmp e a comparacao de inteiros
    if(strcmp(parameter[4], parameter[9]) >= 0)  return 0;

    return 1;
}

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