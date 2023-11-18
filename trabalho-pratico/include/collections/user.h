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

#define MAX_USER_PASSPORT_LEN 8

// #define UserId(_id) char _id[MAX_USER_ID_LEN]
#define UserId(name) TypedArrayVariable(char, name, MAX_USER_ID_LEN + 1)
#define CountryCode(name) TypedArrayVariable(char, name, 2)

typedef struct user USER, *User;

/**
 * @brief Verifies if tokens are valid to create an user.
 * 
 * @param tokens A pointer to the struct used for verifying the data.
 * 
 * @return 1 if all the tokens are valid. Otherwise, 0.
*/
int verify_user_tokens(Tokens tokens);

/**
 * @brief Creates an user using dynamic memory.
 * 
 * @param id User id.
 * @param name User name.
 * @param sex User sex.
 * @param passport User passport.
 * @param country_code User country code
 * @param account_creation Offset from system start date, relative to the user account creation.
 * @param account_status User account status.
 * @param birth_date Offset from system start date, relative to the user birth date.
 * 
 * @return Pointer to the User struct.
*/
User make_user(
    char* id,
    char* name,
    bool sex,
    char* passport,
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
const char* get_user_passport(const User user);
const char *get_user_country_code(const User user);
void set_user_country_code(User user, const char* country_code);
int get_user_account_creation(const User user);
void set_user_account_creation(User user, int account_creation);
bool get_user_account_status(const User user);
void set_user_account_status(User user, bool account_status);
int get_user_age(const User user);
void set_user_age(User user, int age);

/**
 * @brief Creates an user, transforming data.
 * 
 * @param tokens Valid tokens previously verified.
 * 
 * @return Pointer to the User struct.
*/
User parse_user(Tokens tokens);

/**
 * @brief Creates the users_errors file and writes the lines with invalid data.
 * 
 * @param raw_data Invalid data contained in the line.
 * @param store Connection with the outside part of the parser, used to store the discard file address.
 * 
 * @return void.
*/
void discard_user(void* raw_data, ParserStore store);

/**
 * @brief Prints an user (Mainly usage for debug).
 * 
 * @param user Pointer to the user struct.
 * 
 * @return void.
*/
void print_user(void* user);

#endif