#include "queries/query8.h"

void query8(char flag, int argc, char** argv, void** catalogues, FILE* output_file, QueryWriter writer) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    IGNORE_ARG(catalogues);
    IGNORE_ARG(output_file);

    Q_INFO8 information;

    GArray* arrTemp = g_array_new(FALSE, FALSE, sizeof(gpointer));
    guint matched_index = 0;
    int hotel_id = atoi(argv[0] + 3);
    int begin_date = date_string_notime_to_int(argv[1]);
    int end_date = date_string_notime_to_int(argv[2]);
    gboolean exists = catalog_exists_in_array(catalogues[3], GINT_TO_POINTER(hotel_id), &reservationsCatalog_hotelID_compare_func, &matched_index);
    if (exists) {
        int matched_index_down = matched_index;
        void* data1 = catalog_search_in_array(catalogues[3], matched_index_down);

        g_array_append_val(arrTemp, data1);

        while (
            hotel_id == get_reservation_hotelID((Reservation)data1) &&
            matched_index_down > 0) {
            data1 = catalog_search_in_array(catalogues[3], --matched_index_down);
            g_array_append_val(arrTemp, data1);
        };
        if(hotel_id != get_reservation_hotelID((Reservation)data1)) g_array_remove_index(arrTemp, arrTemp->len - 1);

        int matched_index_up = matched_index;
        void* data2 = catalog_search_in_array(catalogues[3], matched_index_up);
        while (
            hotel_id == get_reservation_hotelID((Reservation)data2) &&
            (int)matched_index_up < catalog_get_item_count(catalogues[3])-1) {
            data2 = catalog_search_in_array(catalogues[3], ++matched_index_up);
            g_array_append_val(arrTemp, data2);
        };
        if(hotel_id != get_reservation_hotelID((Reservation)data2)) g_array_remove_index(arrTemp, arrTemp->len - 1);

        // g_array_sort(arrTemp, &reservation_date_compare_func);
    }

    int resolution = 0;
    for (int i = 0; i < (int)arrTemp->len; i++) {
        const Reservation reservation_temp = (const Reservation)(g_array_index(arrTemp, gpointer, i));
        // print_reservation(reservation_temp);
        int start_reservation = get_reservation_begin_date(reservation_temp);
        int end_reservation = get_reservation_end_date(reservation_temp);
        if (start_reservation <= end_date && end_reservation >= begin_date) {
            if (start_reservation <= begin_date) start_reservation = begin_date;
            if (end_reservation >= end_date) end_reservation = end_date;
            int nights = difftime(end_reservation, start_reservation);
            nights /= 3600 * 24;
            if (get_reservation_end_date(reservation_temp) - end_date > 0) nights++;
            // int res = difftime(end_date - DATE_OFFSET , begin_date - DATE_OFFSET );
            // printf("%d\n", res/(3600*24));
            // if(res/(3600*24) < 31) nights++;
            // print_reservation(reservation_temp);
            // printf("Nights: %d - Start: %s End: %s , RealStart: %s RealEnd: %s\n", nights, date_int_notime_to_string(start_reservation), date_int_notime_to_string(end_reservation), date_int_notime_to_string(get_reservation_begin_date(reservation_temp)), date_int_notime_to_string(get_reservation_end_date(reservation_temp)));
            resolution += get_reservation_price_per_night(reservation_temp) * (nights);
        }
    };

    information.revenue = resolution;

    writer(8, flag, &information, output_file, 1);
    g_array_free(arrTemp, TRUE);
}

char* query8_write(void* query_info) {
    char* output_buffer = NULL;

    Q_info8 q_info = (Q_info8)query_info;
    output_buffer = isnprintf(
        "revenue: %d",
        q_info->revenue
    );
    
    return output_buffer;
}
