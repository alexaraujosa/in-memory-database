#include "queries/queries.h"

// Queries: 1, 3, 4, 7, 8, 9

void query1(char flag, int argc, char** argv, Catalog** catalogues, FILE** output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    fputs("1", output_file);
}

void query2(char flag, int argc, char** argv, Catalog** catalogues, FILE** output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    fputs("2", output_file);
}

void query3(char flag, int argc, char** argv, Catalog** catalogues, FILE** output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    guint matched_index = 0;
    int hotel_id = atoi(argv[0] + 3);
    gboolean exists = catalog_exists_in_array(catalogues[3], hotel_id, &reservation_hotelID_compare_func, &matched_index);
    void *data1, *data2;
    if (exists) {
        int matched_index_down = matched_index;
        
        void *data1 = catalog_search_in_array(catalogues[3], matched_index_down);
        while (get_reservation_hotelID((Reservation*)data1)==hotel_id && matched_index_down > 0) {
            data1 = catalog_search_in_array(catalogues[3], --matched_index_down);
        };
        if(get_reservation_hotelID((Reservation*)data1)!=hotel_id) matched_index_down++;

        int matched_index_up = matched_index;
        void *data2 = catalog_search_in_array(catalogues[3], matched_index_up);
        while (get_reservation_hotelID((Reservation*)data2)==hotel_id && matched_index_up<catalog_get_item_count(catalogues[3])-1) {
            data2 = catalog_search_in_array(catalogues[3], ++matched_index_up);
        };
        if(get_reservation_hotelID((Reservation*)data2)!=hotel_id) matched_index_up--;
        
        
        int i = matched_index_down;
        double rating = 0;
        int quantidade_a_percorrer = (matched_index_up - matched_index_down + 1);
        while ( 0 < quantidade_a_percorrer) {
           const Reservation reservation_temp = (const Reservation)(catalog_search_in_array(catalogues[3],i));
           rating += get_reservation_rating(reservation_temp);
           i++;
           quantidade_a_percorrer--;
        };
        rating /= (matched_index_up - matched_index_down + 1);
        if(flag == 'F'){
            fprintf(output_file, "--- 1 ---\n");
            fprintf(output_file, "rating: %.3f\n", rating);
        }
        else{
            fprintf(output_file, "%.3f\n", rating);
        }
    } else {
        fprintf(output_file, "Reservation with that hotel id not found\n");
    }
}

void query4(char flag, int argc, char** argv, Catalog** catalogues, FILE** output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);

    GArray *arrTemp = g_array_new(FALSE, FALSE, sizeof(gpointer));
    guint matched_index = 0;
    gboolean exists = catalog_exists_in_array(catalogues[3], atoi(argv[0]+3), &reservation_hotelID_compare_func, &matched_index);
    void *data1, *data2;
    if (exists) {
        guint cur_index = 0;
        void *data = catalog_search_in_array(catalogues[3], matched_index);
        g_array_append_val(arrTemp,data);
        int matched_index_down = matched_index - 1;
        int matched_index_up = matched_index + 1;
        void *data1 = catalog_search_in_array(catalogues[3], matched_index_down);
        void *data2 = catalog_search_in_array(catalogues[3], matched_index_up);
        while (atoi(argv[0]+3) == get_reservation_hotelID((Reservation*)data1) && matched_index_down >= 0) {
            data1 = catalog_search_in_array(catalogues[3], matched_index_down);
            g_array_append_val(arrTemp,data1);
            // print_flight(data1);
            matched_index_down--;
            if(matched_index_down < 0) break;
            data1 = catalog_search_in_array(catalogues[3], matched_index_down);
            // data1 = catalog_search_in_array(catalog, matched_index_down);
            // print_flight(data1);
            
        };
        while (atoi(argv[0]+3) == get_reservation_hotelID((Reservation*)data2) && matched_index_up != catalog_get_item_count(catalogues[3])) {
            // g_array_append_val(arrTemp,data2);
            data2 = catalog_search_in_array(catalogues[3], matched_index_up);
            g_array_append_val(arrTemp,data2);
            matched_index_up++;
            // print_flight(data2);
            data2 = catalog_search_in_array(catalogues[3], matched_index_up);
        };

        g_array_sort(arrTemp, &reservation_date_compare_func);

        int count = 1;
        for (int i = 0; i < arrTemp->len; i++) {
            const Reservation reservation_temp = (const Reservation*)(g_array_index(arrTemp, gpointer, i));
            int parameter = get_reservation_begin_date(reservation_temp);
            parameter = parameter + TIME_T_SYSTEM;
            time_t converted_time = (time_t)parameter;

            struct tm *timeinfo;
            timeinfo = localtime(&converted_time);
            if(flag == '\0') {
                fprintf(
                    output_file, 
                    "Book%.10d;%.4d/%.2d/%.2d",
                    get_reservation_id(reservation_temp),
                    timeinfo->tm_year + 1900,
                    timeinfo->tm_mon + 1,
                    timeinfo->tm_mday
                );

                int parameter2 = get_reservation_end_date(reservation_temp);
                parameter2 = parameter2 + TIME_T_SYSTEM;
                time_t time_converted = (time_t)parameter2;

                timeinfo = localtime(&time_converted);

                float days = difftime(time_converted, converted_time);
                days /= 60*60*24;
                float res = get_reservation_price_per_night(reservation_temp)*days+((get_reservation_price_per_night(reservation_temp)*days)/100)*get_reservation_city_tax(reservation_temp);

                fprintf(
                    output_file,
                    ";%.4d/%.2d/%.2d;%s;%d;%0.3f\n",
                    timeinfo->tm_year + 1900,
                    timeinfo->tm_mon + 1,
                    timeinfo->tm_mday,     
                    get_reservation_userID(reservation_temp),
                    get_reservation_rating(reservation_temp),
                    res
                );
            } else if(flag == 'F') {
                if(i != 0) fprintf(output_file, "\n");
                fprintf(
                    output_file, 
                    "--- %d ---\nid: Book%.10d\nbegin_date: %.4d/%.2d/%.2d\n",
                    count,
                    get_reservation_id(reservation_temp),
                    timeinfo->tm_year + 1900,
                    timeinfo->tm_mon + 1,
                    timeinfo->tm_mday
                );

                int parameter2 = get_reservation_end_date(reservation_temp);
                parameter2 = parameter2 + TIME_T_SYSTEM;
                time_t time_converted = (time_t)parameter2;

                timeinfo = localtime(&time_converted);

                float days = difftime(time_converted, converted_time);
                days /= 60*60*24;
                float res = get_reservation_price_per_night(reservation_temp)*days+((get_reservation_price_per_night(reservation_temp)*days)/100)*get_reservation_city_tax(reservation_temp);

                fprintf(
                    output_file,
                    "end_date: %.4d/%.2d/%.2d\nuser_id: %s\nrating: %d\ntotal_price: %0.3f\n",
                    timeinfo->tm_year + 1900,
                    timeinfo->tm_mon + 1,
                    timeinfo->tm_mday,     
                    get_reservation_userID(reservation_temp),
                    get_reservation_rating(reservation_temp),
                    res
                );
                count++;
            }
        };
    };

}

void query5(char flag, int argc, char** argv, Catalog** catalogues, FILE** output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);

    GArray *arrTemp = g_array_new(FALSE, FALSE, sizeof(gpointer));
    guint matched_index = 0;
    gboolean exists = catalog_exists_in_array(catalogues[1], argv[0], &flight_origin_compare_func, &matched_index);
    void *data1, *data2;
    if (exists) {
        guint cur_index = 0;
        void *data = catalog_search_in_array(catalogues[1], matched_index);
        g_array_append_val(arrTemp,data);
        int matched_index_down = matched_index - 1;
        int matched_index_up = matched_index + 1;
        void *data1 = catalog_search_in_array(catalogues[1], matched_index_down);
        void *data2 = catalog_search_in_array(catalogues[1], matched_index_up);
        while (strcasecmp(argv[0], get_flight_origin((Flight*)data1)) == 0 && matched_index_down >= 0) {
            data1 = catalog_search_in_array(catalogues[1], matched_index_down);
            g_array_append_val(arrTemp,data1);
            // print_flight(data1);
            matched_index_down--;
            if(matched_index_down < 0) break;
            data1 = catalog_search_in_array(catalogues[1], matched_index_down);
            // data1 = catalog_search_in_array(catalog, matched_index_down);
            // print_flight(data1);
            
        };
        while (strcasecmp(argv[0], get_flight_origin((Flight*)data2)) == 0 && matched_index_up != catalog_get_item_count(catalogues[1])) {
            // g_array_append_val(arrTemp,data2);
            data2 = catalog_search_in_array(catalogues[1], matched_index_up);
            g_array_append_val(arrTemp,data2);
            matched_index_up++;
            // print_flight(data2);
            data2 = catalog_search_in_array(catalogues[1], matched_index_up);
        };

        g_array_sort(arrTemp, &flightsCatalog_full_compare_func);

        int count = 1;
        for (int i = 0; i < arrTemp->len; i++) {
            const Flight flight_temp = (const Flight*)(g_array_index(arrTemp, gpointer, i));
            if(date_with_time_string_to_int(argv[1]) <= get_flight_schedule_departure_date(flight_temp) && get_flight_schedule_departure_date(flight_temp) <= date_with_time_string_to_int(argv[2]) && flag == '\0') {
                int parameter = get_flight_schedule_departure_date(flight_temp);
                parameter = parameter + TIME_T_SYSTEM;
                time_t converted_time = (time_t)parameter;

                struct tm *timeinfo;
                timeinfo = localtime(&converted_time);

                if(count != 1)  fprintf(output_file, "\n");
                fprintf(
                    output_file,
                    "%.10d;%.4d/%.2d/%.2d %.2d:%.2d:%.2d;%s;%s;%s",
                    get_flight_id(flight_temp),
                    timeinfo->tm_year + 1900,
                    timeinfo->tm_mon + 1,
                    timeinfo->tm_mday,
                    timeinfo->tm_hour,
                    timeinfo->tm_min,
                    timeinfo->tm_sec,
                    get_flight_destination(flight_temp),
                    get_flight_airline(flight_temp),
                    get_flight_plane_model(flight_temp)
                );
                count++;
                // if(i != arrTemp->len - 1)   fprintf(output_file, "\n");

            } else if(date_with_time_string_to_int(argv[1]) <= get_flight_schedule_departure_date(flight_temp) && get_flight_schedule_departure_date(flight_temp) <= date_with_time_string_to_int(argv[2]) && flag == 'F') {
                int parameter = get_flight_schedule_departure_date(flight_temp);
                parameter = parameter + TIME_T_SYSTEM;
                time_t converted_time = (time_t)parameter;

                struct tm *timeinfo;
                timeinfo = localtime(&converted_time);
                if(count != 1) fprintf(output_file, "\n\n");
                fprintf(output_file, "--- %d ---\n", count);
                fprintf(output_file, "id: %.10d\nschedule_departure_date: %.4d/%.2d/%.2d %.2d:%.2d:%.2d\ndestination: %s\nairline: %s\nplane_model: %s", 
                get_flight_id(flight_temp), 
                timeinfo->tm_year + 1900,
                timeinfo->tm_mon + 1,
                timeinfo->tm_mday,
                timeinfo->tm_hour,
                timeinfo->tm_min,
                timeinfo->tm_sec,
                get_flight_destination(flight_temp),
                get_flight_airline(flight_temp),
                get_flight_plane_model(flight_temp)
                );
                count++;
                // if(i == arrTemp->len - 1)   fprintf(output_file, "\n");
            }
            if(i == arrTemp->len - 1)   fprintf(output_file, "\n");
        };
    };
}

void query6(char flag, int argc, char** argv, Catalog** catalogues, FILE** output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    fputs("6", output_file);
}

void query7(char flag, int argc, char** argv, Catalog** catalogues, FILE** output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    fputs("7", output_file);

    // Catalog* flights = catalogues[1];
    // int size = catalog_get_item_count(flights);
    // for (int i = 0; i < size; i++) {

    // }
}

void query8(char flag, int argc, char** argv, Catalog** catalogues, FILE** output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    fputs("8", output_file);
}

void query9(char flag, int argc, char** argv, Catalog** catalogues, FILE** output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);

    GArray *arrTemp = g_array_new(FALSE, FALSE, sizeof(gpointer));
    guint matched_index = 0;
    gboolean exists = catalog_exists_in_array(catalogues[0], *argv, &user_username_compare_func, &matched_index);
    void *data1, *data2;
    if (exists) {
        guint cur_index = 0;
        void *data = catalog_search_in_array(catalogues[0], matched_index);
        g_array_append_val(arrTemp,data);
        guint matched_index_down = matched_index - 1;
        guint matched_index_up = matched_index + 1;
        void *data1 = catalog_search_in_array(catalogues[0], matched_index_down--);
        void *data2 = catalog_search_in_array(catalogues[0], matched_index_up++);
        while (strncasecmp(*argv, get_user_name((User*)data1), strlen(*argv)) == 0 && matched_index_down > 0) {
            g_array_append_val(arrTemp,data1);
            data1 = catalog_search_in_array(catalogues[0], matched_index_down--);
        };
        while (strncasecmp(*argv, get_user_name((User*)data2), strlen(*argv)) == 0 && matched_index_up != catalog_get_item_count(catalogues[0])) {
            g_array_append_val(arrTemp,data2);
            data2 = catalog_search_in_array(catalogues[0], matched_index_up++);
        };
        g_array_sort(arrTemp, &users_full_compare_func);

        int count = 1;
        for (int i = 0; i < arrTemp->len; i++) {
           const User user_temp = (const User*)(g_array_index(arrTemp, gpointer, i));
           if(get_user_account_status(user_temp) == TRUE && flag == 'F'){
                if(i != 0) fprintf(output_file, "\n\n");
                fprintf(output_file, "--- %d ---\n", count);
                fprintf(output_file, "id: %s\nname: %s", get_user_id(user_temp), get_user_name(user_temp));

                count++;
           } else if(get_user_account_status(user_temp) == TRUE && flag == '\0') {
                if(i != 0) fprintf(output_file, "\n");
                if(i != arrTemp->len - 1)
                    fprintf(output_file, "%s;%s", get_user_id(user_temp), get_user_name(user_temp));
                else
                    fprintf(output_file, "%s;%s", get_user_id(user_temp), get_user_name(user_temp));
           }
                if(i == arrTemp->len - 1)
                    fprintf(output_file, "\n");
        };
    }
}

void query10(char flag, int argc, char** argv, Catalog** catalogues, FILE** output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    fputs("10", output_file);
}