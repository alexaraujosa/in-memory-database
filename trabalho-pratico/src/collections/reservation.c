#include "collections/reservation.h"

int verify_reservation_tokens(Tokens tokens) {
    char** parameter = tokens->data;

    // Whitespace verifier
    if(have_whitespace(parameter, 10) == 0)  return 0;
    //if(IS_STRING_NULL(parameter[11]) || IS_STRING_NULL(parameter[13]))  return 0; //BUG: Acredito que isto n seja necessario

    // Hotel_stars verifier
    if(is_integer_between_one_and_five(parameter[4]) == 0)  return 0;

    // City_tax verifier
    if(is_integer_positive_or_zero(parameter[5]) == 0)  return 0;

    // Price_per_night verifier
    if(is_integer_positive(parameter[9]) == 0)  return 0;

    // Includes_breakfast verifier
    if(!IS_STRING_NULL(parameter[10]) && !is_boolean(parameter[10]))  return 0; 
    
    // Rating verifier
    if(!IS_STRING_NULL(parameter[12]) && !is_integer_between_one_and_five(parameter[12]))  return 0;

    // Date verifier (Syntax)
    if(!is_date(parameter[7]) || !is_date(parameter[8]))  return 0;

    // Date verifier (Semantic)
    // TODO: Testar a performance entre o strcmp e a comparacao de inteiros
    if(strcmp(parameter[7], parameter[8]) >= 0)  return 0;

    return 1;
}

Reservation make_reservation(
    int id,
    UserId(user_id),
    uint8_t hotel_id,
    char hotel_name[MAX_HOTEL_NAME_LEN],
    unsigned int hotel_stars,
    unsigned int city_tax,
    int begin_date,
    int end_date,
    unsigned int price_per_night,
    bool includes_breakfast,
    int rating) {

    Reservation reservation = malloc(sizeof(RESERVATION));

    reservation->id = id;
    strncpy(reservation->user_id, user_id, MAX_USER_ID_LEN);
    reservation->hotel_id = hotel_id;
    strcpy(reservation->hotel_name, hotel_name);    //TODO: Verify if theres any problem using strcpy instead strncpy
    reservation->hotel_stars = hotel_stars;
    reservation->city_tax = city_tax;
    reservation->begin_date = begin_date;   //TODO: Date offset
    reservation->end_date = end_date;       //TODO: Date offset
    reservation->price_per_night = price_per_night;
    reservation->includes_breakfast = includes_breakfast;
    reservation->rating = rating;

    return reservation;
}

Reservation parse_reservation(Tokens tokens) {
    char** parameter = tokens->data;
    
    int id = atoi(parameter[0]);
    uint8_t hotel_id = atoi(parameter[2]);
    unsigned int hotel_stars = atoi(parameter[4]);
    unsigned int city_tax = atoi(parameter[5]);
    int begin_date = date_string_to_int(parameter[7]);
    int end_date = date_string_to_int(parameter[8]);
    unsigned int price_per_night = atoi(parameter[8]);
    bool includes_breakfast = get_boolean(parameter[10]);
    int rating = atoi(parameter[12]);
    Reservation reservation = make_reservation(id, parameter[1], hotel_id, parameter[3], hotel_stars, city_tax, begin_date, end_date, price_per_night, includes_breakfast, rating);

    return reservation;
}

void discard_reservation(void* raw_data, ParserStore store) {
    void** discard_file = &g_array_index(store, void*, 0);
    if (*discard_file == NULL) {
        char* parts[2] = {get_cwd()->str, "Resultados/reservations_errors.csv"};
        char* full_path = join_paths(parts, 2);
        *discard_file = OPEN_FILE(full_path, "w");

        void** file_header = &g_array_index(store, void*, 1);
        rt_assert(
            *file_header != NULL,
            "Could not open discard file: Dataset header missing."
        );

        fputs(*file_header, *discard_file);
        free(full_path);
    }

    Tokens tokens = (Tokens)raw_data;

    discard_to_file(tokens, store);
}

// RESERVATION parseReservationFromLine(char* line, int len) {
//     IGNORE_ARG(line);
//     IGNORE_ARG(len);
//     // TODO: Parse Reservation from CSV line
// }

void print_reservation(void* pt) {
    RESERVATION* reservation = (RESERVATION*)pt;
    char breakfast_status[10] = "No";

    if (reservation->includes_breakfast) {
        strcpy(breakfast_status, "Yes");
    }

    printf(
        "{Id:%d; "
        "UserId:%s; "
        "HotelId:%d; "
        "HotelName:%s; "
        "HotelStars:%u; "
        "CityTax:%u; "
        "BeginDate:%d; "
        "EndDate:%d; "
        "PricePerNight:%u; "
        "IncludesBreakfast:%s; "
        "Rating:%d}\n",
        reservation->id, reservation->user_id, reservation->hotel_id,
        reservation->hotel_name, reservation->hotel_stars, reservation->city_tax,
        reservation->begin_date, reservation->end_date, reservation->price_per_night,
        breakfast_status, reservation->rating);
}