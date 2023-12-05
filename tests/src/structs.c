// #define _POSIX_C_SOURCE 199309L
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <printf.h>
#include <unistd.h>

#define BILLION  1000000000L
#include <time.h>

// Lenght of the largest portuguese proper noun: 
// "Manuel Maria Filipe Carlos Amélio Luís Miguel Rafael Gabriel Gonzaga Xavier "
// "Francisco de Assis Eugénio de Bragança Orleães Sabóia e Saxe-Coburgo-Gotha"
#define MAX_NAME_LEN 157

#define MAX_HOTEL_NAME 128

// // Length of the largest city in the world:
// // "Taumatawhakatangihangakoauauotamateaturipukakapikimaungahoronukupokaiwhenuakitar"
// #define MAX_ORIGIN_SIZE 81
#define MAX_ORIGIN_SIZE 3

// Lenght of largest on dataset: "Ukraine International Airlines"
#define MAX_AIRLINE_SIZE 31

#define ID_OFFSET 32

#pragma region ======= UTILS =======
#define CREATE_MASK(a, b) ((1u << ((b) - (a) + 1)) - 1) << (a)

// Reason: Fast as fuck boi
volatile int numPlaces (int n) {
    if (n < 0) n = (n == INT_MIN) ? INT_MAX : -n;
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    if (n < 10000000) return 7;
    if (n < 100000000) return 8;
    if (n < 1000000000) return 9;
    return 10;
}

#pragma region ------- PRINT SPECIFIERS -------
static int bool_arginfo(const struct printf_info *info, size_t n, int *argtypes, int *size) {
    if (n) {
        argtypes[0] = PA_INT;
        *size = sizeof(bool);
    }
    return 1;
}
static int bool_printf(FILE *stream, const struct printf_info *info, const void *const *args) {
    bool b =  *(const bool*)(args[0]);
    int r = fputs(b ? "true" : "false", stream);
    return r == EOF ? -1 : (b ? 4 : 5);
}
static int setup_bool_specifier() {
    int r = register_printf_specifier('B', bool_printf, bool_arginfo);
    return r;
}
static int setup_specifiers() {
    int r;

    r = register_printf_specifier('B', bool_printf, bool_arginfo);
    if (r) return r;

    return r;
}
#pragma endregion
#pragma endregion

#pragma region ======= ID =======

typedef struct id {
    char header;     // 8 bits
    unsigned int id; // 9 digits - 27 bits
} ID, *Id;

// #define RawId Id

// volatile RawId makeId(char header, unsigned int data) {
//     if (numPlaces(data) != 9) return NULL;

//     Id id = (Id)malloc(sizeof(ID));
//     id->header = header;
//     id->id = data;

//     return id;
// }

// ID parseId(RawId id) {
//     return *id;
// }

#define RawId uint64_t

volatile uint64_t makeId(char header, unsigned int data) {
    if (numPlaces(data) != 9) return -1;

    return ((uint64_t)header << ID_OFFSET) | data;
}

ID parseId(uint64_t id) {
    return (ID){
        (char)(id >> ID_OFFSET),
        (unsigned int)id
    };
}
#pragma endregion

typedef struct user {
    RawId id;
    char name[MAX_NAME_LEN];
    int phone_number;
    uint8_t age; // Precalculated
    bool sex;
    uint16_t country_code;
    int account_creation; // Offset from Base Date
    bool account_status;
} USER, *User;

typedef struct reservation {
    RawId id;
    RawId user_id; // Relates to User
    RawId hotel_id; // No relation, symbolic
    char hotel_name[MAX_HOTEL_NAME];
    unsigned int hotel_stars:5;
    unsigned int city_tax:7;
    int begin_date; // Offset from Base Date
    int end_date; // Offset from Base Date
    int price_per_night;
    bool includes_breakfast;
    int rating:5; // Depends on the scale (0-5, 0-10, etc), but can possibly be bitfielded.
} RESERVATION, *Reservation;

typedef struct flights {
    RawId id;
    char* airline; // Maybe GLib String
    char* plane_model; // Maybe GLib String
    char origin[MAX_ORIGIN_SIZE];
    char destination[MAX_ORIGIN_SIZE];
    int schedule_departure_date; // Offset from Base Date
    int schedule_arrival_date; // Offset from Base Date
    int real_arrival_date; // Offset from Base Date
} FLIGHT, *Flight;

typedef struct passenger {
    RawId flight_id; // Relates to Flight
    RawId user_id; // Relates to User
} PASSENGER, *Passenger;

enum { NS_PER_SECOND = 1000000000 };

void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td) {
    td->tv_nsec = t2.tv_nsec - t1.tv_nsec;
    td->tv_sec  = t2.tv_sec - t1.tv_sec;

    if (td->tv_sec > 0 && td->tv_nsec < 0) {
        td->tv_nsec += NS_PER_SECOND;
        td->tv_sec--;
    } else if (td->tv_sec < 0 && td->tv_nsec > 0) {
        td->tv_nsec -= NS_PER_SECOND;
        td->tv_sec++;
    }
}

#pragma region ======= USER TESTING =======
int main() {
    struct timespec start, finish, delta;
    clock_gettime(CLOCK_REALTIME, &start);

    /* ==================================================================0 */
    // RawId id = ((uint64_t)'U' << ID_OFFSET) | 123456789;
    char wtf[MAX_NAME_LEN] = "Manuel Maria Filipe Carlos Amélio Luís Miguel Rafael Gabriel Gonzaga "
    "Xavier Francisco de Assis Eugénio de Bragança Orleães Sabóia e Saxe-Coburgo-Gotha";
    int wtf_len = strlen(wtf) * sizeof(char) * 8;

    printf("WTF size: %d | %d\n", wtf_len, (int)floor(log2(wtf_len) + 1));

    char name[9] = "John Doe";
    USER u = {
        makeId('U', 123456789),
        "Manuel Maria Filipe Carlos Amélio Luís Miguel Rafael Gabriel Gonzaga "
        "Xavier Francisco de Assis Eugénio de Bragança Orleães Sabóia e Saxe-Coburgo-Gotha",
        123456789,
        255,
        1,
        351,
        123456,
        1
    };
    
    printf("U ADDR: %p\n", &u);
    fflush(stdout);

    ID pid = parseId(u.id);
    printf("PARSED ID: '%c' %d\n", pid.header, pid.id);
    // printf("ID: '%c' %d\n", (char)(u.id >> ID_OFFSET), (int)(u.id & (1 << ID_OFFSET) - 1));
    
    printf("The user has the data %p | %s | %d | %d | %d | %d | %d | %d\n", 
        u.id, u.name, u.phone_number, u.age, u.sex, u.country_code, u.account_creation, u.account_status
    );
    printf("The size of user is %ld bytes.\n", sizeof(USER));
    fflush(NULL);
    // return 0; 
    /* ==================================================================0 */

    clock_gettime(CLOCK_REALTIME, &finish);
    sub_timespec(start, finish, &delta);
    printf("%d.%.9ld\n", (int)delta.tv_sec, delta.tv_nsec);
    return 0;
}
#pragma endregion

#pragma region ======= RESERVATION TESTING =======
// int main() {
//     int registered = setup_specifiers();
//     if (registered) return 1;

//     RESERVATION r = {
//         makeId('R', 999999999),
//         makeId('U', 123456789),
//         makeId('H', 147896325),
//         "Your mother's",
//         5,
//         69,
//         123456,
//         654321,
//         420,
//         false,
//         5
//     };

//     ID prid = parseId(r.id);
//     printf("PARSED RESERVATION ID: '%c' %d\n", prid.header, prid.id);
//     ID puid = parseId(r.user_id);
//     printf("PARSED USER ID: '%c' %d\n", puid.header, puid.id);
//     ID phid = parseId(r.hotel_id);
//     printf("PARSED GOTEL ID: '%c' %d\n", phid.header, phid.id);
    
//     printf("The reservation has the data %ld | %ld | %ld | %s | %d | %d%% | %d | %d | %d | %B | %d\n", 
//         r.id, r.user_id, r.hotel_id, r.hotel_name, r.hotel_stars, r.city_tax, r.begin_date, r.end_date, r.price_per_night, r.includes_breakfast, r.rating
//     );
//     printf("The size of reservation is %ld bytes.\n", sizeof(RESERVATION));
//     fflush(NULL);
//     return 0; 
// }
#pragma endregion

#pragma region ======= FLIGHT TESTING =======
// int main() {
//     int registered = setup_specifiers();
//     if (registered) return 1;

//     FLIGHT f = {
//         makeId('F', 123456789),
//         (void*)0x7fffffff,
//         (void*)0x7fffffff,
//         "My house",
//         "Your mother's house",
//         123456,
//         654321,
//         632541
//     };

//     ID pfid = parseId(f.id);
//     printf("PARSED FLIGHT ID: '%c' %d\n", pfid.header, pfid.id);
    
//     printf("The flight has the data %ld | %p | %p | %s | %s | %d | %d | %d\n", 
//         f.id, f.airline, f.plane_model, f.origin, f.destination, f.schedule_departure_date, f.schedule_arrival_date, f.real_arrival_date
//     );
//     printf("The size of flight is %ld bytes.\n", sizeof(FLIGHT));
//     fflush(NULL);
//     return 0; 
// }
#pragma endregion
