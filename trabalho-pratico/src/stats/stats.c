#include "catalog/catalogManager.h"
#include "catalog/flightsCatalog.h"
#include "catalog/passengersCatalog.h"
#include "catalog/reservationsCatalog.h"
#include "catalog/usersCatalog.h"
#include "common.h"

typedef struct users_stats {
    char *user_id;
    int user_age;
    int n_flights;
    int total_spent;
} USERS_STATS, *Users_stats;

typedef struct reservations_stats {
    int reservation_id;
    int n_nights;
    int total_price;
} RESERVATIONS_STATS, *Reservations_stats;

typedef struct flights_stats {
    int flight_id;
    int delay;
    int n_passengers;
} FLIGHTS_STATS, *Flights_stats;

typedef struct hotels_stats {
    int hotel_id;
    double average_rating;
    double revenue;
} HOTELS_STATS, *Hotels_stats;

typedef struct aeroport_stats {
    char *aeroport_name;
    int n_passengers;
    double mediana_delays;
} AEROPORT_STATS, *Aeroport_stats;

// TODO PRECORRE O ARRAY TODO
int calculate_user_n_flights(Catalog *catalog, char *userID) {
    int n_flights = 0;
    char *name_to_compare = NULL;
    for (int i = 0; i < catalog_get_item_count(catalog); i++) {
        const Passenger passenger_temp = (const Passenger)(catalog_search_in_array(catalog, i));
        name_to_compare = get_passenger_userdID(passenger_temp);

        if (strcmp(userID, name_to_compare) == 0) {
            n_flights++;
        }

        free(name_to_compare);
    }
    return n_flights;
}

double calculate_reservation_total_price(Reservation reservation) {
    double n_nights = (get_reservation_end_date(reservation) - get_reservation_begin_date(reservation)) / 86400.0;

    unsigned int price_per_night = get_reservation_price_per_night(reservation);
    unsigned int tax = get_reservation_city_tax(reservation);

    double res = (double)(price_per_night * n_nights) + ((double)(price_per_night * n_nights) / 100.0) * tax;

    return res;
}

double calculate_user_total_spent(Catalog *catalog, char *userID, int *n_reservations) {
    double total_spent = 0;
    char *name_to_compare = NULL;
    *n_reservations = 0;
    for (int i = 0; i < catalog_get_item_count(catalog); i++) {
        const Reservation reservation_temp = (const Reservation)(catalog_search_in_array(catalog, i));
        name_to_compare = get_reservation_userID(reservation_temp);

        if (strcmp(userID, name_to_compare) == 0) {
            total_spent += calculate_reservation_total_price(reservation_temp);
            *n_reservations = *n_reservations + 1;
        }

        free(name_to_compare);
    }
    return total_spent;
}

int calculate_flight_total_passengers(Catalog *catalog, int *flightId) {
    guint matched_index = 0;
    gboolean exists = catalog_exists_in_array(catalog, flightId, &passenger_flightID_compare_func, &matched_index);
    if (exists) {
        int matched_index_down = matched_index;

        void *data1 = catalog_search_in_array(catalog, matched_index_down);
        while (get_passenger_flightID((Passenger)data1) == GPOINTER_TO_INT(flightId) && matched_index_down > 0) {
            data1 = catalog_search_in_array(catalog, --matched_index_down);
        };
        if (get_reservation_hotelID(data1) != GPOINTER_TO_INT(flightId)) matched_index_down++;

        int matched_index_up = matched_index;
        void *data2 = catalog_search_in_array(catalog, matched_index_up);
        while (get_passenger_flightID((Passenger)data2) == GPOINTER_TO_INT(flightId) && matched_index_up < catalog_get_item_count(catalog) - 1) {
            data2 = catalog_search_in_array(catalog, ++matched_index_up);
        };
        if (get_reservation_hotelID(data2) != GPOINTER_TO_INT(flightId)) matched_index_up--;

        return (matched_index_up - matched_index_down + 1);
    } else {
        printf("Flight with that id not found\n");
        return -1;
    }
}

int calculate_flight_delay(Flight flight) {
    int delay = (get_flight_schedule_departure_date(flight) - get_flight_real_departure_date(flight));
    return delay;
}

int compareIntegers(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int calculate_flight_delay_median(Catalog *catalog, char *origin_name) {
    guint matched_index = 0;
    gboolean exists = catalog_exists_in_array(catalog, origin_name, &flight_origin_compare_func, &matched_index);
    char *orig;

    if (exists) {
        // ------- DATA 1 -------
        int matched_index_down = matched_index;
        void *data1 = catalog_search_in_array(catalog, matched_index_down);
        orig = get_flight_origin((Flight)data1);

        // while (strcasecmp(origin_name, get_flight_origin((Flight*)data1)) == 0 && matched_index_down > 0) {
        while (strcasecmp(origin_name, orig) == 0 && matched_index_down > 0) {
            data1 = catalog_search_in_array(catalog, --matched_index_down);

            free(orig);
            orig = get_flight_origin((Flight)data1);
        };
        free(orig);

        orig = get_flight_origin((Flight)data1);
        if (strcasecmp(origin_name, orig) != 0) matched_index_down++;
        free(orig);

        // ------- DATA 2 -------
        int matched_index_up = matched_index;
        void *data2 = catalog_search_in_array(catalog, matched_index_up);
        orig = get_flight_origin((Flight)data2);

        // while (strcasecmp(origin_name, get_flight_origin((Flight*)data2)) == 0 && matched_index_up < (catalog_get_item_count(catalog) - 1)) {
        while (strcasecmp(origin_name, orig) == 0 && matched_index_up < (catalog_get_item_count(catalog) - 1)) {
            data2 = catalog_search_in_array(catalog, ++matched_index_up);

            free(orig);
            orig = get_flight_origin((Flight)data2);
        };
        free(orig);

        orig = get_flight_origin((Flight)data1);
        if (strcasecmp(origin_name, orig) != 0) matched_index_up--;
        free(orig);

        int i = matched_index_down;
        int quantidade_a_percorrer = (matched_index_up - matched_index_down + 1);
        int len = quantidade_a_percorrer;
        int arr[len];

        while (0 < quantidade_a_percorrer) {
            const Flight flight_temp = (const Flight)(catalog_search_in_array(catalog, i));

            arr[quantidade_a_percorrer - 1] = calculate_flight_delay(flight_temp) * (-1);

            i++;
            quantidade_a_percorrer--;
        };

        qsort(arr, len, sizeof(arr[0]), &compareIntegers);
        int mediana = len / 2;

        if ((len % 2) != 0) {
            return arr[mediana];
        } else {
            return ((arr[mediana - 1] + arr[mediana]) / 2);
        }
    } else {
        printf("Flight with that origin not found");
        return -1;
    }
}

// TODO Still a prototype... i need to add the input for the year
int calculate_aeroport_n_passengers(Catalog *flight_catalog, Catalog *passenger_catalog, char *origin_name, int *year) {
    guint matched_index = 0;
    gboolean exists = catalog_exists_in_array(flight_catalog, origin_name, &flight_origin_compare_func, &matched_index);
    if (exists) {
        int matched_index_down = matched_index;
        void *data1 = catalog_search_in_array(flight_catalog, matched_index_down);
        while (strcasecmp(origin_name, get_flight_origin((Flight)data1)) == 0 && matched_index_down > 0) {
            data1 = catalog_search_in_array(flight_catalog, --matched_index_down);
        };
        if (strcasecmp(origin_name, get_flight_origin((Flight)data1)) != 0) matched_index_down++;

        int matched_index_up = matched_index;
        void *data2 = catalog_search_in_array(flight_catalog, matched_index_up);
        while (strcasecmp(origin_name, get_flight_origin((Flight)data2)) == 0 && matched_index_up < catalog_get_item_count(flight_catalog) - 1) {
            data2 = catalog_search_in_array(flight_catalog, ++matched_index_up);
        };
        if (strcasecmp(origin_name, get_flight_origin((Flight)data1)) != 0) matched_index_up--;

        int i = matched_index_down;
        int n_passengers = 0;
        int quantidade_a_percorrer = (matched_index_up - matched_index_down + 1);
        while (0 < quantidade_a_percorrer) {
            const Flight flight_temp = (const Flight)(catalog_search_in_array(flight_catalog, i));
            int flight_year = (get_flight_schedule_departure_date(flight_temp) - DATE_OFFSET ) / (365 * 24 * 60 * 60) + 1970;
            if (*year == flight_year) {
                int flight_id = get_flight_id(flight_temp);
                n_passengers += calculate_flight_total_passengers(passenger_catalog, GINT_TO_POINTER(flight_id));
            }
            i++;
            quantidade_a_percorrer--;
        };
        return n_passengers;
    } else {
        printf("Flight with that origin not found\n");
        return -1;
    }
}

//esta função calcula o n_passengers que chegam a um aeroporto... mas obriga ao array estar ordenado pelo destino
int calculate_aeroport_n_passengers2(Catalog *flight_catalog, Catalog *passenger_catalog, char *origin_name, int *year) {
    guint matched_index = 0;
    gboolean exists = catalog_exists_in_array(flight_catalog, origin_name, &flight_destination_compare_funcB, &matched_index);
    if (exists) {
        int matched_index_down = matched_index;
        void *data1 = catalog_search_in_array(flight_catalog, matched_index_down);
        while (strcasecmp(origin_name, get_flight_destination((Flight)data1)) == 0 && matched_index_down > 0) {
            data1 = catalog_search_in_array(flight_catalog, --matched_index_down);
        };
        if (strcasecmp(origin_name, get_flight_destination((Flight)data1)) != 0) matched_index_down++;

        int matched_index_up = matched_index;
        void *data2 = catalog_search_in_array(flight_catalog, matched_index_up);
        while (strcasecmp(origin_name, get_flight_destination((Flight)data2)) == 0 && matched_index_up < catalog_get_item_count(flight_catalog) - 1) {
            data2 = catalog_search_in_array(flight_catalog, ++matched_index_up);
        };
        if (strcasecmp(origin_name, get_flight_destination((Flight)data1)) != 0) matched_index_up--;

        int i = matched_index_down;
        int n_passengers = 0;
        int quantidade_a_percorrer = (matched_index_up - matched_index_down + 1);
        while (0 < quantidade_a_percorrer) {
            const Flight flight_temp = (const Flight)(catalog_search_in_array(flight_catalog, i));
            int flight_year = (get_flight_schedule_arrival_date(flight_temp) - DATE_OFFSET ) / (365 * 24 * 60 * 60) + 1970;
            if (*year == flight_year) {
                int flight_id = get_flight_id(flight_temp);
                n_passengers += calculate_flight_total_passengers(passenger_catalog, GINT_TO_POINTER(flight_id));
            }
            i++;
            quantidade_a_percorrer--;
        };
        return n_passengers;
    } else {
        printf("Flight with that destination not found");
        return -1;
    }
}