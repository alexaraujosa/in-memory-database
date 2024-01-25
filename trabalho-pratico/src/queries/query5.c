#include "queries/query5.h"

#define ERROR(code) {\
    *error = strdup(code);\
    return 1;\
}

int query5_verify(Query query, void* catalogues, char** error) {
    if (query->argc != 3) ERROR(LOCALE_QUERIES_ARGC3);

    if (strlen(query->argv[0]) != 3) ERROR(LOCALE_QUERIES_QUERY5_INVALID_NAME);

    // Not enough time to rewrite this and use get_localized_string_formatted.
    if (!is_date_with_time(query->argv[1])) ERROR(LOCALE_QUERIES_QUERY5_INVALID_BEGIN_DATE);
    if (!is_date_with_time(query->argv[2])) ERROR(LOCALE_QUERIES_QUERY5_INVALID_END_DATE);

    *error = NULL;
    return 0;
}

void query5(char flag, int argc, char** argv, void** catalogues, FILE* output_file, QueryWriter writer) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);

    Q_INFO5 information;

    GArray* arrTemp = g_array_new(FALSE, FALSE, sizeof(gpointer));
    guint matched_index = 0;
    gboolean exists = catalog_exists_in_array(catalogues[1], argv[0], &flightsCatalog_origin_compare_func, &matched_index);
    // void *data1, *data2;
    char* orig;

    if (exists) {
        void* data = catalog_search_in_array(catalogues[1], matched_index);
        g_array_append_val(arrTemp, data);

        int matched_index_down = matched_index - 1;
        int matched_index_up = matched_index + 1;

        void* data1 = catalog_search_in_array(catalogues[1], matched_index_down);
        void* data2 = catalog_search_in_array(catalogues[1], matched_index_up);

        orig = get_flight_origin((Flight)data1);
        while (strcasecmp(argv[0], orig) == 0 && matched_index_down >= 0) {
            data1 = catalog_search_in_array(catalogues[1], matched_index_down);
            g_array_append_val(arrTemp, data1);

            matched_index_down--;
            if (matched_index_down < 0) break;

            data1 = catalog_search_in_array(catalogues[1], matched_index_down);

            free(orig);
            orig = get_flight_origin((Flight)data1);
        };
        free(orig);

        orig = get_flight_origin((Flight)data2);
        while (strcasecmp(argv[0], orig) == 0 && matched_index_up + 1 < catalog_get_item_count(catalogues[1])) {
            data2 = catalog_search_in_array(catalogues[1], matched_index_up);
            g_array_append_val(arrTemp, data2);

            matched_index_up++;

            data2 = catalog_search_in_array(catalogues[1], matched_index_up);

            free(orig);
            orig = get_flight_origin((Flight)data2);
        };
        free(orig);

        g_array_sort(arrTemp, &flightsCatalog_full_compare_func);
        // bool activated = FALSE;
        int count = 1;
        for (int i = 0; i < (int)arrTemp->len; i++) {
            const Flight flight_temp = (const Flight)(g_array_index(arrTemp, gpointer, i));
            if (date_string_withtime_to_int(argv[1]) <= get_flight_schedule_departure_date(flight_temp) && get_flight_schedule_departure_date(flight_temp) <= date_string_withtime_to_int(argv[2])) {
                // activated = TRUE;

                information.flight_id = get_flight_id(flight_temp);
                information.schedule_departure_date = get_flight_schedule_departure_date(flight_temp);
                information.destination = get_flight_destination(flight_temp);
                information.airline = get_flight_airline(flight_temp);
                information.plane_model = get_flight_plane_model(flight_temp);

                writer(5, flag, &information, output_file, count);

                free(information.destination);
                free(information.airline);
                free(information.plane_model);

                count++;
            }
            // if(i == (int)arrTemp->len - 1 && activated == TRUE)   fprintf(output_file, "\n");
            // PAULO - NÃO SEI SE ISTO VAI SER UTIL POR ISSO NÃO APAGO
        };
    };

    g_array_free(arrTemp, TRUE);
}

char* query5_write(void* query_info) {
    char* output_buffer = NULL;

    Q_info5 q_info = (Q_info5)query_info;
    output_buffer = isnprintf(
        "id: %.10d;schedule_departure_date: %.4d/%.2d/%.2d %.2d:%.2d:%.2d;destination: %s;airline: %s;plane_model: %s",
        q_info->flight_id,
        get_year(q_info->schedule_departure_date),
        get_month(q_info->schedule_departure_date),
        get_day(q_info->schedule_departure_date),
        get_hour(q_info->schedule_departure_date),
        get_minute(q_info->schedule_departure_date),
        get_second(q_info->schedule_departure_date),
        q_info->destination,
        q_info->airline,
        q_info->plane_model
    );
    
    return output_buffer;
}
