#ifndef TP_UTIL_STRING_H
#define TP_UTIL_STRING_H

#include "common.h"
#include "error.h"
#include <strings.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>

#define xstr(s) str(s)
#define str(s) #s

#define IS_NULL(ARG)         (ARG == NULL)
#define IS_STRING_NULL(ARG)  (IS_NULL(ARG) || *ARG == '\0')

char* isnprintf(const char *format, ...);
char* strdup_to(char* dest, char* src);
int is_digit(char c);
int is_digit_positive(char c);
int is_integer_positive_or_zero(const char* number);
int is_integer_positive(const char* number);
int is_integer_between_one_and_five(const char* number);
int have_whitespace(const char* parameter[], int num_parameters);
int is_email(const char* parameter, int length);
int is_boolean(const char* value);
int is_length(const char* string, int length);
int is_date(const char* string);
int is_date_with_time(const char* string);
bool get_sex(char* parameter);
bool get_account_status(char* parameter);
int date_string_to_int(char* parameter);

#endif