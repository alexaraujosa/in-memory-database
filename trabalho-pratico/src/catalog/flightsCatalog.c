#include "catalog/catalogManager.h"
#include "collections/flight.h"

gint flights(gconstpointer a, gconstpointer b, gpointer user_data) {
    const FLIGHT* flights1 = (const FLIGHT*)a;
    const FLIGHT* flights2 = (const FLIGHT*)b;

    if (strcmp(flights1->origin, flights2->origin) < 0) return -1;
    if (strcmp(flights1->origin, flights2->origin) > 0) return 1;
    if (flights1->schedule_departure_date < flights2->schedule_departure_date) return -1;
    if (flights1->schedule_departure_date > flights2->schedule_departure_date) return 1;
    if (flights1->schedule_arrival_date < flights2->schedule_arrival_date) return -1;
    if (flights1->schedule_arrival_date > flights2->schedule_arrival_date) return 1;
    if (flights1->id < flights2->id) return -1;
    if (flights1->id > flights2->id) return 1;
    return 0;
}

void writeFlight(FLIGHT* fligh, Catalog* flighCatalog) {
    catalog_add_to_catalog(flighCatalog, fligh->id, fligh, fligh);
}

void printFlight(void *pt) {
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
        flight->schedule_arrival_date, flight->real_departure_date
    );
}