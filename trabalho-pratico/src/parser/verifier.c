#include "parser/verifier.h"

//TODO: Ponderar o uso de const antes do parametero char* nas funcoes, se os valores nao forem modificados,
//TODO: apenas acedidos, usar o const que o compilador vai otimizar

int verify_user_tokens(char* parameter[]) {
    // Whitespace verifier
    if(have_whitespace(parameter, 12) == 0)  return 0;
    
    // Email verifier
    int email_length = strlen(parameter[2]);
    if(!isalpha(parameter[2][email_length - 1]) || !is_email(parameter[2], email_length))  return 0;

    // Country Code verifier
    if(!is_length(parameter[7], 2) || !isalpha(parameter[7][0]) || !isalpha(parameter[7][1]))  return 0;

    // Account Status verifier
    if(strcasecmp(parameter[11], "active") && strcasecmp(parameter[11], "inactive"))  return 0; 

    //TODO: Verificacao da data de nascimento (fazer com uma funcao verify_date, que vai ser usada na verify_tokens_flight,etc...)
    //TODO: Verificacao da account_creation (fazer com a verify_date_time, que vai ver a data, mas como esta tem tempo tem de ser diferente da verify_date)

    return 1;
}


int verify_reservation_tokens(char** parameter) {
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
    if(!IS_STRING_NULL(parameter[10]) && !is_boolean(parameter[10]))
     return 0; 
    
    // Rating verifier
    if(!IS_STRING_NULL(parameter[12]) && !is_integer_between_one_and_five(parameter[12]))
     return 0;
    // TODO: Date verifier para a begin_date,end_date
    return 1;
}


int verify_flight_tokens(char** parameter) {
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
    // TODO: schedule verifiers

    return 1;
}


int verify_passenger_tokens(char** parameter) {
    // Whitespace verifier
    if(have_whitespace(parameter, 2) == 0)  return 0;
    
    return 1;
}