#include "queries/query10.h"

bool all_zero_Q10(Date_value info){
    bool empty = true;
    if(get_conteudo_flights(info) != 0) return !empty; 
    if(get_conteudo_passengers(info) != 0) return !empty; 
    if(get_conteudo_unique_passengers(info) != 0) return !empty; 
    if(get_conteudo_reservations(info) != 0) return !empty; 
    if(get_conteudo_users(info) != 0) return !empty; 
    return empty;
}

void query10(char flag, int argc, char** argv, void** catalogues, FILE* output_file, QueryWriter writer) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    IGNORE_ARG(catalogues);
    IGNORE_ARG(output_file);

    Q_INFO10 information;
    Stats_info stats = catalogues[4];

    if(argc == 0) {
        for(int i = 0, number = 1; i < (int)stats->query10->len ; i++) {
            Date_value info = g_array_index(stats->query10, Date_value, i);
            if (all_zero_Q10(info)) continue;
            information.type = 0;
            information.date = get_date_value(info);
            information.unique_passengers = get_conteudo_unique_passengers(info);
            information.users = get_conteudo_users(info);
            information.passengers = get_conteudo_passengers(info);
            information.flights = get_conteudo_flights(info);
            information.reservations = get_conteudo_reservations(info);
            writer(10, flag, &information, output_file, number++);
        }
    } else if(argc == 1) {
        Date_value info_year = NULL;
        Date_value info_month = NULL;
        for(int i = 0; i < (int)stats->query10->len ; i++){
            info_year = g_array_index(stats->query10, Date_value, i);
            if(get_date_value(info_year) == atoi(argv[0])) break;
        }
        GArray* array_w_months = get_lower_array(info_year);  
        for(int i = 0, number = 1; i < (int)array_w_months->len; i++){
            info_month = g_array_index(array_w_months, Date_value, i);
            if (all_zero_Q10(info_month)) continue;
            information.type = 1;
            information.date = get_date_value(info_month);
            information.unique_passengers = get_conteudo_unique_passengers(info_month);
            information.users = get_conteudo_users(info_month);
            information.passengers = get_conteudo_passengers(info_month);
            information.flights = get_conteudo_flights(info_month);
            information.reservations = get_conteudo_reservations(info_month);
            writer(10, flag, &information, output_file, number++);
        }
    } else if(argc == 2) {
        Date_value info_year = NULL;
        Date_value info_month = NULL;
        Date_value info_day = NULL;
        for(int i = 0; i < (int)stats->query10->len ; i++){
            info_year = g_array_index(stats->query10, Date_value, i);
            if(get_date_value(info_year) == atoi(argv[0])) break;
        }
        GArray* array_w_months = get_lower_array(info_year);
        for(int i = 0; i < (int)array_w_months->len ; i++){
            info_month = g_array_index(array_w_months, Date_value, i);
            if(get_date_value(info_month) == atoi(argv[1])) break;
        }
        GArray* array_w_days = get_lower_array(info_month);
        for(int i = 0, number = 1; i < (int)array_w_days->len; i++){
            info_day = g_array_index(array_w_days, Date_value, i);
            if (all_zero_Q10(info_day)) continue;
            information.type = 2;
            information.date = get_date_value(info_day);
            information.unique_passengers = get_conteudo_unique_passengers(info_day);
            information.users = get_conteudo_users(info_day);
            information.passengers = get_conteudo_passengers(info_day);
            information.flights = get_conteudo_flights(info_day);
            information.reservations = get_conteudo_reservations(info_day);
            writer(10, flag, &information, output_file, number++);
        }
    }
}

char* query10_write(void* query_info) {
    char* output_buffer = NULL;

    Q_info10 q_info = (Q_info10)query_info;
    if (q_info->type == 0) {
        output_buffer = isnprintf(
            "year: %d;users: %d;flights: %d;passengers: %d;unique_passengers: %d;reservations: %d",
            q_info->date,
            q_info->users,
            q_info->flights,
            q_info->passengers,
            q_info->unique_passengers,
            q_info->reservations
        );
    } else if (q_info->type == 1) {
        output_buffer = isnprintf(
            "month: %d;users: %d;flights: %d;passengers: %d;unique_passengers: %d;reservations: %d",
            q_info->date,
            q_info->users,
            q_info->flights,
            q_info->passengers,
            q_info->unique_passengers,
            q_info->reservations
        );
    } else if (q_info->type == 2) {
        output_buffer = isnprintf(
            "day: %d;users: %d;flights: %d;passengers: %d;unique_passengers: %d;reservations: %d",
            q_info->date,
            q_info->users,
            q_info->flights,
            q_info->passengers,
            q_info->unique_passengers,
            q_info->reservations
        );
    }
    
    return output_buffer;
}
