#include "collections/user.h"

typedef struct user {
    UserId(id);
    char name[MAX_NAME_LEN + 1];
    bool sex;
    char passport[MAX_USER_PASSPORT_LEN + 1];
    // char country_code[2];
    CountryCode(country_code);
    int account_creation;  // Offset from Base Date
    bool account_status;
    unsigned int information;   // 100000 offset for number of reservations |||| 1000 offset for total spent
    GArray* reservations;
    GArray* flights;

    // Statistics
    uint8_t age;
} USER, *User;


char *get_user_id(const User user) {
    return strdup(user->id);
}

void set_user_id(User user, const char *id) {
    strncpy(user->id, id, sizeof(user->id) - 1);
    user->id[sizeof(user->id) - 1] = '\0'; 
}

char *get_user_name(const User user){
    return strdup(user->name);
}

void set_user_name(User user, const char *name){
    strncpy(user->name, name, sizeof(user->name) - 1);
    user->name[sizeof(user->name) - 1] = '\0';
}

bool get_user_sex(const User user){
    bool user_sex = user->sex;
    return user_sex;
}

void set_user_sex(User user, bool sex){
    user->sex = sex;
}

char* get_user_passport(const User user) {
    return strdup(user->passport);
}

void set_user_passport(User user, const char *passport) {
    strncpy(user->passport, passport, sizeof(user->passport) - 1);
    user->passport[sizeof(user->passport) - 1] = '\0';
}

char *get_user_country_code(const User user){
    return strdup(user->country_code);
}

void set_user_country_code(User user, const char* country_code){
    strncpy(user->country_code, country_code, sizeof(user->country_code) - 1);
    user->country_code[sizeof(user->country_code) - 1] = '0';
}

int get_user_account_creation(const User user){
    int account_creation = user->account_creation;
    return account_creation;
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

uint8_t get_user_age(const User user){
    uint8_t age = user->age;
    return age;
}

void set_user_age(User user, uint8_t age){
    user->age = age;
}

int get_user_information(const User user) {
    return user->information;
}

void set_user_information(User user, int information) {
    user->information = information;
}

void add_user_information(User user, int information) {
    user->information += information;
}

double get_user_total_spent(const User user) {
    return (double)(user->information%100000000)/1000;    // TODO: FALTA OS SHIFTS
}

int get_user_reservations_counter(const User user) {
    return (user->information/100000000);
}

GArray* get_user_reservations(const User user) {
    return user->reservations;
}

GArray* get_user_flights(const User user) {
    return user->flights;
}

int get_user_reservations_len(const User user) {
    GArray* reservations = user->reservations;

    return reservations->len;
}

int get_user_flights_len(const User user) {
    GArray* flights = user->flights;

    return flights->len;
}

void* get_user_flights_data(const User user, int index) {
    GArray* flights = user->flights;

    return g_array_index(flights, void*, index);
}

void* get_user_reservations_data(const User user, int index) {
    GArray* reservations = user->reservations;

    return g_array_index(reservations, void*, index);
}

int verify_user_tokens(Tokens tokens, ParserStore store) {
    IGNORE_ARG(store);
    
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
    if(strcmp(parameter[4], parameter[9]) >= 0)  return 0;

    return 1;
}

User make_user(
    char* id,
    char* name,
    bool sex,
    char* passport,
    CountryCode(country_code),
    int account_creation,
    bool account_status,
    int birth_date
    ) {

    User user = malloc(sizeof(USER));

    strncpy(user->id, id, (size_t)MAX_USER_ID_LEN);
    strncpy(user->name, name, (size_t)MAX_NAME_LEN);
    user->sex = sex;
    strncpy(user->passport, passport, (size_t)MAX_USER_PASSPORT_LEN);
    strncpy(user->country_code, country_code, 2);
    user->account_creation = account_creation;
    user->account_status = account_status;
    user->age = get_age(birth_date);
    user->information = 0;
    user->reservations = g_array_new(FALSE, FALSE, sizeof(void*));
    user->flights = g_array_new(FALSE, FALSE, sizeof(void*));

    return user;
}

void* parse_user(Tokens tokens) {
    char** parameter = tokens->data;
    bool sex = get_sex(parameter[5]);
    bool account_status = get_account_status(parameter[11]);
    int account_creation = date_string_notime_to_int(parameter[9]);
    int birth_date = date_string_notime_to_int(parameter[4]);
    
    User user = make_user(parameter[0], parameter[1], sex, parameter[6], parameter[7], account_creation, account_status, birth_date);
    return user;
}

void preprocessor_user(FILE* stream, ParserStore store, va_list args) {
    gpointer null_element = NULL;
    g_array_append_vals(store, &null_element, 1);   // Discard file
    default_csv_preprocessor(stream, store, args);  // File header
    // cvs_preprocessor_helper(stream, store);

    Catalog* catalogo = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo, 1);

    GArray* generic_catalog = va_arg(args, GArray*);
    g_array_append_vals(store, &generic_catalog, 1);
}

void discard_user(void* raw_data, ParserStore store) {
    void** discard_file = &g_array_index(store, void*, 0);
    if (*discard_file == NULL) {
        // char* parts[2] = {get_cwd()->str, "Resultados/users_errors.csv"};
        // char* full_path = join_paths(parts, 2);
        char* full_path = join_paths(2, get_cwd()->str, "Resultados/users_errors.csv");
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
        "CountryCode:%.2s; "
        "Account_Creation:%d; "
        "Account_status:%s; "
        "Idade:%d; "
        "Information:%d}\n",
        user->id, user->name, sex, user->country_code, user->account_creation, status, user->age, user->information);
}