#include "queries/query1.h"

#define ERROR(code) {\
    *error = strdup(code);\
    return 1;\
}

int query1_verify(Query query, void* catalogues, char** error) {
    if (query->argc != 1) ERROR(LOCALE_QUERIES_ARGC1);

    *error = NULL;
    return 0;
}

void query1(char flag, int argc, char** argv, void** catalogues, FILE* output_file, QueryWriter writer) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);

    Q_INFO1 information;
    information.flight_info = NULL;
    information.reservation_info = NULL;
    information.user_info = NULL;
    information.type = -1;

    if (strncmp("Book", argv[0], 4) == 0) {
        QUERY1_RESERVATION reservation_info;
        information.reservation_info = &reservation_info;
        information.type = 2;
        void* reservation = catalog_search_in_int_hashtable(catalogues[3], atoi(argv[0] + 4));

        if (reservation == NULL) return;

        int nights = (get_reservation_end_date(reservation) - get_reservation_begin_date(reservation)) / (60 * 60 * 24);

        reservation_info.hotelID = get_reservation_hotelID(reservation);
        reservation_info.hotel_name = get_reservation_hotel_name(reservation);
        reservation_info.hotel_stars = get_reservation_hotel_stars(reservation);
        reservation_info.reservation_begin_date = get_reservation_begin_date(reservation);
        reservation_info.reservation_end_date = get_reservation_end_date(reservation);
        reservation_info.breakfast = get_reservation_includes_breakfast(reservation);
        reservation_info.nights = nights;
        reservation_info.total_price = (double)get_reservation_price_per_night(reservation) * (double)nights + (((double)get_reservation_price_per_night(reservation) * (double)nights) / 100) * (double)get_reservation_city_tax(reservation);

        writer(1, flag, &information, output_file, 1);
        free(reservation_info.hotel_name);
    }
    if (atoi(argv[0]) > 0) {
        QUERY1_FLIGHT flight_info;
        information.flight_info = &flight_info;
        information.type = 1;

        void* flight = catalog_search_in_int_hashtable(catalogues[1], atoi(argv[0]));

        if (flight == NULL) return;

        flight_info.airline = get_flight_airline(flight);
        flight_info.airplane_model = get_flight_plane_model(flight);
        flight_info.origin = get_flight_origin(flight);
        flight_info.destination = get_flight_destination(flight);
        flight_info.schedule_departure_date = get_flight_schedule_departure_date(flight);
        flight_info.schedule_arrival_date = get_flight_schedule_arrival_date(flight);
        flight_info.passangers = get_flight_passengers(flight);
        flight_info.delay = get_flight_real_departure_date(flight) - information.flight_info->schedule_departure_date;

        writer(1, flag, &information, output_file, 1);

        free(flight_info.airline);
        free(flight_info.airplane_model);
        free(flight_info.origin);
        free(flight_info.destination);
    } else {
        QUERY1_USER user_info;
        information.user_info = &user_info;
        information.type = 0;
        void* user = catalog_search_in_str_hashtable(catalogues[0], argv[0]);

        if (user == NULL) return;
        if (!get_user_account_status(user)) return;

        // int n_reservas;
        // double valor = calculate_user_total_spent(catalogues[3], argv[0], &n_reservas);

        user_info.name = get_user_name(user);
        user_info.sex = get_user_sex(user);
        user_info.age = get_user_age(user);
        user_info.country_code = get_user_country_code(user);
        user_info.passport = get_user_passport(user);
        user_info.n_flights = get_user_flights_len(user);
        user_info.n_reservas = get_user_reservations_len(user);
        user_info.total_spent = get_user_total_spent(user);

        writer(1, flag, &information, output_file, 1);

        free(user_info.name);
        free(user_info.country_code);
        free(user_info.passport);
    }
}

char* query1_write(void* query_info) {
    char* output_buffer = NULL;

    Q_info1 q_info = (Q_info1)query_info;
    if (q_info->type == 2) {
        output_buffer = isnprintf(
            "hotel_id: HTL%d;hotel_name: %s;hotel_stars: %d;begin_date: %.4d/%.2d/%.2d;"
                "end_date: %.4d/%.2d/%.2d;includes_breakfast: %s;nights: %d;total_price: %.3f",
            q_info->reservation_info->hotelID,
            q_info->reservation_info->hotel_name,
            q_info->reservation_info->hotel_stars,
            get_year(q_info->reservation_info->reservation_begin_date),
            get_month(q_info->reservation_info->reservation_begin_date),
            get_day(q_info->reservation_info->reservation_begin_date),
            get_year(q_info->reservation_info->reservation_end_date),
            get_month(q_info->reservation_info->reservation_end_date),
            get_day(q_info->reservation_info->reservation_end_date),
            q_info->reservation_info->breakfast ? "True" : "False",
            q_info->reservation_info->nights,
            q_info->reservation_info->total_price
        );
    } else if (q_info->type == 1) {
        output_buffer = isnprintf(
            "airline: %s;plane_model: %s;origin: %s;destination: %s;"
                "schedule_departure_date: %.4d/%.2d/%.2d %.2d:%.2d:%.2d;"
                "schedule_arrival_date: %.4d/%.2d/%.2d %.2d:%.2d:%.2d;passengers: %d;delay: %d",
            q_info->flight_info->airline,
            q_info->flight_info->airplane_model,
            q_info->flight_info->origin,
            q_info->flight_info->destination,
            get_year(q_info->flight_info->schedule_departure_date),
            get_month(q_info->flight_info->schedule_departure_date),
            get_day(q_info->flight_info->schedule_departure_date),
            get_hour(q_info->flight_info->schedule_departure_date),
            get_minute(q_info->flight_info->schedule_departure_date),
            get_second(q_info->flight_info->schedule_departure_date),
            get_year(q_info->flight_info->schedule_arrival_date),
            get_month(q_info->flight_info->schedule_arrival_date),
            get_day(q_info->flight_info->schedule_arrival_date),
            get_hour(q_info->flight_info->schedule_arrival_date),
            get_minute(q_info->flight_info->schedule_arrival_date),
            get_second(q_info->flight_info->schedule_arrival_date),
            q_info->flight_info->passangers,
            q_info->flight_info->delay
        );
    } else if (q_info->type == 0) {
        output_buffer = isnprintf(
            "name: %s;sex: %c;age: %d;country_code: %.2s;passport: %.8s;number_of_flights: %d;"
                "number_of_reservations: %d;total_spent: %.3f",
            q_info->user_info->name,
            q_info->user_info->sex ? 'M' : 'F',
            q_info->user_info->age,
            q_info->user_info->country_code,
            q_info->user_info->passport,
            q_info->user_info->n_flights,
            q_info->user_info->n_reservas,
            q_info->user_info->total_spent);
    } else {
        output_buffer = isnprintf(
            "ERROR(Q) - Not able to create information struct"
        );
    };
    
    return output_buffer;
}