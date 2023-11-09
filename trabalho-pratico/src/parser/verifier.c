#include "parser/verifier.h"

int verify_user_tokens(Tokens tokens) {
    const char** parameter = tokens->data;

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


int verify_reservation_tokens(Tokens tokens) {
    const char** parameter = tokens->data;

    // Whitespace verifier
    if(have_whitespace(parameter, 10) == 0)  return 0;
    if(IS_STRING_NULL(parameter[11]) || IS_STRING_NULL(parameter[13]))  return 0;

    // Hotel_stars verifier
    if(is_integer_between_one_and_five(parameter[4]) == 0)  return 0;

    // City_tax verifier
    if(is_integer_positive_or_zero(parameter[5]) == 0)  return 0;

    // Price_per_night verifier
    if(is_integer_positive(parameter[9]) == 0)  return 0;

    // Includes_breakfast verifier
    if(!IS_STRING_NULL(parameter[10]) && !is_boolean(parameter[10]))  return 0; 
    
    // Rating verifier
    if(!IS_STRING_NULL(parameter[12]) && !is_integer_between_one_and_five(parameter[12]))  return 0;

    // Date verifier (Syntax)
    if(!is_date(parameter[7]) || !is_date(parameter[8]))  return 0;

    // Date verifier (Semantic)
    // TODO: Testar a performance entre o strcmp e a comparacao de inteiros
    if(strcmp(parameter[7], parameter[8]) >= 0)  return 0;

    return 1;
}


int verify_flight_tokens(Tokens tokens) {
    const char** parameter = tokens->data;

    // Whitespace verifier
    if(have_whitespace(parameter, 12) == 0)  return 0;

    // Origin verifier
    if(!is_length(parameter[4], 3))  return 0;
    for(int i = 0 ; i < 3 ; i++) {
        if(!isalpha(parameter[4][i]))  return 0;
    }
    // Destination verifier
    if(!is_length(parameter[5], 3))  return 0;
    for(int i = 0 ; i < 3 ; i++) {
        if(!isalpha(parameter[5][i]))  return 0;
    }
    // Total_seats first verifier (Syntax)
    if(!is_integer_positive(parameter[3]))  return 0;
    // TODO: total_seats verifier

    // Dates with time verifier (Syntax)
    for(int i = 6 ; i < 10 ; i++) {
        if(!is_date_with_time(parameter[i]))
            return 0;
    }
    // Date verifier (Semantic)
    // TODO: Testar a performance entre o strcmp e a comparacao de inteiros
    if(strcmp(parameter[6], parameter[8]) >= 0)  return 0;
    if(strcmp(parameter[8], parameter[9]) >= 0)  return 0;

    return 1;
}


int verify_passenger_tokens(Tokens tokens) {
    const char** parameter = tokens->data;

    // Whitespace verifier
    if(have_whitespace(parameter, 2) == 0)  return 0;
    
    return 1;
}