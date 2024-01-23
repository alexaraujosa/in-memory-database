#include "queries/query3.h"

void query3(char flag, int argc, char** argv, void** catalogues, FILE* output_file, QueryWriter writer) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);

    Q_INFO3 information;

    guint matched_index = 0;
    int hotel_id = atoi(argv[0] + 3);
    gboolean exists = catalog_exists_in_array(catalogues[3], GINT_TO_POINTER(hotel_id), &reservationsCatalog_hotelID_compare_func, &matched_index);
    if (exists) {
        int matched_index_down = matched_index;

        void* data1 = catalog_search_in_array(catalogues[3], matched_index_down);
        while (get_reservation_hotelID((Reservation)data1) == hotel_id && matched_index_down > 0) {
            data1 = catalog_search_in_array(catalogues[3], --matched_index_down);
        };
        if (get_reservation_hotelID(data1) != hotel_id) matched_index_down++;

        int matched_index_up = matched_index;
        void* data2 = catalog_search_in_array(catalogues[3], matched_index_up);
        while (get_reservation_hotelID((Reservation)data2) == hotel_id && matched_index_up < catalog_get_item_count(catalogues[3]) - 1) {
            data2 = catalog_search_in_array(catalogues[3], ++matched_index_up);
        };
        if (get_reservation_hotelID(data2) != hotel_id) matched_index_up--;

        int i = matched_index_down;
        double rating = 0;
        int quantidade_a_percorrer = (matched_index_up - matched_index_down + 1);
        while (0 < quantidade_a_percorrer) {
            const Reservation reservation_temp = (const Reservation)(catalog_search_in_array(catalogues[3], i));
            rating += get_reservation_rating(reservation_temp);
            i++;
            quantidade_a_percorrer--;
        };
        rating /= (matched_index_up - matched_index_down + 1);

        information.rating = rating;

        writer(3, flag, &information, output_file, 1);

    } else {
        fprintf(output_file, "ERRO na query3 (Reservation with that hotel id not found)\n");
    }
}

char* query3_write(void* query_info) {
    char* output_buffer = NULL;

    Q_info3 q_info = (Q_info3)query_info;
    output_buffer = isnprintf(
        "rating: %.3f",
        q_info->rating
    );
    
    return output_buffer;
}
