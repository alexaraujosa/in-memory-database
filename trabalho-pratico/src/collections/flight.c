#include "collections/flight.h"
#include "time.h"

typedef struct flights {
    int id;
    char airline[MAX_AIRLINE_NAME_LEN + 1]; // Maybe GLib String
    char plane_model[MAX_PLANE_NAME_LEN + 1]; // Maybe GLib String
    char origin[LOCATION_LEN + 1];
    char destination[LOCATION_LEN + 1];
    int schedule_departure_date; // Offset from Base Date
    int schedule_arrival_date; // Offset from Base Date
    int real_departure_date; // Offset from Base Date
} FLIGHT, *Flight;

int get_flight_id(const Flight flight){
    int flight_id = flight->id;
    return flight_id;
}

void set_flight_id(Flight flight, int id){
    flight->id = id;
}

char *get_flight_airline(const Flight flight){
    return strdup(flight->airline);
}

void set_flight_airline(Flight flight, const char *airline){
    strncpy(flight->airline, airline, sizeof(flight->airline) - 1);
    flight->airline[sizeof(flight->airline) - 1] = '\0';
}

char *get_flight_plane_model(const Flight flight){
    return strdup(flight->plane_model);
}

void set_flight_plane_model(Flight flight, const char *plane_model){
    strncpy(flight->plane_model, plane_model, sizeof(flight->plane_model) - 1);
    flight->plane_model[sizeof(flight->plane_model) - 1] = '\0';
}

char *get_flight_origin(const Flight flight){
    return strdup(flight->origin);
}

void set_flight_origin(Flight flight, const char *origin){
    strncpy(flight->origin, origin, sizeof(flight->origin) - 1);
    flight->origin[sizeof(flight->origin) - 1] = '\0';
}

char *get_flight_destination(const Flight flight){
    return strdup(flight->destination);
}

void set_flight_destination(Flight flight, const char *destination){
    strncpy(flight->destination, destination, sizeof(flight->destination) - 1);
    flight->destination[sizeof(flight->destination) - 1] = '\0';
}

int get_flight_schedule_departure_date(const Flight flight){
    int flight_schedule_departure_date = flight->schedule_departure_date;
    return flight_schedule_departure_date;
}

void set_flight_schedule_departure_date(Flight flight, int schedule_departure_date){
    flight->schedule_departure_date = schedule_departure_date;
}

int get_flight_schedule_arrival_date(const Flight flight){
    int flight_schedule_arrival_date = flight->schedule_arrival_date;
    return flight_schedule_arrival_date;
}

void set_flight_schedule_arrival_date(Flight flight, int schedule_arrival_date){
    flight->schedule_arrival_date = schedule_arrival_date;
}

int get_flight_real_departure_date(const Flight flight){
    int flight_real_departure_date = flight->real_departure_date;
    return flight_real_departure_date;
}

void set_flight_real_departure_date(Flight flight, int real_departure_date){
    flight->real_departure_date = real_departure_date;
}


int verify_flight_tokens(Tokens tokens, ParserStore store) {
    IGNORE_ARG(store);

    char** parameter = tokens->data;

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

    // Dates with time verifier (Syntax)
    for(int i = 6 ; i < 10 ; i++) {
        if(!is_date_with_time(parameter[i]))
            return 0;
    }
    // Date verifier (Semantic)
    // TODO: Testar a performance entre o strcmp e a comparacao de inteiros
    if(strcmp(parameter[6], parameter[8]) > 0)  return 0;
    if(strcmp(parameter[8], parameter[9]) >= 0)  return 0;

    return 1;
}

Flight make_flight(
    int id,
    char* airline,
    char* plane_model,
    char* origin,
    char* destination,
    int schedule_departure_date,
    int schedule_arrival_date,
    int real_departure_date) {

    Flight flight = malloc(sizeof(FLIGHT));

    flight->id = id;
    strncpy(flight->airline, airline, (size_t)MAX_AIRLINE_NAME_LEN);
    strncpy(flight->plane_model, plane_model, (size_t)MAX_PLANE_NAME_LEN);
    // strcpy(flight->origin, origin);
    strcpy(flight->origin, to_upper_string(origin));
    strcpy(flight->destination, to_upper_string(destination));
    flight->schedule_departure_date = schedule_departure_date;
    flight->schedule_arrival_date = schedule_arrival_date;
    flight->real_departure_date = real_departure_date;

    return flight;
}

void* parse_flight(Tokens tokens) {
    char** parameter = tokens->data;

    int id = atoi(parameter[0]);
    int schedule_departure_date = date_with_time_string_to_int(parameter[6]);
    int schedule_arrival_date = date_with_time_string_to_int(parameter[7]);
    int real_departure_date = date_with_time_string_to_int(parameter[8]);
    Flight flight = make_flight(id, parameter[1], parameter[2], parameter[4], parameter[5], schedule_departure_date, schedule_arrival_date, real_departure_date);
    return flight;
}

void discard_flight(void* raw_data, ParserStore store) {
    void** discard_file = &g_array_index(store, void*, 0);
    if (*discard_file == NULL) {
        // char* parts[2] = {get_cwd()->str, "Resultados/flights_errors.csv"};
        // char* full_path = join_paths(parts, 2);
        char* full_path = join_paths(2, get_cwd()->str, "Resultados/flights_errors.csv");
        *discard_file = OPEN_FILE(full_path, "w");

        void** file_header = &g_array_index(store, void*, 1);
        rt_assert(
            *file_header != NULL,
            "Could not open discard file: Dataset header missing."
        );

        fputs(*file_header, *discard_file);
        free(full_path);
    }

    Tokens tokens = (Tokens)raw_data;

    discard_to_file(tokens, store);
}

void print_flight(void* pt) {
    FLIGHT* flight = (FLIGHT*)pt;
    int parameter = flight->schedule_departure_date;
    parameter = parameter + TIME_T_SYSTEM;

    printf(
        "%.10d;"
        "%s;"
        "%s;"       
        "%s;"
        "%s;"
        "%s;"
        "%s\n",
        flight->id, 
        date_int_to_string(flight->schedule_departure_date + TIME_T_SYSTEM), 
        date_int_to_string(flight->schedule_arrival_date + TIME_T_SYSTEM), 
        date_int_to_string(flight->real_departure_date + TIME_T_SYSTEM),
        flight->destination, 
        flight->airline, 
        flight->plane_model
    );
}