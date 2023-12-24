#include "queries/queries.h"

// Queries: 1, 3, 4, 7, 8, 9

void query1(char flag, int argc, char** argv, Catalog** catalogues, FILE* output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);

    if(strncmp("Book", argv[0], 4)==0){
        void *reservation = catalog_search_in_int_hashtable(catalogues[3], atoi(argv[0]+4));

        if(reservation == NULL) return; 
        if(flag == 'F'){
            int nights = (get_reservation_end_date(reservation) - get_reservation_begin_date(reservation))/(60*60*24);
            double total_price = (double)get_reservation_price_per_night(reservation)*(double)nights+(((double)get_reservation_price_per_night(reservation)*(double)nights)/100)*(double)get_reservation_city_tax(reservation);
            int parameter = get_reservation_begin_date(reservation);
            
            parameter = parameter + DATE_OFFSET ;
            time_t converted_time = (time_t)parameter;

            struct tm *timeinfo;
            timeinfo = localtime(&converted_time);

            char* hotel_name = get_reservation_hotel_name(reservation);

            fprintf(output_file, "--- 1 ---\n");
            fprintf(output_file, "hotel_id: HTL%d\n", get_reservation_hotelID(reservation));
            fprintf(output_file, "hotel_name: %s\n", hotel_name);
            fprintf(output_file, "hotel_stars: %d\n", get_reservation_hotel_stars(reservation));
            fprintf(output_file, "begin_date: %.4d/%.2d/%.2d\n", timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday);
            
            free(hotel_name);

            parameter = get_reservation_end_date(reservation);
            parameter = parameter + DATE_OFFSET ;
            converted_time = (time_t)parameter;
            timeinfo = localtime(&converted_time);
            fprintf(output_file, "end_date: %.4d/%.2d/%.2d\n", timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday);
            fprintf(output_file, "includes_breakfast: %s\n", get_reservation_includes_breakfast(reservation) ? "True" : "False");
            fprintf(output_file, "nights: %d\n", nights);
            fprintf(output_file, "total_price: %0.3f\n", total_price);
        } else {
            int nights = (get_reservation_end_date(reservation) - get_reservation_begin_date(reservation))/(60*60*24);
            double total_price = (double)get_reservation_price_per_night(reservation)*(double)nights+(((double)get_reservation_price_per_night(reservation)*(double)nights)/100)*(double)get_reservation_city_tax(reservation);
            int parameter = get_reservation_begin_date(reservation);
            
            parameter = parameter + DATE_OFFSET ;
            time_t converted_time = (time_t)parameter;

            struct tm *timeinfo;
            timeinfo = localtime(&converted_time);

            char* hotel_name = get_reservation_hotel_name(reservation);

            fprintf(
                output_file, 
                "HTL%d;%s;%d;%.4d/%.2d/%.2d;",
                get_reservation_hotelID(reservation),
                hotel_name,
                get_reservation_hotel_stars(reservation),
                timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday
            );

            free(hotel_name);

            parameter = get_reservation_end_date(reservation);
            parameter = parameter + DATE_OFFSET ;
            converted_time = (time_t)parameter;
            timeinfo = localtime(&converted_time);

            fprintf(output_file, "%.4d/%.2d/%.2d;%s;%d;%0.3f\n", 
                timeinfo->tm_year+1900, 
                timeinfo->tm_mon+1, 
                timeinfo->tm_mday, 
                get_reservation_includes_breakfast(reservation) ? "True" : "False",
                nights,
                total_price 
            );
        }
    }
    
    if(atoi(argv[0]) > 0){
        void *flight = catalog_search_in_int_hashtable(catalogues[1], atoi(argv[0]));

        if(flight == NULL) return; 
        if(flag == 'F'){
            int parameter = get_flight_schedule_departure_date(flight);
            parameter = parameter + DATE_OFFSET ;
            time_t converted_time = (time_t)parameter;

            struct tm *timeinfo;
            timeinfo = localtime(&converted_time);
            
            char* flight_airline = get_flight_airline(flight);
            char* flight_plane_model = get_flight_plane_model(flight);
            char* flight_origin = get_flight_origin(flight);
            char* flight_destination = get_flight_destination(flight);

            fprintf(output_file, "--- 1 ---\n");
            fprintf(output_file, "airline: %s\n", flight_airline);
            fprintf(output_file, "plane_model: %s\n", flight_plane_model);
            fprintf(output_file, "origin: %s\n", flight_origin);
            fprintf(output_file, "destination: %s\n", flight_destination);
            fprintf(
                output_file, 
                "schedule_departure_date: %.4d/%.2d/%.2d %.2d:%.2d:%.2d\n", 
                timeinfo->tm_year+1900, 
                timeinfo->tm_mon+1, 
                timeinfo->tm_mday, 
                timeinfo->tm_hour, 
                timeinfo->tm_min, 
                timeinfo->tm_sec
            );

            free(flight_airline);
            free(flight_plane_model);
            free(flight_origin);
            free(flight_destination);
            
            parameter = get_flight_schedule_arrival_date(flight);
            parameter = parameter + DATE_OFFSET ;
            converted_time = (time_t)parameter;
            timeinfo = localtime(&converted_time);

            fprintf(
                output_file, 
                "schedule_arrival_date: %.4d/%.2d/%.2d %.2d:%.2d:%.2d\n", 
                timeinfo->tm_year+1900, 
                timeinfo->tm_mon+1, 
                timeinfo->tm_mday, 
                timeinfo->tm_hour, 
                timeinfo->tm_min, 
                timeinfo->tm_sec
            );
            fprintf(
                output_file, 
                "passengers: %d\n",
                calculate_flight_total_passengers(catalogues[2], GINT_TO_POINTER(get_flight_id(flight)))
            );
            fprintf(
                output_file, 
                "delay: %d\n", 
                get_flight_real_departure_date(flight) - get_flight_schedule_departure_date(flight)
            );
        } else {
            int parameter = get_flight_schedule_departure_date(flight);
            parameter = parameter + DATE_OFFSET ;
            time_t converted_time = (time_t)parameter;

            struct tm *timeinfo;
            timeinfo = localtime(&converted_time);

            char* flight_airline = get_flight_airline(flight);
            char* flight_plane_model = get_flight_plane_model(flight);
            char* flight_origin = get_flight_origin(flight);
            char* flight_destination = get_flight_destination(flight);

            fprintf(
                output_file, 
                "%s;%s;%s;%s;%.4d/%.2d/%.2d %.2d:%.2d:%.2d;", 
                flight_airline,
                flight_plane_model,
                flight_origin,
                flight_destination,
                timeinfo->tm_year+1900, 
                timeinfo->tm_mon+1, 
                timeinfo->tm_mday, 
                timeinfo->tm_hour, 
                timeinfo->tm_min, 
                timeinfo->tm_sec
            );

            free(flight_airline);
            free(flight_plane_model);
            free(flight_origin);
            free(flight_destination);

            parameter = get_flight_schedule_arrival_date(flight);
            parameter = parameter + DATE_OFFSET ;
            converted_time = (time_t)parameter;
            timeinfo = localtime(&converted_time);

            fprintf(
                output_file, 
                "%.4d/%.2d/%.2d %.2d:%.2d:%.2d;%d;%d\n", 
                timeinfo->tm_year+1900, 
                timeinfo->tm_mon+1, 
                timeinfo->tm_mday, 
                timeinfo->tm_hour, 
                timeinfo->tm_min, 
                timeinfo->tm_sec, 
                calculate_flight_total_passengers(catalogues[2], GINT_TO_POINTER(get_flight_id(flight))), 
                get_flight_real_departure_date(flight) - get_flight_schedule_departure_date(flight) 
            );
        }
    } else {
        void *user = catalog_search_in_str_hashtable(catalogues[0], argv[0]);

        if(user == NULL) return; 
        if(!get_user_account_status(user)) return;

        char* name = get_user_name(user);
        char* country_code = get_user_country_code(user);
        int n_reservas = 0;
        double total_spent = calculate_user_total_spent(catalogues[3], argv[0], &n_reservas);

        char* user_passport = get_user_passport(user);

        if(flag == 'F'){
            fprintf(output_file, "--- 1 ---\n");
            fprintf(output_file, "name: %s\n", name);
            fprintf(output_file, "sex: %s\n", get_user_sex(user) ? "M" : "F");
            fprintf(output_file, "age: %d\n", get_user_age(user));
            fprintf(output_file, "country_code: %s\n", country_code);
            fprintf(output_file, "passport: %s\n", user_passport);
            fprintf(output_file, "number_of_flights: %d\n", calculate_user_n_flights(catalogues[2], argv[0]));
            fprintf(output_file, "number_of_reservations: %d\n", n_reservas);
            fprintf(output_file, "total_spent: %.3f\n", total_spent);
        } else {
            fprintf(
                output_file, 
                "%s;%s;%d;%s;%s;%d;%d;%.3f\n", 
                name,
                get_user_sex(user) ? "M" : "F",
                get_user_age(user),
                country_code,
                user_passport,
                calculate_user_n_flights(catalogues[2], argv[0]),
                n_reservas,
                total_spent
            );
        }

        free(name);
        free(country_code);
        free(user_passport);
    }
}

void query2(char flag, int argc, char** argv, Catalog** catalogues, FILE* output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    IGNORE_ARG(catalogues);
    IGNORE_ARG(output_file);
    fputs("2", output_file);
}

void query3(char flag, int argc, char** argv, Catalog** catalogues, FILE* output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    guint matched_index = 0;
    int hotel_id = atoi(argv[0] + 3);
    gboolean exists = catalog_exists_in_array(catalogues[3], GINT_TO_POINTER(hotel_id), &reservation_hotelID_compare_func, &matched_index);
    if (exists) {
        int matched_index_down = matched_index;
        
        void *data1 = catalog_search_in_array(catalogues[3], matched_index_down);
        while (get_reservation_hotelID((Reservation)data1)==hotel_id && matched_index_down > 0) {
            data1 = catalog_search_in_array(catalogues[3], --matched_index_down);
        };
        if(get_reservation_hotelID(data1)!=hotel_id) matched_index_down++;

        int matched_index_up = matched_index;
        void *data2 = catalog_search_in_array(catalogues[3], matched_index_up);
        while (get_reservation_hotelID((Reservation)data2)==hotel_id && matched_index_up<catalog_get_item_count(catalogues[3])-1) {
            data2 = catalog_search_in_array(catalogues[3], ++matched_index_up);
        };
        if(get_reservation_hotelID(data2)!=hotel_id) matched_index_up--;
        
        
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

void query4(char flag, int argc, char** argv, Catalog** catalogues, FILE* output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);

    GArray *arrTemp = g_array_new(FALSE, FALSE, sizeof(gpointer));
    guint matched_index = 0;
    int hotel_id = atoi(argv[0] + 3);
    gboolean exists = catalog_exists_in_array(catalogues[3], GINT_TO_POINTER(hotel_id), &reservation_hotelID_compare_func, &matched_index);
    if (exists) {
        void *data = catalog_search_in_array(catalogues[3], matched_index);
        g_array_append_val(arrTemp,data);
        int matched_index_down = matched_index - 1;
        int matched_index_up = matched_index + 1;
        void *data1 = catalog_search_in_array(catalogues[3], matched_index_down);
        void *data2 = catalog_search_in_array(catalogues[3], matched_index_up);
        while (hotel_id == get_reservation_hotelID((Reservation)data1) && matched_index_down >= 0) {
            data1 = catalog_search_in_array(catalogues[3], matched_index_down);
            g_array_append_val(arrTemp,data1);
            // print_flight(data1);
            matched_index_down--;
            if(matched_index_down < 0) break;
            data1 = catalog_search_in_array(catalogues[3], matched_index_down);
            // data1 = catalog_search_in_array(catalog, matched_index_down);
            // print_flight(data1);
            
        };
        while (hotel_id == get_reservation_hotelID((Reservation)data2) && matched_index_up != catalog_get_item_count(catalogues[3])) {
            // g_array_append_val(arrTemp,data2);
            data2 = catalog_search_in_array(catalogues[3], matched_index_up);
            g_array_append_val(arrTemp,data2);
            matched_index_up++;
            // print_flight(data2);
            data2 = catalog_search_in_array(catalogues[3], matched_index_up);
        };

        g_array_sort(arrTemp, &reservation_date_compare_func);

        int count = 1;
        for (int i = 0; i < (int)arrTemp->len; i++) {
            const Reservation reservation_temp = (const Reservation)(g_array_index(arrTemp, gpointer, i));
            int parameter = get_reservation_begin_date(reservation_temp);
            parameter = parameter + DATE_OFFSET ;
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
                parameter2 = parameter2 + DATE_OFFSET ;
                time_t time_converted = (time_t)parameter2;

                timeinfo = localtime(&time_converted);

                double days = difftime(time_converted, converted_time);
                days /= 60*60*24;
                double res = get_reservation_price_per_night(reservation_temp)*days+((get_reservation_price_per_night(reservation_temp)*days)/100)*get_reservation_city_tax(reservation_temp);

                char* userId = get_reservation_userID(reservation_temp);

                fprintf(
                    output_file,
                    ";%.4d/%.2d/%.2d;%s;%d;%0.3f\n",
                    timeinfo->tm_year + 1900,
                    timeinfo->tm_mon + 1,
                    timeinfo->tm_mday,     
                    userId,
                    get_reservation_rating(reservation_temp),
                    res
                );

                free(userId);
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
                parameter2 = parameter2 + DATE_OFFSET ;
                time_t time_converted = (time_t)parameter2;

                timeinfo = localtime(&time_converted);

                double days = difftime(time_converted, converted_time);
                days /= 60*60*24;
                double res = get_reservation_price_per_night(reservation_temp)*days+((get_reservation_price_per_night(reservation_temp)*days)/100)*get_reservation_city_tax(reservation_temp);

                char* userId = get_reservation_userID(reservation_temp);

                fprintf(
                    output_file,
                    "end_date: %.4d/%.2d/%.2d\nuser_id: %s\nrating: %d\ntotal_price: %0.3f\n",
                    timeinfo->tm_year + 1900,
                    timeinfo->tm_mon + 1,
                    timeinfo->tm_mday,     
                    userId,
                    get_reservation_rating(reservation_temp),
                    res
                );

                free(userId);

                count++;
            }
        };
    };

    g_array_free(arrTemp, TRUE);
}

void query5(char flag, int argc, char** argv, Catalog** catalogues, FILE* output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);

    GArray *arrTemp = g_array_new(FALSE, FALSE, sizeof(gpointer));
    guint matched_index = 0;
    gboolean exists = catalog_exists_in_array(catalogues[1], argv[0], &flight_origin_compare_func, &matched_index);
    // void *data1, *data2;
    char* orig;

    if (exists) {
        void *data = catalog_search_in_array(catalogues[1], matched_index);
        g_array_append_val(arrTemp,data);

        int matched_index_down = matched_index - 1;
        int matched_index_up = matched_index + 1;

        void *data1 = catalog_search_in_array(catalogues[1], matched_index_down);
        void *data2 = catalog_search_in_array(catalogues[1], matched_index_up);

        orig = get_flight_origin((Flight)data1);
        while (strcasecmp(argv[0], orig) == 0 && matched_index_down >= 0) {
            data1 = catalog_search_in_array(catalogues[1], matched_index_down);
            g_array_append_val(arrTemp,data1);

            matched_index_down--;
            if(matched_index_down < 0) break;

            data1 = catalog_search_in_array(catalogues[1], matched_index_down);

            free(orig);
            orig = get_flight_origin((Flight)data1);
            
        };
        free(orig);

        orig = get_flight_origin((Flight)data2);
        while (strcasecmp(argv[0], orig) == 0 && matched_index_up + 1 < catalog_get_item_count(catalogues[1])) {
            data2 = catalog_search_in_array(catalogues[1], matched_index_up);
            g_array_append_val(arrTemp,data2);

            matched_index_up++;

            data2 = catalog_search_in_array(catalogues[1], matched_index_up);

            free(orig);
            orig = get_flight_origin((Flight)data2);
        };
        free(orig);

        g_array_sort(arrTemp, &flightsCatalog_full_compare_func);

        int count = 1;
        for (int i = 0; i < (int)arrTemp->len; i++) {
            const Flight flight_temp = (const Flight)(g_array_index(arrTemp, gpointer, i));
            if(
                date_string_withtime_to_int(argv[1]) <= get_flight_schedule_departure_date(flight_temp) 
                && get_flight_schedule_departure_date(flight_temp) <= date_string_withtime_to_int(argv[2]) 
                && flag == '\0'
            ) {
                // int parameter = 
                // parameter = parameter + DATE_OFFSET ;
                // time_t converted_time = (time_t)parameter;

                // struct tm *timeinfo;
                // timeinfo = localtime(&converted_time);

                // if(count != 1)  fprintf(output_file, "\n");
                // fprintf(
                //     output_file,
                //     "%.10d;%.4d/%.2d/%.2d %.2d:%.2d:%.2d;%s;%s;%s",
                //     get_flight_id(flight_temp),
                //     timeinfo->tm_year + 1900,
                //     timeinfo->tm_mon + 1,
                //     timeinfo->tm_mday,
                //     timeinfo->tm_hour,
                //     timeinfo->tm_min,
                //     timeinfo->tm_sec,
                //     get_flight_destination(flight_temp),
                //     get_flight_airline(flight_temp),
                //     get_flight_plane_model(flight_temp)
                // );

                int schedule_departure_date = get_flight_schedule_departure_date(flight_temp) + DATE_OFFSET ;
                // char* sched_dep_date_str = date_int_to_string(schedule_departure_date);
                time_t converted_time = (time_t)schedule_departure_date;

                struct tm *timeinfo;
                timeinfo = localtime(&converted_time);

                char* flight_destination = get_flight_destination(flight_temp);
                char* flight_airline = get_flight_airline(flight_temp);
                char* flight_plane_model = get_flight_plane_model(flight_temp);

                if(count != 1)  fprintf(output_file, "\n");
                fprintf(
                    output_file,
                    "%.10d;%.4d/%.2d/%.2d %.2d:%.2d:%.2d;%s;%s;%s",
                    get_flight_id(flight_temp),
                    // sched_dep_date_str,
                    timeinfo->tm_year + 1900,
                    timeinfo->tm_mon + 1,
                    timeinfo->tm_mday,
                    timeinfo->tm_hour,
                    timeinfo->tm_min,
                    timeinfo->tm_sec,
                    flight_destination,
                    flight_airline,
                    flight_plane_model
                );

                // free(sched_dep_date_str);
                free(flight_destination);
                free(flight_airline);
                free(flight_plane_model);

                count++;

            } else if(date_string_withtime_to_int(argv[1]) <= get_flight_schedule_departure_date(flight_temp) && get_flight_schedule_departure_date(flight_temp) <= date_string_withtime_to_int(argv[2]) && flag == 'F') {
                // int parameter = get_flight_schedule_departure_date(flight_temp);
                // parameter = parameter + DATE_OFFSET ;
                // time_t converted_time = (time_t)parameter;

                // struct tm *timeinfo;
                // timeinfo = localtime(&converted_time);
                // if(count != 1) fprintf(output_file, "\n\n");
                // fprintf(output_file, "--- %d ---\n", count);
                // fprintf(output_file, "id: %.10d\nschedule_departure_date: %.4d/%.2d/%.2d %.2d:%.2d:%.2d\ndestination: %s\nairline: %s\nplane_model: %s", 
                // get_flight_id(flight_temp), 
                // timeinfo->tm_year + 1900,
                // timeinfo->tm_mon + 1,
                // timeinfo->tm_mday,
                // timeinfo->tm_hour,
                // timeinfo->tm_min,
                // timeinfo->tm_sec,
                // get_flight_destination(flight_temp),
                // get_flight_airline(flight_temp),
                // get_flight_plane_model(flight_temp)
                // );

                int schedule_departure_date = get_flight_schedule_departure_date(flight_temp) + DATE_OFFSET ;
                // char* sched_dep_date_str = date_int_to_string(schedule_departure_date);
                time_t converted_time = (time_t)schedule_departure_date;

                struct tm *timeinfo;
                timeinfo = localtime(&converted_time);

                char* flight_destination = get_flight_destination(flight_temp);
                char* flight_airline = get_flight_airline(flight_temp);
                char* flight_plane_model = get_flight_plane_model(flight_temp);

                if(count != 1)  fprintf(output_file, "\n\n");
                fprintf(output_file, "--- %d ---\n", count);
                fprintf(
                    output_file,
                    "id: %.10d\nschedule_departure_date: %.4d/%.2d/%.2d %.2d:%.2d:%.2d\ndestination: %s\nairline: %s\nplane_model: %s",
                    get_flight_id(flight_temp),
                    // sched_dep_date_str,
                    timeinfo->tm_year + 1900,
                    timeinfo->tm_mon + 1,
                    timeinfo->tm_mday,
                    timeinfo->tm_hour,
                    timeinfo->tm_min,
                    timeinfo->tm_sec,
                    flight_destination,
                    flight_airline,
                    flight_plane_model
                );

                // free(sched_dep_date_str);
                free(flight_destination);
                free(flight_airline);
                free(flight_plane_model);

                count++;
            }
            if(i == (int)arrTemp->len - 1)   fprintf(output_file, "\n");
        };
    };

    g_array_free(arrTemp, TRUE);
}

void query6(char flag, int argc, char** argv, Catalog** catalogues, FILE* output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    IGNORE_ARG(catalogues);
    IGNORE_ARG(output_file);
    
}

struct q7_index {
    char* origin;
    int median;
};
gint compare_q7_indices(gconstpointer a, gconstpointer b, gpointer user_data) {
    IGNORE_ARG(user_data);
    
    const struct q7_index *index_a = (const struct q7_index *)a;
    const struct q7_index *index_b = (const struct q7_index *)b;

    return index_b->median - index_a->median;
}

void query7(char flag, int argc, char** argv, Catalog** catalogues, FILE* output_file) {
    IGNORE_ARG(argc);

    Catalog* flights = catalogues[1];
    int size = catalog_get_item_count(flights);

    GSequence* sequence = g_sequence_new(NULL);

    for (int i = 0; i < size; i++) {
        Flight flight = (Flight)catalog_search_in_array(flights, i);
        char* flight_origin = get_flight_origin(flight);

        int calculated = 0;

        GSequenceIter *iter = g_sequence_get_begin_iter(sequence);
        while (!g_sequence_iter_is_end(iter)) {
            const struct q7_index *ind = g_sequence_get(iter);
            if (strcmp(ind->origin, flight_origin) == 0) {
                calculated = 1;
                break;
            }

            iter = g_sequence_iter_next(iter);
        }

        if (calculated) {
            free(flight_origin);
            continue;
        }

        struct q7_index* ind = (struct q7_index*)malloc(sizeof(struct q7_index));
        ind->origin = flight_origin;

        // This function does the heavy lifting. It has cost both me and Paulo most of our sanities.
        ind->median = calculate_flight_delay_median(flights, flight_origin);

        g_sequence_insert_sorted(sequence, ind, compare_q7_indices, NULL);
    }

    GSequenceIter* iter = g_sequence_get_begin_iter(sequence);
    GSequenceIter* iter_root = iter;

    int count = 0;
    
    while (!g_sequence_iter_is_end(iter) && count < atoi(argv[0])) {
        struct q7_index *ind = g_sequence_get(iter);
        
        char* line;
        if (flag == 'F') line = isnprintf("--- %d ---\nname: %s\nmedian: %d\n", count + 1, ind->origin, ind->median);
        else line = isnprintf("%s;%d\n", ind->origin, ind->median);

        fputs(line, output_file);
        free(line);

        iter = g_sequence_iter_next(iter);
        count++;
        
        if (
            flag == 'F' && count < atoi(argv[0]) && !g_sequence_iter_is_end(iter) && count < atoi(argv[0])
        ) fputs("\n", output_file);
        else if (flag == 'F' && count < atoi(argv[0]) -1) fputs("\n", output_file);
    }

    iter = iter_root;
    while (!g_sequence_iter_is_end(iter)) {
        struct q7_index* ind = g_sequence_get(iter);
        free(ind->origin);
        free(ind);
        iter = g_sequence_iter_next(iter);
    }

    g_sequence_free(sequence);
}

void query8(char flag, int argc, char** argv, Catalog** catalogues, FILE* output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    IGNORE_ARG(catalogues);
    IGNORE_ARG(output_file);
    // printf("--|--\n");
    GArray *arrTemp = g_array_new(FALSE, FALSE, sizeof(gpointer));
    guint matched_index = 0;
    int hotel_id = atoi(argv[0] + 3);
    int begin_date = date_string_notime_to_int(argv[1]);
    int end_date = date_string_notime_to_int(argv[2]);
    gboolean exists = catalog_exists_in_array(catalogues[3], GINT_TO_POINTER(hotel_id), &reservation_hotelID_compare_func, &matched_index);
    if (exists) {
        void *data = catalog_search_in_array(catalogues[3], matched_index);
        g_array_append_val(arrTemp,data);
        int matched_index_down = matched_index - 1;
        int matched_index_up = matched_index + 1;
        void *data1 = catalog_search_in_array(catalogues[3], matched_index_down);
        void *data2 = catalog_search_in_array(catalogues[3], matched_index_up);
        while (
                hotel_id == get_reservation_hotelID((Reservation)data1) && 
                matched_index_down >= 0
            ) {
                data1 = catalog_search_in_array(catalogues[3], matched_index_down);
                g_array_append_val(arrTemp,data1);
                matched_index_down--;
                if(matched_index_down < 0) break;
                data1 = catalog_search_in_array(catalogues[3], matched_index_down);
        };
        while (
                hotel_id == get_reservation_hotelID((Reservation)data2) && 
                matched_index_up != catalog_get_item_count(catalogues[3])
            ) {
            data2 = catalog_search_in_array(catalogues[3], matched_index_up);
            g_array_append_val(arrTemp,data2);
            matched_index_up++;
            data2 = catalog_search_in_array(catalogues[3], matched_index_up);
        };

        // g_array_sort(arrTemp, &reservation_date_compare_func);
    }

    int resolution = 0;
    for (int i = 0; i < (int)arrTemp->len; i++) {
        const Reservation reservation_temp = (const Reservation)(g_array_index(arrTemp, gpointer, i));
        // print_reservation(reservation_temp);
        int start_reservation = get_reservation_begin_date(reservation_temp);
        int end_reservation = get_reservation_end_date(reservation_temp);
        if(start_reservation <= end_date && end_reservation >= begin_date) {
            if(start_reservation <= begin_date)  start_reservation = begin_date;
            if(end_reservation >= end_date)  end_reservation = end_date;
            int nights = difftime(end_reservation, start_reservation);
            nights /= 3600*24;
            if(get_reservation_end_date(reservation_temp) - end_date > 0)  nights++;
            // int res = difftime(end_date + DATE_OFFSET , begin_date + DATE_OFFSET );
            // printf("%d\n", res/(3600*24));
            // if(res/(3600*24) < 31) nights++;
        // print_reservation(reservation_temp);
            // printf("Nights: %d - Start: %s End: %s , RealStart: %s RealEnd: %s\n", nights, date_int_notime_to_string(start_reservation), date_int_notime_to_string(end_reservation), date_int_notime_to_string(get_reservation_begin_date(reservation_temp)), date_int_notime_to_string(get_reservation_end_date(reservation_temp)));
            resolution += get_reservation_price_per_night(reservation_temp) * (nights);
        }
    };
    if(flag == 'F') {
        fprintf(output_file, "--- 1 ---\n");
        fprintf(output_file, "revenue: %d\n", resolution);
    } else if(flag == '\0') {
        fprintf(output_file, "%d\n", resolution);
    }
}

void query9(char flag, int argc, char** argv, Catalog** catalogues, FILE* output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);

    guint matched_index = 0;
    gboolean exists = catalog_exists_in_array(catalogues[0], *argv, &user_username_compare_func, &matched_index);
    
    char* user_name;

    if (exists) {
        int matched_index_down = matched_index;
        void *data1 = catalog_search_in_array(catalogues[0], matched_index);
        user_name = get_user_name((User)data1);

        while (strncasecmp(*argv, user_name, strlen(*argv)) == 0 && matched_index_down > 0) {
            data1 = catalog_search_in_array(catalogues[0], --matched_index_down);
            free(user_name);
            user_name = get_user_name((User)data1);
        };
        if(strncasecmp(*argv, user_name, strlen(*argv)) != 0) matched_index_down++;
        free(user_name);

        int matched_index_up = matched_index;
        void *data2 = catalog_search_in_array(catalogues[0], matched_index_up);
        user_name = get_user_name((User)data2);
        while (strncasecmp(*argv, user_name, strlen(*argv)) == 0 && (int)matched_index_up < catalog_get_item_count(catalogues[0])-1) {
            data2 = catalog_search_in_array(catalogues[0], ++matched_index_up);
            free(user_name);
            user_name = get_user_name((User)data2);
        };
        if(strncasecmp(*argv, user_name, strlen(*argv)) != 0) matched_index_up--;
        free(user_name);

        int index = matched_index_down;
        int i = 0;
        int quantidade_a_percorrer = (matched_index_up - matched_index_down + 1);
        int count = 1;

        while(i < quantidade_a_percorrer){
            const User user_temp = (const User)(catalog_search_in_array(catalogues[0], index));
            if(get_user_account_status(user_temp) == TRUE && flag == 'F'){
                
                char* user_id = get_user_id(user_temp);
                user_name = get_user_name(user_temp);

                fprintf(output_file, "--- %d ---\n", count);
                fprintf(output_file, "id: %s\nname: %s\n\n", user_id, user_name);

                free(user_id);
                free(user_name);

                count++;
            } else if(get_user_account_status(user_temp) == TRUE && flag == '\0') {

                char* user_id = get_user_id(user_temp);
                user_name = get_user_name(user_temp);

                fprintf(output_file, "%s;%s\n", user_id, user_name);

                free(user_id);
                free(user_name);

                // if(i != arrTemp->len - 1)
                //     fprintf(output_file, "%s;%s", get_user_id(user_temp), get_user_name(user_temp));
                // else
                //     fprintf(output_file, "%s;%s", get_user_id(user_temp), get_user_name(user_temp));
            }
            index++;
            i++;
        }
        if(quantidade_a_percorrer == 0) fprintf(output_file, "\n");
    }
    else {
        printf("Account with that prefix does not exist\n");
        return;
    }
}

void query10(char flag, int argc, char** argv, Catalog** catalogues, FILE* output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    IGNORE_ARG(catalogues);
    IGNORE_ARG(output_file);
    fputs("10", output_file);
}