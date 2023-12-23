#include "collections/passenger.h"

typedef struct passenger {
    int flight_id; // Relates to Flight
    UserId(user_id); // Relates to User
} PASSENGER, *Passenger;

int get_passenger_flightID(const Passenger passenger){
    int flight_id = passenger->flight_id;
    return flight_id;
}

void set_passenger_flightID(Passenger passenger, int flight_id){
    passenger->flight_id = flight_id;
}

char *get_passenger_userdID(const Passenger passenger){
    return strdup(passenger->user_id);
}

void set_passenger_userID(Passenger passenger, const char *user_id){
    strncpy(passenger->user_id, user_id, sizeof(passenger->user_id) - 1);
    passenger->user_id[sizeof(passenger->user_id) - 1] = '\0';
}

int verify_passenger_tokens(Tokens tokens, ParserStore store) {
    char** parameter = tokens->data;

    // Whitespace verifier
    if(have_whitespace(parameter, 2) == 0)  return 0;

    // User verifier
    Catalog* user_catalog = g_array_index(store, Catalog*, 2);
    if(catalog_search_in_str_hashtable(user_catalog, parameter[1]) == NULL)  return 0;

    // Flight verifier
    Catalog* flight_catalog = g_array_index(store, Catalog*, 3);
    if(catalog_search_in_int_hashtable(flight_catalog, atoi(parameter[0])) == NULL)  return 0;
    
    // // Whitespace verifier
    // if(have_whitespace(parameter, 2) == 0)  return 0;
    
    return 1;
}

Passenger make_passenger(
    int flight_id,
    UserId(user_id)) {
    
    Passenger passenger = malloc(sizeof(PASSENGER));

    passenger->flight_id = flight_id;
    strncpy(passenger->user_id, user_id, (size_t)MAX_USER_ID_LEN);

    return passenger;
}

void* parse_passenger(Tokens tokens) {
    char** parameter = tokens->data;

    int flight_id = atoi(parameter[0]);
    Passenger passenger = make_passenger(flight_id, parameter[1]);
    return passenger;
}

void discard_passenger(void* raw_data, ParserStore store) {
    void** discard_file = &g_array_index(store, void*, 0);
    if (*discard_file == NULL) {
        // char* parts[2] = {get_cwd()->str, "Resultados/passengers_errors.csv"};
        // char* full_path = join_paths(parts, 2);
        char* full_path = join_paths(2, get_cwd()->str, "Resultados/passengers_errors.csv");
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

void print_passenger(void* pt) {
    Passenger passenger = (Passenger)pt;

    printf(
        "{FlightID:%d; "
        "UserId:%s}\n"
        ,passenger->flight_id, passenger->user_id);
}