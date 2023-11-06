#ifndef TP_UTIL_STRING_H
#define TP_UTIL_STRING_H

#include "common.h"
#include <strings.h>
#include <stdarg.h>
#include <ctype.h>

#define xstr(s) str(s)
#define str(s) #s

#define IS_NULL(ARG)         (ARG == NULL)
#define IS_STRING_NULL(ARG)  (IS_NULL(ARG) || *ARG == '\0')

char* isnprintf(const char *format, ...);
char* strdup_to(char* dest, char* src);
int is_digit(char c);
int is_digit_positive(char c);
int is_integer_positive_or_zero(char* number);
int is_integer_positive(char* number);
int is_integer_between_one_and_five(char* number);
int have_whitespace(char* parameter[], int num_parameters);
int is_email(char* parameter, int length);
int is_boolean(char* value);
int is_length(char* string, int length);
int is_date(char *string);
int is_date_with_time(char *string);

#endif