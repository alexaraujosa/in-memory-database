#include "queries/query4.h"

#define ERROR(code) {\
    *error = strdup(code);\
    return 1;\
}

int query4_verify(Query query, void* catalogues, char** error) {
    if (query->argc != 1) ERROR(LOCALE_QUERIES_ARGC1);

    if (!STRING_BEGIN_EQUAL(query->argv[0], "HTL", 3)) ERROR(LOCALE_QUERIES_QUERY3_INVALID_ID);

    *error = NULL;
    return 0;
}

void query4(char flag, int argc, char** argv, void** catalogues, FILE* output_file, QueryWriter writer) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);

    Q_INFO4 information;

    GArray* arrTemp = g_array_new(FALSE, FALSE, sizeof(gpointer));
    guint matched_index = 0;
    int hotel_id = atoi(argv[0] + 3);
    gboolean exists = catalog_exists_in_array(catalogues[3], GINT_TO_POINTER(hotel_id), &reservationsCatalog_hotelID_compare_func, &matched_index);
    if (exists) {
        void* data = catalog_search_in_array(catalogues[3], matched_index);
        g_array_append_val(arrTemp, data);
        int matched_index_down = matched_index - 1;
        int matched_index_up = matched_index + 1;
        void* data1 = catalog_search_in_array(catalogues[3], matched_index_down);
        void* data2 = catalog_search_in_array(catalogues[3], matched_index_up);
        while (hotel_id == get_reservation_hotelID((Reservation)data1) && matched_index_down >= 0) {
            data1 = catalog_search_in_array(catalogues[3], matched_index_down);
            g_array_append_val(arrTemp, data1);
            // print_flight(data1);
            matched_index_down--;
            if (matched_index_down < 0) break;
            data1 = catalog_search_in_array(catalogues[3], matched_index_down);
            // data1 = catalog_search_in_array(catalog, matched_index_down);
            // print_flight(data1);
        };
        while (hotel_id == get_reservation_hotelID((Reservation)data2) && matched_index_up != catalog_get_item_count(catalogues[3])) {
            // g_array_append_val(arrTemp,data2);
            data2 = catalog_search_in_array(catalogues[3], matched_index_up);
            g_array_append_val(arrTemp, data2);
            matched_index_up++;
            // print_flight(data2);
            data2 = catalog_search_in_array(catalogues[3], matched_index_up);
        };

        g_array_sort(arrTemp, &reservationsCatalog_date_compare_func);

        int count = 1;
        for (int i = 0; i < (int)arrTemp->len; i++) {
            const Reservation reservation_temp = (const Reservation)(g_array_index(arrTemp, gpointer, i));

            int first_day = get_reservation_begin_date(reservation_temp);
            int last_day = get_reservation_end_date(reservation_temp);

            double days = (last_day - first_day);
            days /= 60 * 60 * 24;

            double price = get_reservation_price_per_night(reservation_temp) * days + ((get_reservation_price_per_night(reservation_temp) * days) / 100) * get_reservation_city_tax(reservation_temp);

            information.reservationID = get_reservation_id(reservation_temp);
            information.reservation_begin_date = first_day;
            information.reservation_end_date = last_day;
            information.userID = get_reservation_userID(reservation_temp);
            information.reservation_rating = get_reservation_rating(reservation_temp);
            information.price = price;

            writer(4, flag, &information, output_file, count);

            free(information.userID);

            count++;
        };
    };
    g_array_free(arrTemp, TRUE);
}

char* query4_write(void* query_info) {
    char* output_buffer = NULL;

    Q_info4 q_info = (Q_info4)query_info;
    output_buffer = isnprintf(
        "id: Book%.10d;begin_date: %.4d/%.2d/%.2d;end_date: %.4d/%.2d/%.2d;user_id: %s;rating: %d;total_price: %0.3f",
        q_info->reservationID,
        get_year(q_info->reservation_begin_date),
        get_month(q_info->reservation_begin_date),
        get_day(q_info->reservation_begin_date),
        get_year(q_info->reservation_end_date),
        get_month(q_info->reservation_end_date),
        get_day(q_info->reservation_end_date),
        q_info->userID,
        q_info->reservation_rating,
        q_info->price
    );
    
    return output_buffer;
}
