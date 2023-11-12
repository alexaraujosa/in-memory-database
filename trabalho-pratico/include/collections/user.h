#ifndef TP_COLLECTIONS_USER_H
#define TP_COLLECTIONS_USER_H

#include <stdbool.h>
#include <stdint.h>

#include "debug.h"
#include "catalog/catalogManager.h"
#include "util/collection.h"
#include "util/error.h"
#include "util/string.h"
#include "parser/parser.h"
#include "util/helpers.h"

#define MAX_USER_ID_LEN 60

// Lenght of the largest portuguese proper noun:
// "Manuel Maria Filipe Carlos Amélio Luís Miguel Rafael Gabriel Gonzaga Xavier "
// "Francisco de Assis Eugénio de Bragança Orleães Sabóia e Saxe-Coburgo-Gotha"
#define MAX_NAME_LEN 156

// #define UserId(_id) char _id[MAX_USER_ID_LEN]
#define UserId(name) TypedArrayVariable(char, name, MAX_USER_ID_LEN + 1)
#define CountryCode(name) TypedArrayVariable(char, name, 2)

typedef struct user USER, *User;

int verify_user_tokens(Tokens tokens);

User make_user(
    // UserId(id),
    // char name[MAX_NAME_LEN]
    char* id,
    char* name,

    bool sex,
    CountryCode(country_code),
    int account_creation,
    bool account_status,
    int birth_date
    );

const char *get_user_id(const User user);
void set_user_id(User user, const char *id);
const char *get_user_name(const User user);
void set_user_name(User user, const char *name);
bool get_user_sex(User user);
void set_user_sex(User user, bool sex);
const char *get_user_country_code(const User user);
void set_user_country_code(User user, const char* country_code);
int get_user_account_creation(const User user);
void set_user_account_creation(User user, int account_creation);
bool get_user_account_status(const User user);
void set_user_account_status(User user, bool account_status);
int get_user_age(const User user);
void set_user_age(User user, int age);

User parse_user(Tokens tokens);
void discard_user(void* raw_data, ParserStore store);
void print_user(void* user);

#endif