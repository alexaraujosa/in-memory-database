#include "stats/stats.h"

short int _calculate_flight_delay(Flight flight) {
    short int delay = (get_flight_schedule_departure_date(flight) - get_flight_real_departure_date(flight));
    return delay;
}

int _compareIntegers(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

gint _sort_origin_delay(gconstpointer a, gconstpointer b) {
    Origin_delay value1 = *(Origin_delay *)a;
    Origin_delay value2 = *(Origin_delay *)b;

    if (value1->delay > value2->delay) return -1;
    if (value1->delay < value2->delay) return 1;
    return (strcmp(value1->origin, value2->origin));
}

/*interno*/
GArray *_create_origin_delay_struct(Catalog *flights_catalog, GHashTable *origins) {
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
    g_array_sort(arr_origin_delay, (GCompareFunc)_sort_origin_delay);
    return arr_origin_delay;
}

void _create_info_tables(Stats_info stats, Catalog *flights_catalog) {
    GHashTable *aeroports = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);
    GHashTable *origins = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);
    for (int i = 0; i < catalog_get_item_count(flights_catalog); i++) {
        const Flight flight_temp = (const Flight)catalog_search_in_array(flights_catalog, i);
        char *origin = get_flight_origin(flight_temp);
        char *origin2 = get_flight_origin(flight_temp);
        char *destination = get_flight_destination(flight_temp);

        if (g_hash_table_lookup(origins, origin) == NULL) {
            g_hash_table_add(origins, origin);
        } else {
            free(origin);
        }
        if (g_hash_table_lookup(aeroports, origin2) == NULL) {
            int *passengers = (int *)malloc(sizeof(int));
            *passengers = 0;
            g_hash_table_insert(aeroports, origin2, passengers);
        } else {
            free(origin2);
        }
        if (g_hash_table_lookup(aeroports, destination) == NULL) {
            int *passengers = (int *)malloc(sizeof(int));
            *passengers = 0;
            g_hash_table_insert(aeroports, destination, passengers);
        } else {
            free(destination);
        }
    }
    stats->origins = origins;
    stats->aeroports = aeroports;
}

bool _is_in_array(GArray *arr, int num){
    bool found = false;
    for(int i = 0; i < (int)arr->len; i++){
        if(num == g_array_index(arr, int, i)) return true;
    }
    return found;
}

gint _sort_q10(gconstpointer a, gconstpointer b) {
    Date_value value1 = *(Date_value *)a;
    Date_value value2 = *(Date_value *)b;

    if (get_date_value(value1) > get_date_value(value2)) return 1;
    if (get_date_value(value1) < get_date_value(value2)) return -1;
    return 0;
} 

void _create_query10_unique_passengers(GArray* generic_catalog, Catalog* users_catalog){
    g_array_sort(generic_catalog, (GCompareFunc)_sort_q10);
    for(int i = 0; i < catalog_get_item_count(users_catalog); i++){
        GArray *years_checked = g_array_new(FALSE, FALSE, sizeof(int));
        GArray *years_month_checked = g_array_new(FALSE, FALSE, sizeof(int));
        GArray *years_month_day_checked = g_array_new(FALSE, FALSE, sizeof(int));
        User user = catalog_search_in_array(users_catalog, i);
        GArray *user_flights = get_user_flights(user); 
        for(int j = 0; j < (int)user_flights->len; j++){
            Flight flight = g_array_index(user_flights, Flight, j);
            int current_year = get_year(get_flight_schedule_departure_date(flight));
            int current_year_month = get_month(get_flight_schedule_departure_date(flight));
            int current_year_month_day = get_day(get_flight_schedule_departure_date(flight));

            if(!_is_in_array(years_checked, current_year)){
                increment_unique_passenger_by_year(current_year, generic_catalog);
                g_array_append_val(years_checked, current_year);
            }
            int value_to_append = offset_year_month(current_year, current_year_month);
            if(!_is_in_array(years_month_checked, value_to_append)){
                increment_unique_passenger_by_month(current_year, current_year_month, generic_catalog);
                g_array_append_val(years_month_checked, value_to_append);
            }
            value_to_append = offset_year_month_day(current_year, current_year_month, current_year_month_day);
            if(!_is_in_array(years_month_day_checked, value_to_append)){
                increment_unique_passenger_by_day(current_year, current_year_month, current_year_month_day, generic_catalog);
                g_array_append_val(years_month_day_checked, value_to_append);
            }
        }
        g_array_free(years_checked, TRUE);
        g_array_free(years_month_checked, TRUE);
        g_array_free(years_month_day_checked, TRUE);
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

int calculate_flight_delay_median(Catalog *flights_catalog, char *origin_name) {
    guint matched_index = 0;
    gboolean exists = catalog_exists_in_array(flights_catalog, origin_name, &flightsCatalog_origin_compare_func, &matched_index);
    char *orig;

    if (exists) {
        // ------- DATA 1 -------
        int matched_index_down = matched_index;
        void *data1 = catalog_search_in_array(flights_catalog, matched_index_down);
        orig = get_flight_origin((Flight)data1);

        // while (strcasecmp(origin_name, get_flight_origin((Flight*)data1)) == 0 && matched_index_down > 0) {
        while (strcasecmp(origin_name, orig) == 0 && matched_index_down > 0) {
            data1 = catalog_search_in_array(flights_catalog, --matched_index_down);

            free(orig);
            orig = get_flight_origin((Flight)data1);
        };
        free(orig);

        orig = get_flight_origin((Flight)data1);
        if (strcasecmp(origin_name, orig) != 0) matched_index_down++;
        free(orig);

        // ------- DATA 2 -------
        int matched_index_up = matched_index;
        void *data2 = catalog_search_in_array(flights_catalog, matched_index_up);
        orig = get_flight_origin((Flight)data2);

        // while (strcasecmp(origin_name, get_flight_origin((Flight*)data2)) == 0 && matched_index_up < (catalog_get_item_count(catalog) - 1)) {
        while (strcasecmp(origin_name, orig) == 0 && matched_index_up < (catalog_get_item_count(flights_catalog) - 1)) {
            data2 = catalog_search_in_array(flights_catalog, ++matched_index_up);

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
            const Flight flight_temp = (const Flight)(catalog_search_in_array(flights_catalog, i));

            arr[quantidade_a_percorrer - 1] = _calculate_flight_delay(flight_temp) * (-1);

            i++;
            quantidade_a_percorrer--;
        };

        qsort(arr, len, sizeof(arr[0]), &_compareIntegers);
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

void calculate_aeroport_n_passengers(GHashTable *origin_passengers, Catalog *flight_catalog, int *year) {
    for (int i = 0; i < catalog_get_item_count(flight_catalog); i++) {
        const Flight flight_temp = (const Flight)catalog_search_in_array(flight_catalog, i);
        int arrival = get_flight_schedule_arrival_date(flight_temp);
        int departure = get_flight_schedule_departure_date(flight_temp);
        if (*year == get_year(departure)) {
            char *origin = get_flight_origin(flight_temp);
            int *passengers = (int *)g_hash_table_lookup(origin_passengers, origin);
            *passengers += get_flight_passengers(flight_temp);
            free(origin);
        }
        if (*year == get_year(arrival)) {
            char *destination = get_flight_destination(flight_temp);
            int *passengers = (int *)g_hash_table_lookup(origin_passengers, destination);
            *passengers += get_flight_passengers(flight_temp);
            free(destination);
        }
    }
}

Stats_info create_stats_info(Catalog* users_catalog, Catalog *flights_catalog, GArray* generic_catalog) {
    Stats_info stats = (Stats_info)malloc(sizeof(STATS_INFO));
    _create_info_tables(stats, flights_catalog);
    stats->origin_delay = _create_origin_delay_struct(flights_catalog, stats->origins);
    _create_query10_unique_passengers(generic_catalog, users_catalog);
    stats->query10 = generic_catalog;
    return stats;
}

void stats_destroy(Stats_info stats) {
    g_hash_table_destroy(stats->origins);
    g_hash_table_destroy(stats->aeroports);
    for (int i = 0; i < (int)stats->origin_delay->len; i++) {
        free(g_array_index(stats->origin_delay, Origin_delay, i));
    }
    g_array_free(stats->origin_delay, TRUE);
    free(stats);
}
