#include "catalog/catalogManager.h"
#include "catalog/reservationsCatalog.h"
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

gint reservation_hotelID_compare_func(gconstpointer a, gconstpointer b) {
    const Reservation *reservation1 = (const Reservation*)a;
    const short int hotel_id2 = (const short int *)b;

    short int hotel_id1 = get_reservation_hotelID(*reservation1);

    if (hotel_id1 < hotel_id2) return -1;
    if (hotel_id1 > hotel_id2) return 1;
    return 0;
}

// TODO Fazer com que não haja necessidade de usar arrays
double calculate_hotel_average_ratingV1(Catalog *catalog, int hotel_id) {
    GArray *arrTemp = g_array_new(FALSE, FALSE, sizeof(gpointer));
    guint matched_index = 0;
    gboolean exists = catalog_exists_in_array(catalog, hotel_id, &reservation_hotelID_compare_func, &matched_index);
    void *data1, *data2;
    if (exists) {
        guint cur_index = 0;
        void *data = catalog_search_in_array(catalog, matched_index);
        g_array_append_val(arrTemp,data);
        guint matched_index_down = matched_index - 1;
        guint matched_index_up = matched_index + 1;
        void *data1 = catalog_search_in_array(catalog, matched_index_down--);
        void *data2 = catalog_search_in_array(catalog, matched_index_up++);
        while (get_reservation_hotelID((Reservation*)data1)==hotel_id) {
            g_array_append_val(arrTemp,data1);
            data1 = catalog_search_in_array(catalog, matched_index_down--);
        };
        while (get_reservation_hotelID((Reservation*)data2)==hotel_id && matched_index_up!=catalog_get_item_count(catalog)) {
            g_array_append_val(arrTemp,data2);
            data2 = catalog_search_in_array(catalog, matched_index_up++);
        };
        int i = 0;
        double rating = 0;
        for (i = 0; i < arrTemp->len; i++) {
           const Reservation reservation_temp = (const Reservation*)(g_array_index(arrTemp, gpointer, i));
           rating += get_reservation_rating(reservation_temp);
        };
        return rating/i;
    } else {
        printf("Reservation with that hotel id not found");
        return -1;
    }
}

//TODO verificar se é possivel fazer melhorias
double calculate_hotel_average_rating(Catalog *catalog, int hotel_id) {
    guint matched_index = 0;
    gboolean exists = catalog_exists_in_array(catalog, hotel_id, &reservation_hotelID_compare_func, &matched_index);
    void *data1, *data2;
    if (exists) {
        guint matched_index_down = matched_index - 1;
        guint matched_index_up = matched_index + 1;
        void *data1 = catalog_search_in_array(catalog, matched_index_down--);
        void *data2 = catalog_search_in_array(catalog, matched_index_up++);
        while (get_reservation_hotelID((Reservation*)data1)==hotel_id && matched_index_down >= 0) {
            data1 = catalog_search_in_array(catalog, matched_index_down--);
        };
        while (get_reservation_hotelID((Reservation*)data2)==hotel_id && matched_index_up<catalog_get_item_count(catalog)) {
            data2 = catalog_search_in_array(catalog, matched_index_up++);
        };
        int i = matched_index_down+2;
        int len = 0;
        double rating = 0;
        while ( i < matched_index_up-1) {
           const Reservation reservation_temp = (const Reservation)(catalog_search_in_array(catalog,i));
           rating += get_reservation_rating(reservation_temp);
           i++;
           len++;
        };
        return (rating/len);
    } else {
        printf("Reservation with that hotel id not found");
        return -1;
    }
}