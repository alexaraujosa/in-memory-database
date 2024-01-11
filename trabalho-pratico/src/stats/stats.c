#include "stats/stats.h"

typedef struct origin_delay {
    char origin[4];
    short int delay;
} ORIGIN_DELAY, *Origin_delay;

int calculate_user_n_flights(Catalog *catalog, char *userID) {
    guint matched_index = 0;
    gboolean exists = catalog_exists_in_array(catalog, userID, &passengersCatalog_userID_compare_func, &matched_index);
    if (exists) {
        int matched_index_down = matched_index;

        void *data1 = catalog_search_in_array(catalog, matched_index_down);
        while (strcmp(get_passenger_userID((Passenger)data1), userID) == 0 && matched_index_down > 0) {
            data1 = catalog_search_in_array(catalog, --matched_index_down);
        };
        if (strcmp(get_passenger_userID(data1), userID) != 0) matched_index_down++;

        int matched_index_up = matched_index;
        void *data2 = catalog_search_in_array(catalog, matched_index_up);
        while (strcmp(get_passenger_userID((Passenger)data2), userID) == 0 && matched_index_up < catalog_get_item_count(catalog) - 1) {
            data2 = catalog_search_in_array(catalog, ++matched_index_up);
        };
        if (strcmp(get_passenger_userID(data2), userID) != 0) matched_index_up--;

        return (matched_index_up - matched_index_down + 1);
    } else {
        printf("User with that id(%s) not found in passangers\n", userID);
        return -1;
    }
}

int calculate_reservation_total_price(Reservation reservation) {
    double n_nights = (get_reservation_end_date(reservation) - get_reservation_begin_date(reservation)) / 86400.0;

    unsigned int price_per_night = get_reservation_price_per_night(reservation);
    unsigned int tax = get_reservation_city_tax(reservation);

    double res = (double)(price_per_night * n_nights) + ((double)(price_per_night * n_nights) / 100.0) * tax;
    res *= 1000;

    return (int)res;
}

// TODO fazer em tempo de parsing
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

// TODO feito em tempo de parsing
int calculate_flight_total_passengers(Catalog *catalog, int *flightId) {
    guint matched_index = 0;
    gboolean exists = catalog_exists_in_array(catalog, flightId, &passengersCatalog_flightID_compare_func, &matched_index);
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

short int calculate_flight_delay(Flight flight) {
    short int delay = (get_flight_schedule_departure_date(flight) - get_flight_real_departure_date(flight));
    return delay;
}

int compareIntegers(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// TODO CRIAR UM ARRAY COM ORIGEM-DELAY ORDENADO, PARA FACILITAR QUERY 7
int calculate_flight_delay_median(Catalog *catalog, char *origin_name) {
    guint matched_index = 0;
    gboolean exists = catalog_exists_in_array(catalog, origin_name, &flightsCatalog_origin_compare_func, &matched_index);
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

int calculate_aeroport_n_passengers(Catalog *flight_catalog, char *aeroport_name, int *year) {
    int n_passengers = 0;
    
    for(int i = 0; i < catalog_get_item_count(flight_catalog); i++){
        const Flight flight_temp = (const Flight)catalog_search_in_array(flight_catalog, i);
        char *origin = get_flight_origin(flight_temp);
        char *destination = get_flight_destination(flight_temp);
        int arrival = get_flight_schedule_arrival_date(flight_temp);
        int departure = get_flight_schedule_departure_date(flight_temp);
        if (strcasecmp(aeroport_name, origin)==0 && *year == get_year(departure)){
            n_passengers += get_flight_passengers(flight_temp);
        }
        if (strcasecmp(aeroport_name, destination)==0 && *year == get_year(arrival)){
            n_passengers += get_flight_passengers(flight_temp);
        }
        free(origin);
        free(destination);
    }
    return n_passengers;
}


gint sort_origin_delay(gconstpointer a, gconstpointer b){
    Origin_delay value1 = *(Origin_delay*)a;
    Origin_delay value2 = *(Origin_delay*)b;

    if(value1->delay > value2->delay) return -1;
    if(value1->delay < value2->delay) return 1;
    return 0;
}


GArray *create_origin_delay_struct(Catalog *flights_catalog, GHashTable *origins){
    GArray *arr_origin_delay = g_array_new(FALSE, FALSE, sizeof(Origin_delay));
    
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, origins);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Origin_delay new_value = (Origin_delay)malloc(sizeof(ORIGIN_DELAY));
        int delay_median = calculate_flight_delay_median(flights_catalog, key);
        new_value->delay = delay_median;
        strcpy(new_value->origin, key);
        g_array_append_val(arr_origin_delay, new_value);
    }
    g_array_sort(arr_origin_delay, (GCompareFunc) sort_origin_delay);
    return arr_origin_delay;
}

Stats_info create_stats_info(Catalog *flights_catalog){
    Stats_info stats = (Stats_info)malloc(sizeof(STATS_INFO));
    GHashTable *origins = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);
    for(int i = 0; i < catalog_get_item_count(flights_catalog); i++){
        const Flight flight_temp = (const Flight)catalog_search_in_array(flights_catalog, i);
        char *origin = get_flight_origin(flight_temp);
        
        if(g_hash_table_lookup(origins, origin) == NULL){
            g_hash_table_add(origins, origin);
        } else {
            free(origin);
        }
    }
    stats->origin_delay = create_origin_delay_struct(flights_catalog, origins);
    for(int i = 0; i < (int)stats->origin_delay->len; i++){
        Origin_delay value = g_array_index(stats->origin_delay, Origin_delay, i);
        printf("A origem e: %s\nA mediana e:%d\n", value->origin, value->delay);
    }
    g_hash_table_destroy(origins);
    return stats;
}


//Passengers
/* 
- Já sabemos o numero de passageiros por voo
- está ordenado por nomes
*/
//Users
/* 
- Esta ordenado pelo nome e depois id 
 */
//Reservations
/* 
- Esta ordenado por hotelID, beginDate, id;
 */
//Flights
/* 
- Esta ordenado por origin, departure, arrival e id
- temos um array com todas as origens
 */