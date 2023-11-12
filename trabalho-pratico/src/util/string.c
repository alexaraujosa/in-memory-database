#include "util/string.h"

char* isnprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    // Determine the length of the formatted string
    int length = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (length < 0) return NULL;

    va_start(args, format);

    char *result = (char *)malloc(length + 1);
    if (result == NULL) {
        va_end(args);
        return NULL;
    }

    // Format the string into the allocated memory
    vsnprintf(result, length + 1, format, args);
    va_end(args);

    return result;
}

char* strdup_to(char* dest, char* src) {
    char* str;
    char* p;
    int len = 0;

    while (src[len]) len++;
    str = malloc(len + 1);
    p = str;

    while (*src) *p++ = *src++;
    *p = '\0';

    *dest = *str;

    return str;
}

int is_digit(char c) {
    return (c >= '0' && c <= '9');
}

int is_digit_positive(char c) {
    return (c > '0' && c <= '9');
}

int is_integer_positive_or_zero(char* number) {
    for(int i = 0 ; number[i] ; i++) {
        if(is_digit(number[i]) == 0)  return 0;
    }

    return 1;
}

int is_integer_positive(char* number) {

    if(!is_digit_positive(number[0])) {
        return 0;
    } else {
        for(int i = 1; number[i] ; i++) {
            if(!is_digit(number[i]))  return 0;
        }
    }

    return 1;
}

int is_integer_between_one_and_five(char* number) {
    return (number[0] >= '1' && number[0] <= '5' && number[1] == '\0');
}

int have_whitespace(char* parameter[], int num_parameters) {
    for(int i = 0 ; i < num_parameters ; i++)
        if(IS_STRING_NULL(parameter[i]))  return 0;
    
    return 1;
}

int is_email(char* parameter, int length) {
    int i = length - 2;
    int tld = 1, domain = 0, username = 0;

    while(i >= 0 && parameter[i] != '.' && isalpha(parameter[i])) {
        i--;
        tld++;
    }

    if(i > 0 && parameter[i] == '.' && tld >= 2) {
        i--;
        while(i >= 0 && parameter[i] != '@' && parameter[i] != '.') {
            i--;
            domain++;
        }

        if(i > 0 && parameter[i] == '@' && domain >= 1) {
            i--;
            while(i >= 0 && parameter[i] != '@' && parameter[i] != '.') {
                i--;
                username++;
            }

            if(parameter[0] == '@' || parameter[0] == '.') {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 0;
    }

    return 1;
}

int is_boolean(char* value) {
    int length = strlen(value);

    if(length == 1) {
        tolower(value[0]);
        return (value[0] == 'f' || value[0] == 't' || value[0] == '0' || value[0] == '1');
    } else {
        return (!strcasecmp(value, "true") || !strcasecmp(value, "false"));
    }
    
    return 0;
}

int is_length(char* string, int length) {
    int i;
    for(i = 0 ; i < length ; i++) {
        if(string[i] == '\0')
            return 0;
    }

    return (string[i] == '\0');
}

int is_date(char* string) {
    if(
        !is_digit_positive(string[0])
        || string[4] != '/'
        || string[7] != '/'
    ) return 0;
    if( // Year verifier
        !isdigit(string[1])
        || !isdigit(string[2])
        || !isdigit(string[3])
    ) return 0;
    if( // Month verifier
        (string[5] >= '2') ||
        (string[5] == '0' && !is_digit_positive(string[6])) ||
        (string[5] == '1' && string[6] >= '3')
    ) return 0;
    if( // Day verifier
        (string[8] >= '4') ||
        (string[8] == '0' && !is_digit_positive(string[9])) ||
        (string[8] == '1' && !isdigit(string[9])) ||
        (string[8] == '2' && !isdigit(string[9])) ||
        (string[8] == '3' && string[9] >= '2')
    ) return 0;
    

    return 1;
}

int is_date_with_time(char* string) {
    if(!is_length(string, 19)) return 0;
    if(!is_date(string)) return 0;

    if(string[10] != ' ' || string[13] != ':' || string[16] != ':') return 0;
    if( // Hour verifier
        (string[11] >= '3') ||
        (string[11] == '0' && !is_digit(string[12])) ||
        (string[11] == '1' && !is_digit(string[12])) ||
        (string[11] == '2' && string[12] >= '5')
    ) return 0;
    if( // Minutes verifier
        (string[14] >= '6') ||
        (!is_digit(string[15]))
    ) return 0;
    if( // Seconds verifier
        (string[17] >= '6') ||
        (!is_digit(string[18]))
    ) return 0;

    return 1;
}

bool get_sex(char* parameter) {
    if(parameter[0] == 'M')  return TRUE;

    return FALSE;
}

bool get_account_status(char* parameter) {
    if(parameter[0] == 'a' || parameter[0] == 'A')  return TRUE;

    return FALSE;
}

int date_string_to_int(char* parameter) {

    int year, month, day, date;
    date = year = month = day = 0 ;

    if(sscanf(parameter, "%d/%d/%d", &year, &month, &day) != 3) {
        printf("ERROR Parsing date string to int!\n");
        exit(EXIT_FAILURE);
    }

    date = year * 10000 + month * 100 + day;

    return date;
}

int date_with_time_string_to_int(char* parameter) { // TODO: Performance and refactor this code

    int year, month, day, hour, minute, second, date;
    date = year = month = day = 0 ;

    if(sscanf(parameter, "%d/%d/%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) != 5) {
        printf("ERROR Parsing date string to int!\n");
        exit(EXIT_FAILURE);
    }

    date = year * 100000000 + month * 1000000 + day + 10000 * hour + 100 * minute + second;

    return date;
}

bool get_boolean(char* parameter) {

    if(parameter[0] == '\0' || parameter[0] == 'f' || parameter[0] == 'F' || parameter[0] == '0')  return FALSE;

    return TRUE;
}