#include "collections/flight.h"

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
    strncpy(flight->origin, origin, (size_t)LOCATION_LEN);
    strncpy(flight->destination, destination, (size_t)LOCATION_LEN);
    flight->schedule_departure_date = schedule_departure_date;
    flight->schedule_arrival_date = schedule_arrival_date;      //TODO: Date offset
    flight->real_departure_date = real_departure_date;

    return flight;
}

Flight parse_flight(Tokens tokens) {
    char** parameter = tokens->data;    //TODO: Converter os valores do id char* para id int

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
        char* parts[2] = {get_cwd()->str, "Resultados/flights_errors.csv"};
        char* full_path = join_paths(parts, 2);
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

// FLIGHT parseFlightFromLine(char* line, int len) {
//     IGNORE_ARG(line);
//     IGNORE_ARG(len);
//     // TODO: Parse Flight from CSV line
// }

void print_flight(void* pt) {
    FLIGHT* flight = (FLIGHT*)pt;
    printf(
        "{Id:%d; "
        "Airline:%s; "
        "PlaneModel:%s; "
        "Origin:%s; "
        "Destination:%s; "
        "ScheduleDepartureDate:%d; "
        "ScheduleArrivalDate:%d; "
        "RealDepartureDate:%d}\n",
        flight->id, flight->airline, flight->plane_model,
        flight->origin, flight->destination, flight->schedule_departure_date,
        flight->schedule_arrival_date, flight->real_departure_date);
}