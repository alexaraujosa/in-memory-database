#include "collections/reservation.h"

typedef struct reservation {
    int id;
    UserId(user_id); // Relates to User
    short int hotel_id; // No relation, symbolic
    char hotel_name[MAX_HOTEL_NAME_LEN];
    unsigned int hotel_stars:RESERVATION_HOTEL_STARS_BF;
    unsigned int city_tax:RESERVATION_CITY_TAX_BF;
    int begin_date; // Offset from Base Date
    int end_date; // Offset from Base Date
    unsigned int price_per_night:RESERVATION_PRICE_PER_NIGHT_BF;
    bool includes_breakfast;
    int rating:RESERVATION_RATING_BF; // Depends on the scale (0-5, 0-10, etc), but can possibly be bitfielded.
} RESERVATION, *Reservation;

int get_reservation_id(const Reservation reservation){
    int id = reservation->id;
    return id;
}

void set_reservation_id(Reservation reservation, int id){
    reservation->id = id;
}

char *get_reservation_userID(const Reservation reservation){
    return strdup(reservation->user_id);
}

void set_reservation_userID(Reservation reservation, const char *user_id){
    strncpy(reservation->user_id, user_id, sizeof(reservation->user_id) - 1);
    reservation->user_id[sizeof(reservation->user_id) - 1] = '\0';
}

short int get_reservation_hotelID(const Reservation reservation){
    short int hotelID = reservation->hotel_id;
    return hotelID;
}

void set_reservation_hotelID(Reservation reservation, short int hotel_id){
    reservation->hotel_id = hotel_id;
}

char *get_reservation_hotel_name(const Reservation resarvation){
    return strdup(resarvation->hotel_name);
}

void set_reservation_hotel_name(Reservation reservation, const char *hotel_name){
   strncpy(reservation->hotel_name, hotel_name, sizeof(reservation->hotel_name) - 1);
    reservation->hotel_name[sizeof(reservation->hotel_name) - 1] = '\0';;
}

unsigned int get_reservation_hotel_stars(const Reservation reservation){
    unsigned int hotel_stars = reservation->hotel_stars;
    return hotel_stars;
}

void set_reservation_hotel_stars(Reservation reservation, unsigned int hotel_stars){
    reservation->hotel_stars = hotel_stars;
}

unsigned int get_reservation_city_tax(const Reservation reservation){
    unsigned int city_tax = reservation->city_tax;
    return  city_tax;
}

void set_reservation_city_tax(Reservation reservation, unsigned int city_tax){
    reservation->city_tax = city_tax;
}

int get_reservation_begin_date(const Reservation reservation){
    int begin_date = reservation->begin_date;
    return begin_date;
}

void set_reservation_begin_date(Reservation reservation, int begin_date){
    reservation->begin_date = begin_date;
}

int get_reservation_end_date(const Reservation reservation){
    int end_date = reservation->end_date;
    return end_date;
}

void set_reservation_end_data(Reservation reservation, int end_date){
    reservation->end_date = end_date;
}

unsigned int get_reservation_price_per_night(const Reservation reservation){
    unsigned int price_per_night = reservation->price_per_night;
    return price_per_night;
}

void set_reservation_price_per_night(Reservation reservation, unsigned int price_per_night){
    reservation->price_per_night = price_per_night;
}

bool get_reservation_includes_breakfast(const Reservation reservation){
    bool includes_breakfast = reservation->includes_breakfast;
    return includes_breakfast;
}

void set_reservation_includes_breakfast(Reservation reservation, bool includes_breakfast){
    reservation->includes_breakfast = includes_breakfast;
}

int get_reservation_rating(const Reservation reservation){
    int rating = reservation->rating;
    return rating;
}

void set_reservation_rating(Reservation reservation, int rating){
    reservation->rating = rating;
}

int verify_reservation_tokens(Tokens tokens, ParserStore store) {
    char** parameter = tokens->data;

    // Whitespace verifier
    if(have_whitespace(parameter, 10) == 0)  return 0;

    // // User verifier
    // Catalog* user_catalog = g_array_index(store, Catalog*, 2);
    // if(catalog_search_in_str_hashtable(user_catalog, parameter[1]) == NULL)  return 0;

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
    if(strcmp(parameter[7], parameter[8]) >= 0)  return 0;

    // User verifier
    Catalog* user_catalog = g_array_index(store, Catalog*, 2);
    User user = catalog_search_in_str_hashtable(user_catalog, parameter[1]);
    if(user == NULL)  return 0;

    return 1;
}

Reservation make_reservation(
    int id,
    UserId(user_id),
    short int hotel_id,
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
    strcpy(reservation->hotel_name, hotel_name);
    reservation->hotel_stars = hotel_stars;
    reservation->city_tax = city_tax;
    reservation->begin_date = begin_date;
    reservation->end_date = end_date;
    reservation->price_per_night = price_per_night;
    reservation->includes_breakfast = includes_breakfast;
    reservation->rating = rating;

    return reservation;
}

void* parse_reservation(Tokens tokens) {
    char** parameter = tokens->data;
    
    int id = atoi(parameter[0]+RESERVATION_ID_OFFSET);
    short int hotel_id = atoi(parameter[2]+HOTEL_ID_OFFSET);
    unsigned int hotel_stars = atoi(parameter[4]);
    unsigned int city_tax = atoi(parameter[5]);
    int begin_date = date_string_notime_to_int(parameter[7]);
    int end_date = date_string_notime_to_int(parameter[8]);
    unsigned int price_per_night = atoi(parameter[9]);
    bool includes_breakfast = get_boolean(parameter[10]);
    int rating = atoi(parameter[12]);
    Reservation reservation = make_reservation(id, parameter[1], hotel_id, parameter[3], hotel_stars, city_tax, begin_date, end_date, price_per_night, includes_breakfast, rating);

    return reservation;
}

void preprocessor_reservation(FILE* stream, ParserStore store, va_list args) {
    gpointer null_element = NULL;
    g_array_append_vals(store, &null_element, 1);   // Discard file
    default_csv_preprocessor(stream, store, args);  // File header
    // cvs_preprocessor_helper(stream, store);

    Catalog* catalogo = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo, 1);

    Catalog* catalogo2 = va_arg(args, Catalog*);
    g_array_append_vals(store, &catalogo2, 1);
}

void discard_reservation(void* raw_data, ParserStore store) {
    void** discard_file = &g_array_index(store, void*, 0);
    if (*discard_file == NULL) {
        // char* parts[2] = {get_cwd()->str, "Resultados/reservations_errors.csv"};
        // char* full_path = join_paths(parts, 2);
        char* full_path = join_paths(2, get_cwd()->str, "Resultados/reservations_errors.csv");
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

void print_reservation(void* pt) {
    RESERVATION* reservation = (RESERVATION*)pt;
    char breakfast_status[10] = "No";

    if (reservation->includes_breakfast) {
        strcpy(breakfast_status, "Yes");
    }

    printf(
        "{Id:%d; "
        "UserId:%s; "
        "HotelId:HTL%d; "
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

void csv_destructor_passenger_reservation(FILE* stream, ParserStore store) {
    IGNORE_ARG(stream);

    FILE* discarder = g_array_index(store, FILE*, 0);
    if (discarder != NULL) CLOSE_FILE(discarder);

    void** file_header = g_array_index(store, void**, 1);
    free(file_header);

    g_array_free(store, TRUE);
}