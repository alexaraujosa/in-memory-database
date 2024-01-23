#include "queries/query2.h"

gint sort_Q2(gconstpointer a, gconstpointer b){
    Q_info2 value1 = (Q_info2)a;
    Q_info2 value2 = (Q_info2)b;
    if(value1->date < value2->date) return 1;
    if(value1->date > value2->date) return -1;

    if(value1->id < value2->id) return -1;
    if(value1->id > value2->id) return 1;

    return 0;
}

void query2(char flag, int argc, char** argv, void** catalogues, FILE* output_file, QueryWriter writer) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    IGNORE_ARG(catalogues);
    IGNORE_ARG(output_file);

    Q_INFO2 information;

    void* user = catalog_search_in_str_hashtable(catalogues[0], argv[0]);

    if (user == NULL) return;
    if (!get_user_account_status(user)) return;
    GArray* arrTemp = g_array_new(FALSE, FALSE, sizeof(Q_INFO2));

    if(argv[1] != NULL) {
        if(*argv[1] == 'f') {    
            for(int i = 0 ; i < get_user_flights_len(user) ; i++) {
                void* data = get_user_flights_data(user, i);
                information.type = 0;
                information.id = get_flight_id(data);
                information.date = get_flight_schedule_departure_date(data);
                g_array_append_val(arrTemp, information);
            }

            g_array_sort(arrTemp, (GCompareFunc)sort_Q2);

            for(guint i = 0 ; i < arrTemp->len ; i++) {
                information = g_array_index(arrTemp, Q_INFO2, i);
                writer(2, flag, &information, output_file, i+1);
            }
        } else if(*argv[1] == 'r') {
            for(int i = 0 ; i < get_user_reservations_len(user) ; i++) {
                void* data = get_user_reservations_data(user, i);
                information.type = 1;
                information.id = get_reservation_id(data);
                information.date = get_reservation_begin_date(data);
                g_array_append_val(arrTemp, information);
            }

            g_array_sort(arrTemp, (GCompareFunc)sort_Q2);

            for(guint i = 0 ; i < arrTemp->len ; i++) {
                information = g_array_index(arrTemp, Q_INFO2, i);
                writer(2, flag, &information, output_file, i+1);
            }
        }
    } else {
        for(int i = 0 ; i < get_user_flights_len(user) ; i++) {
            void* data = get_user_flights_data(user, i);
            information.type = 2;
            information.both_type = 0;
            information.id = get_flight_id(data);
            information.date = get_flight_schedule_departure_date(data);
            g_array_append_val(arrTemp, information);
        }
        for(int i = 0 ; i < get_user_reservations_len(user) ; i++) {
            void* data = get_user_reservations_data(user, i);
            information.type = 2;
            information.both_type = 1;
            information.id = get_reservation_id(data);
            information.date = get_reservation_begin_date(data);
            g_array_append_val(arrTemp, information);
        }

        g_array_sort(arrTemp, (GCompareFunc)sort_Q2);

        for(guint i = 0 ; i < arrTemp->len ; i++) {
            information = g_array_index(arrTemp, Q_INFO2, i);
            writer(2, flag, &information, output_file, i+1);
        }
    }
    g_array_free(arrTemp, TRUE);
}

char* query2_write(void* query_info) {
    char* output_buffer = NULL;

    Q_info2 q_info = (Q_info2)query_info;
    if (q_info->type == 0) {
        output_buffer = isnprintf(
            "id: %.10d;date: %.4d/%.2d/%.2d",
            q_info->id,
            get_year(q_info->date),
            get_month(q_info->date),
            get_day(q_info->date)
        );
    } else if (q_info->type == 1) {
        output_buffer = isnprintf(
            "id: Book%.10d;date: %.4d/%.2d/%.2d",
            q_info->id,
            get_year(q_info->date),
            get_month(q_info->date),
            get_day(q_info->date)
        );
    } else if (q_info->type == 2) {
        output_buffer = isnprintf(
            "id: %s%.10d;date: %.4d/%.2d/%.2d;type: %s",
            (q_info->both_type == 0) ? "" : "Book",
            q_info->id,
            get_year(q_info->date),
            get_month(q_info->date),
            get_day(q_info->date),
            (q_info->both_type == 0) ? "flight" : "reservation"
        );
    } else {
        output_buffer = isnprintf(
            "ERROR(Q) - Not able to create information struct"
        );
    };
    
    return output_buffer;
}
