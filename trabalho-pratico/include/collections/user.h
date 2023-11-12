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

User parse_user(Tokens tokens);
void discard_user(void* raw_data, ParserStore store);
void print_user(void* user);

#endif