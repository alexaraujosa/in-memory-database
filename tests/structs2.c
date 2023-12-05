#include "./structs2.h"

typedef struct user {
    UserId(id);
    char name[MAX_NAME_LEN];
    bool sex;
    uint16_t country_code;
    int account_creation; // Offset from Base Date
    bool account_status;

    // Statistics
    uint8_t age;
} USER, *User;

typedef struct reservation {
    int id;
    UserId(user_id); // Relates to User
    uint8_t hotel_id; // No relation, symbolic
    char hotel_name[MAX_HOTEL_NAME_LEN];
    unsigned int hotel_stars:5;
    unsigned int city_tax:7;
    int begin_date; // Offset from Base Date
    int end_date; // Offset from Base Date
    unsigned int price_per_night:15;
    bool includes_breakfast;
    int rating:5; // Depends on the scale (0-5, 0-10, etc), but can possibly be bitfielded.
} RESERVATION, *Reservation;

typedef struct flights {
    int id;
    char airline[MAX_AIRLINE_NAME_LEN]; // Maybe GLib String
    char plane_model[MAX_PLANE_NAME_LEN]; // Maybe GLib String
    char origin[LOCATION_LEN];
    char destination[LOCATION_LEN];
    int schedule_departure_date; // Offset from Base Date
    int schedule_arrival_date; // Offset from Base Date
    int real_departure_date; // Offset from Base Date
} FLIGHT, *Flight;