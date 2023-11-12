#include "collections/flight.h"

FLIGHT makeFlight(
    int id,
    char* airline,
    char* plane_model,
    char* origin,
    char* destination,
    int schedule_departure_date,
    int schedule_arrival_date,
    int real_departure_date) {
    rt_assert(
        strlen(airline) > 0 && strlen(airline) <= MAX_AIRLINE_NAME_LEN,
        isnprintf("Flight Airline must be a string with length up to %d (got %d)", MAX_AIRLINE_NAME_LEN, strlen(airline)));
    rt_assert(
        strlen(plane_model) > 0 && strlen(plane_model) <= MAX_PLANE_NAME_LEN,
        isnprintf("Flight Plane Model must be a string with length up to %d (got %d)", MAX_PLANE_NAME_LEN, strlen(plane_model)));
    rt_assert(
        strlen(origin) == LOCATION_LEN,
        isnprintf("Flight Origin must be a string with length %d (got %d)", LOCATION_LEN, strlen(origin)));
    rt_assert(
        strlen(destination) == LOCATION_LEN,
        isnprintf("Flight Destination must be a string with length %d (got %d)", LOCATION_LEN, strlen(destination)));

    FLIGHT flight = {
        .id = id,
        .schedule_departure_date = schedule_departure_date,
        .schedule_arrival_date = schedule_arrival_date,
        .real_departure_date = real_departure_date};

    strncpy(flight.airline, airline, (size_t)MAX_AIRLINE_NAME_LEN);
    strncpy(flight.plane_model, plane_model, (size_t)MAX_PLANE_NAME_LEN);
    strncpy(flight.origin, origin, (size_t)LOCATION_LEN);
    strncpy(flight.destination, destination, (size_t)LOCATION_LEN);

    return flight;
}

FLIGHT parseFlightFromLine(char* line, int len) {
    IGNORE_ARG(line);
    IGNORE_ARG(len);
    // TODO: Parse Flight from CSV line
}

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