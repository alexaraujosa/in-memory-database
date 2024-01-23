#ifndef TP_COLLECTIONS_USER_H
#define TP_COLLECTIONS_USER_H

#include <stdbool.h>
#include <stdint.h>

#include "debug.h"
#include "catalog/catalogManager.h"
#include "util/collection.h"
#include "util/error.h"
#include "util/string.h"
#include "util/date.h"
#include "parser/parser.h"

#define MAX_USER_ID_LEN 33

#define MAX_NAME_LEN 33

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
int verify_user_tokens(Tokens tokens, ParserStore store);

char *get_user_id(const User user);
void set_user_id(User user, const char *id);
char *get_user_name(const User user);
void set_user_name(User user, const char *name);
bool get_user_sex(const User user);
void set_user_sex(User user, bool sex);
char* get_user_passport(const User user);
void set_user_passport(User user, const char *passport);
char *get_user_country_code(const User user);
void set_user_country_code(User user, const char* country_code);
int get_user_account_creation(const User user);
void set_user_account_creation(User user, int account_creation);
bool get_user_account_status(const User user);
void set_user_account_status(User user, bool account_status);
uint8_t get_user_age(const User user);
void set_user_age(User user, uint8_t age);
int get_user_information(const User user);
void set_user_information(User user, int information);
void add_user_information(User user, int information);
double get_user_total_spent(const User user);
GArray* get_user_reservations(const User user);
GArray* get_user_flights(const User user);
int get_user_reservations_len(const User user);
int get_user_flights_len(const User user);
void* get_user_flights_data(const User user, int index);
void* get_user_reservations_data(const User user, int index);

/**
 * @brief Creates an user, transforming data.
 * 
 * @param tokens Valid tokens previously verified.
 * 
 * @return Pointer to the User struct.
*/
void* parse_user(Tokens tokens);

/**
 * @brief User pre processor for parsing a file.
 * 
 * @param stream File to be stored.
 * @param store Store that connects the outside of the function with the inside.
 * @param args Variadic arguments.
 * 
 * @return void.
*/
void preprocessor_user(FILE* stream, ParserStore store, va_list args);

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