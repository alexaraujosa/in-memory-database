#include "util/string.h"
#include "stdint.h"

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

// wchar_t* char_str_to_wchar(char** str) {
//     #define SCOPE "char_str_to_wchar"
//     size_t size = mbstowcs(NULL, str, 0);
//     if (size == -1) {
//         perror(trace_msg(SCOPE, "Could not determine size of wchar string."));
//         exit(EXIT_FAILURE);
//     }

//     wchar_t* wstr = (wchar_t*)malloc((size + 1) * sizeof(wchar_t));
//     if (wstr == NULL) {
//         perror(trace_msg(SCOPE, "Could not allocate wchar string."));
//         exit(EXIT_FAILURE);
//     }

//     if (mbstowcs(wstr, str, size + 1) == (size_t)-1) {
//         perror(trace_msg(SCOPE, "Could not convert char string to wchar string."));
//         free(wstr);
//         exit(EXIT_FAILURE);
//     }

//     wstr[size] = L'\0';

//     return wstr;
//     #undef SCOPE
// }

// TODO: Uniformize tokenizers into one single function?
// Tokens get_lines(char* line, size_t len) {
//     char* ptr = strdup(line);
//     char* ptr_root = ptr;

//     if (ptr == NULL) exit(EXIT_FAILURE);

//     if (ptr[len - 1] == '\n') {
//         ptr[len - 1] = '\0';
//     }

//     int seps = 1;
//     for (int i = 0; line[i]; i++) seps += (line[i] == '\n');

//     // Early return, avoid unnecessary work.
//     if (seps == 1) {
//         char** arr = (char**)malloc(1 * sizeof(char*));
//         arr[0] = strdup(line);

//         free(ptr_root);

//         Tokens ret = (Tokens)malloc(sizeof(TOKENS));
//         ret->data = arr;
//         ret->len = 1;

//         return ret;
//     }

//     char** arr = (char**)malloc(seps * sizeof(char*));
//     memset(arr, 0, seps * sizeof(char*));

//     char* token;
//     int i = 0;
//     while ((token = strsep(&ptr, "\n")) != NULL) {
//         char* tokenData = strdup(token);

//         arr[i++] = tokenData;
//     }

//     Tokens ret = (Tokens)malloc(sizeof(TOKENS));
//     ret->data = arr;
//     ret->len = seps;

//     free(ptr_root);
//     return ret;
// }
Tokens get_lines(char* line, size_t len) {
    return tokenize_char_delim(line, len, "\n");
}

Tokens break_lines(Tokens lines, int max_len) {
    int line_count = lines->len;
    for (int i = 0; i < lines->len; i++) line_count += (strlen(lines->data[i]) / max_len);

    char** ndata = (char**)malloc(line_count * sizeof(char*));
    int nind = 0;

    for (int i = 0; i < lines->len; i++) {
        int line_len = strlen(lines->data[i]);

        if (line_len <= max_len) {
            ndata[nind++] = strdup(lines->data[i]);
        } else {
            for (int j = 0; j <= line_len / max_len; j++) {
                ndata[nind++] = strndup(lines->data[i] + (max_len * j), max_len);
            }
        }
    }

    destroy_tokens(lines);

    Tokens nlines = (Tokens)malloc(sizeof(TOKENS));
    nlines->data = ndata;
    nlines->len = line_count;

    return nlines;
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
        return (value[0] == 'f' || value[0] == 'F' || value[0] == 't' || value[0] == 'T' || value[0] == '0' || value[0] == '1');
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

bool get_boolean(char* parameter) {

    if(parameter[0] == '\0' || parameter[0] == 'f' || parameter[0] == 'F' || parameter[0] == '0')  return FALSE;

    return TRUE;
}

char* to_upper_string(char* parameter) {
    for(int i = 0 ; parameter[i] ; i++)
        parameter[i] = toupper(parameter[i]);

    return parameter;
}

int string_to_based_int(char* input, int base) {
    if (input[0] == '\0' || isspace(input[0])) return INT_MAX;
    errno = 0;

    char *end;
    long l = strtol(input, &end, base);

    /* Both checks are needed because INT_MAX == LONG_MAX is possible. */
    if (l > INT_MAX || (errno == ERANGE && l == LONG_MAX)) return INT_MAX;
    if (l < INT_MIN || (errno == ERANGE && l == LONG_MIN)) return INT_MAX;
    if (*end != '\0') return INT_MAX;
    
    return l;
}

long string_to_based_long(char* input, int base) {
    if (input[0] == '\0' || isspace(input[0])) return LONG_MAX;
    errno = 0;

    char *end;
    long l = strtol(input, &end, base);

    if ((errno == ERANGE && l == LONG_MAX)) return LONG_MAX;
    if ((errno == ERANGE && l == LONG_MIN)) return LONG_MAX;
    if (*end != '\0') return LONG_MAX;
    
    return l;
}

MAX_COLS_AND_ROWS get_max_rows_and_cols(char* line, ssize_t len) {
    Tokens lines = get_lines(line, len);

    int max_col = 0;
    for (int i = 0; i < lines->len; i++) {
        int len = strlen(lines->data[i]);
        if (max_col < len) max_col = len; 
    }

    MAX_COLS_AND_ROWS mcar = (MAX_COLS_AND_ROWS){
        .cols = max_col,
        .rows = lines->len
    };

    destroy_tokens(lines);

    return mcar;
}

char* join_strings_with_delim(char* delim, int len, ...) {
    va_list _args;
    va_start(_args, len);

    char** args = (char**)malloc(len * sizeof(char*));
    for (int i = 0; i < len; i++) args[i] = va_arg(_args, char*);

    va_end(_args);

    return join_strings_with_delim_list(delim, len, args);
}

char* join_strings_with_delim_list(char* delim, int len, char** args) {
    int delim_len = strlen(delim);

    char* parts[len];
    int totalLen = len * delim_len;
    for (int i = 0; i < len; i++) {
        char* part = args[i];
        parts[i] = part;
        totalLen += strlen(part);
    }

    // Some fuckery to shut GCC (https://gcc.gnu.org/bugzilla//show_bug.cgi?id=85783)
    size_t totalLenBits = totalLen * sizeof(char); 
    if (totalLenBits >= PTRDIFF_MAX) return NULL;

    char* fullStr = (char*)malloc(totalLenBits);
    int offset = 0;
    for (int i = 0; i < len; i++) {
        int partLen = strlen(parts[i]);

        memcpy(fullStr + offset, parts[i], partLen);
        offset += partLen + 1;
        if (i < len - 1) memcpy(fullStr + offset - 1, delim, delim_len);
    }

    if (fullStr[totalLen - 1] != '\0') fullStr[totalLen - 1] = '\0';

    return fullStr;
}

void add_char_to_str_at(char* source, int index, char insert) {
    #define SCOPE "add_str_to_str_at"
    int source_len = strlen(source);

    rt_assert(
        (index >= 0 && index <= source_len),
        trace_msg(SCOPE, "Index OOB.")
    );

    // Shift characters to make space for the new character
    for (int i = source_len; i > index; i--) {
        source[i] = source[i - 1];
    }

    // Insert the new character
    source[index] = insert;
    #undef SCOPE
}


void add_str_to_str_at(char* source, int index, char* insert, int insert_len) {
    #define SCOPE "add_str_to_str_at"
    int source_len = strlen(source);

    rt_assert(
        // (index < 0 || index > source_len),
        (index >= 0 && index <= source_len),
        trace_msg(SCOPE, "Index OOB.")
    );

    // Shift characters right to fit the insertion substring
    for (int i = source_len; i >= index; i--) {
        source[i + insert_len] = source[i];
    }

    // Insert substring
    for (int i = 0; i < insert_len; i++) {
        source[index + i] = insert[i];
    }
    #undef SCOPE
}
