#include "output/output.h"

/*interna*/
char* build_query_buffer(int query, void* query_info, char* output_buffer) {
    switch (query) {
        case 1: {
            Q_info1 q_info = (Q_info1)query_info;
            if (q_info->type == 2) {
                snprintf(
                    output_buffer,
                    MAX_BUFFER_SIZE,
                    "hotel_id: HTL%d;hotel_name: %s;hotel_stars: %d;begin_date: %.4d/%.2d/%.2d;end_date: %.4d/%.2d/%.2d;includes_breakfast: %s;nights: %d;total_price: %.3f",
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
                    q_info->reservation_info->total_price);
            } else if (q_info->type == 1) {
                snprintf(
                    output_buffer,
                    MAX_BUFFER_SIZE,
                    "airline: %s;plane_model: %s;origin: %s;destination: %s;schedule_departure_date: %.4d/%.2d/%.2d %.2d:%.2d:%.2d;schedule_arrival_date: %.4d/%.2d/%.2d %.2d:%.2d:%.2d;passengers: %d;delay: %d",
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
                    q_info->flight_info->delay);
            } else if (q_info->type == 0) {
                snprintf(
                    output_buffer,
                    MAX_BUFFER_SIZE,
                    "name: %s;sex: %c;age: %d;country_code: %.2s;passport: %.8s;number_of_flights: %d;number_of_reservations: %d;total_spent: %.3f",
                    q_info->user_info->name,
                    q_info->user_info->sex ? 'M' : 'F',
                    q_info->user_info->age,
                    q_info->user_info->country_code,
                    q_info->user_info->passport,
                    q_info->user_info->n_flights,
                    q_info->user_info->n_reservas,
                    q_info->user_info->total_spent);
            } else {
                snprintf(
                    output_buffer,
                    MAX_BUFFER_SIZE,
                    "ERROR(Q) - Not able to create information struct");
            };
            break;
        }
        case 2: {
            Q_info2 q_info = (Q_info2)query_info;
            if (q_info->type == 0) {
                snprintf(
                    output_buffer,
                    MAX_BUFFER_SIZE,
                    "id: %.10d;date: %.4d/%.2d/%.2d",
                    q_info->id,
                    get_year(q_info->date),
                    get_month(q_info->date),
                    get_day(q_info->date)
                );
            } else if (q_info->type == 1) {
                snprintf(
                    output_buffer,
                    MAX_BUFFER_SIZE,
                    "id: Book%.10d;date: %.4d/%.2d/%.2d",
                    q_info->id,
                    get_year(q_info->date),
                    get_month(q_info->date),
                    get_day(q_info->date)
                );
            } else if (q_info->type == 2) {
                snprintf(
                    output_buffer,
                    MAX_BUFFER_SIZE,
                    "id: %s%.10d;date: %.4d/%.2d/%.2d;type: %s",
                    (q_info->both_type == 0) ? "" : "Book",
                    q_info->id,
                    get_year(q_info->date),
                    get_month(q_info->date),
                    get_day(q_info->date),
                    (q_info->both_type == 0) ? "flight" : "reservation"
                );
            } else {
                snprintf(
                    output_buffer,
                    MAX_BUFFER_SIZE,
                    "ERROR(Q) - Not able to create information struct");
            };
            break;
        }
        case 3: {
            Q_info3 q_info = (Q_info3)query_info;
            snprintf(
                output_buffer,
                MAX_BUFFER_SIZE,
                "rating: %.3f",
                q_info->rating);
            break;
        }
        case 4: {
            Q_info4 q_info = (Q_info4)query_info;
            snprintf(
                output_buffer,
                MAX_BUFFER_SIZE,
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
                q_info->price);
            break;
        }
        case 5: {
            Q_info5 q_info = (Q_info5)query_info;
            snprintf(
                output_buffer,
                MAX_BUFFER_SIZE,
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
                q_info->plane_model);
            break;
        }
        case 6: {
            Q_info6 q_info = (Q_info6)query_info;
            snprintf(
                output_buffer,
                MAX_BUFFER_SIZE,
                "name: %s;passengers: %d",
                q_info->origin,
                q_info->passangers);
            break;
        }
        case 7: {
            Q_info7 q_info = (Q_info7)query_info;
            snprintf(
                output_buffer,
                MAX_BUFFER_SIZE,
                "name: %s;median: %d",
                q_info->origin,
                q_info->median);
            break;
        }
        case 8: {
            Q_info8 q_info = (Q_info8)query_info;
            snprintf(
                output_buffer,
                MAX_BUFFER_SIZE,
                "revenue: %d",
                q_info->revenue);
            break;
        }
        case 9: {
            Q_info9 q_info = (Q_info9)query_info;
            snprintf(
                output_buffer,
                MAX_BUFFER_SIZE,
                "id: %s;name: %s",
                q_info->userID,
                q_info->user_name);
            break;
        }
        case 10: {
            Q_info10 q_info = (Q_info10)query_info;
            if (q_info->type == 0) {
                snprintf(
                    output_buffer,
                    MAX_BUFFER_SIZE,
                    "year: %d;users: %d;flights: %d;passengers: %d;unique_passengers: %d;reservations: %d",
                    q_info->date,
                    q_info->users,
                    q_info->flights,
                    q_info->passengers,
                    q_info->unique_passengers,
                    q_info->reservations
                );
            } else if (q_info->type == 1) {
                snprintf(
                    output_buffer,
                    MAX_BUFFER_SIZE,
                    "month: %d;users: %d;flights: %d;passengers: %d;unique_passengers: %d;reservations: %d",
                    q_info->date,
                    q_info->users,
                    q_info->flights,
                    q_info->passengers,
                    q_info->unique_passengers,
                    q_info->reservations
                );
            } else if (q_info->type == 2) {
                snprintf(
                    output_buffer,
                    MAX_BUFFER_SIZE,
                    "day: %d;users: %d;flights: %d;passengers: %d;unique_passengers: %d;reservations: %d",
                    q_info->date,
                    q_info->users,
                    q_info->flights,
                    q_info->passengers,
                    q_info->unique_passengers,
                    q_info->reservations);
            }
            break;
        }
    }
    return output_buffer;
}

char* _extractValueInPlace(char* keyValuePair) {
    char* colonPosition = strchr(keyValuePair, ':');

    if (colonPosition != NULL) {
        return colonPosition + 2;
    } else {
        return keyValuePair;
    }
}

/*interna*/
void output_to_file(char flag, char* line, FILE* output_file, int n_element) {
    Tokens tokens = tokenize_csv(line, strlen(line));
    char** parameter = tokens->data;
    int i = 0;
    if (flag == 'F') {
        if(n_element > 1) fprintf(output_file, "\n");
        fprintf(output_file, "--- %d ---\n", n_element);
        while (i < tokens->len) {
            fprintf(output_file, "%s\n", parameter[i]);
            i++;
        }
    } else {
        while (i < tokens->len) {
            char* newPointer = _extractValueInPlace(parameter[i]);
            fprintf(output_file, "%s", newPointer);
            i++;
            if (i < tokens->len) fprintf(output_file, ";");
        }
        fprintf(output_file, "\n");
    }
    destroy_tokens(tokens);
}

void output_query_info(int query, char flag, void* query_info, FILE* output_file, int n_element) {
    char output_buffer[1024];
    build_query_buffer(query, query_info, output_buffer);
    output_to_file(flag, output_buffer, output_file, n_element);
}