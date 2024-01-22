#include "catalog/genericCatalog.h"

typedef struct conteudo {
    int users;
    int flights;
    int passengers;
    int unique_passengers;
    int reservations;
} CONTEUDO, *Conteudo;

typedef struct date_value {
    int date_value;
    CONTEUDO info;
    GHashTable *unique_passengers;  // VERIFICAR SE O PASSAGEIRO é unico ou não
    GArray *lower_array;
} DATE_VALUE, *Date_value;

//

int get_date_value(Date_value info) { 
    return info->date_value;
}

GArray *get_lower_array(Date_value info) { 
    return info->lower_array;
}

int get_conteudo_users(Date_value info) {
    return info->info.users;
}

int get_conteudo_flights(Date_value info) {
    return info->info.flights;
}

int get_conteudo_passengers(Date_value info) {
    return info->info.passengers;
}

int get_conteudo_unique_passengers(Date_value info) {
    return info->info.unique_passengers;
}

int get_conteudo_reservations(Date_value info) {
    return info->info.reservations;
}

GArray *generate_genCat() {
    GArray *years = g_array_new(FALSE, FALSE, sizeof(Date_value));
    return years;
}

GArray *generate_genCat_days() {
    GArray *days = g_array_new(FALSE, FALSE, sizeof(Date_value));
    for (int day = 1; day <= 31; day++) {
        // GHashTable *unique_passengers_day = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, NULL);
        Date_value new_day = (Date_value)malloc(sizeof(DATE_VALUE));
        new_day->date_value = day;
        new_day->info.flights = 0;
        new_day->info.reservations = 0;
        new_day->info.passengers = 0;
        new_day->info.users = 0;
        new_day->info.unique_passengers = 0;
        new_day->lower_array = NULL;
        g_array_append_val(days, new_day);
    }
    return days;
}

GArray *generate_genCat_months() {
    GArray *months = g_array_new(FALSE, FALSE, sizeof(Date_value));
    for (int month = 1; month <= 12; month++) {
        // GHashTable *unique_passengers_month = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, NULL);
        Date_value new_month = (Date_value)malloc(sizeof(DATE_VALUE));
        new_month->date_value = month;
        new_month->info.flights = 0;
        new_month->info.reservations = 0;
        new_month->info.passengers = 0;
        new_month->info.users = 0;
        new_month->info.unique_passengers = 0;
        new_month->lower_array = generate_genCat_days();
        g_array_append_val(months, new_month);
    }
    return months;
}

void genCat_add(int year, GArray *arr_ptr) {
    for (int i = 0; i < (int)arr_ptr->len; i++) {
        Date_value value = g_array_index(arr_ptr, Date_value, i);
        if (value->date_value == year) return;
    }
    // GHashTable *unique_passengers_year = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, NULL);
    Date_value new_year = (Date_value)malloc(sizeof(DATE_VALUE));
    new_year->date_value = year;
    new_year->info.flights = 0;
    new_year->info.reservations = 0;
    new_year->info.passengers = 0;
    new_year->info.users = 0;
    new_year->info.unique_passengers = 0;
    new_year->lower_array = generate_genCat_months();
    g_array_append_val(arr_ptr, new_year);
}

int get_flights_by_year(int year, GArray* arr) {
    Conteudo info = conteudo_by_year(year, arr);
    return info->flights;
}

Conteudo conteudo_by_year(int year, GArray *arr_ptr) {
    Conteudo info = NULL;
    for (int i = 0; i < (int)arr_ptr->len; i++) {
        Date_value value = g_array_index(arr_ptr, Date_value, i);
        if (value->date_value == year) {
            info = &value->info;
            break;
        }
    }
    return info;
}

Conteudo conteudo_by_month(int year, int month, GArray *arr_ptr) {
    Conteudo info = NULL;
    Date_value value_year = NULL;
    Date_value value_month = NULL;
    for (int i = 0; i < (int)arr_ptr->len; i++) {
        value_year = g_array_index(arr_ptr, Date_value, i);
        if (value_year->date_value == year) {
            for (int j = 0; j < (int)value_year->lower_array->len; j++) {
                value_month = g_array_index(value_year->lower_array, Date_value, i);
                if (value_month->date_value == month) {
                    info = &value_month->info;
                    break;
                }
            }
        }
    }
    return info;
}

Conteudo conteudo_by_day(int year, int month, int day, GArray *arr_ptr) {
    Conteudo info = NULL;
    Date_value value_year = NULL;
    Date_value value_month = NULL;
    Date_value value_day = NULL;
    for (int i = 0; i < (int)arr_ptr->len; i++) {
        value_year = g_array_index(arr_ptr, Date_value, i);
        if (value_year->date_value == year) {
            for (int j = 0; j < (int)value_year->lower_array->len; j++) {
                value_month = g_array_index(value_year->lower_array, Date_value, i);
                if (value_month->date_value == month) {
                    for (int i = 0; i < (int)value_month->lower_array->len; i++) {
                        value_day = g_array_index(value_month->lower_array, Date_value, i);
                        if (value_day->date_value == day) {
                            info = &value_day->info;
                            break;
                        }
                    }
                }
            }
        }
    }
    return info;
}

Conteudo increment_conteudo(int year, int month, int day, GArray *arr_ptr) {
    Conteudo info = NULL;
    Date_value value_year = NULL;
    Date_value value_month = NULL;
    Date_value value_day = NULL;
    for (int i = 0; i < (int)arr_ptr->len; i++) {
        value_year = g_array_index(arr_ptr, Date_value, i);
        if (value_year->date_value == year) {
            for (int j = 0; j < (int)value_year->lower_array->len; j++) {
                value_month = g_array_index(value_year->lower_array, Date_value, j);
                if (value_month->date_value == month) {
                    for (int k = 0; k < (int)value_month->lower_array->len; k++) {
                        value_day = g_array_index(value_month->lower_array, Date_value, k);
                        if (value_day->date_value == day) {
                            info = &value_day->info;
                            break;
                        }
                    }
                }
            }
        }
    }
    return info;
}

void increment_user_conteudo(int year, int month, int day, GArray *arr_ptr) {
    Date_value value_year = NULL;
    Date_value value_month = NULL;
    Date_value value_day = NULL;
    for (int i = 0; i < (int)arr_ptr->len; i++) {
        value_year = g_array_index(arr_ptr, Date_value, i);
        if (value_year->date_value == year) {
            value_year->info.users++;
            for (int j = 0; j < (int)value_year->lower_array->len; j++) {
                value_month = g_array_index(value_year->lower_array, Date_value, j);
                if (value_month->date_value == month) {
                    value_month->info.users++;
                    for (int k = 0; k < (int)value_month->lower_array->len; k++) {
                        value_day = g_array_index(value_month->lower_array, Date_value, k);
                        if (value_day->date_value == day) {
                            value_day->info.users++;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void increment_flight_conteudo(int year, int month, int day, GArray *arr_ptr) {
    Date_value value_year = NULL;
    Date_value value_month = NULL;
    Date_value value_day = NULL;
    for (int i = 0; i < (int)arr_ptr->len; i++) {
        value_year = g_array_index(arr_ptr, Date_value, i);
        if (value_year->date_value == year) {   
            value_year->info.flights++;
            for (int j = 0; j < (int)value_year->lower_array->len; j++) {
                value_month = g_array_index(value_year->lower_array, Date_value, j);
                if (value_month->date_value == month) {
                    value_month->info.flights++;
                    for (int k = 0; k < (int)value_month->lower_array->len; k++) {
                        value_day = g_array_index(value_month->lower_array, Date_value, k);
                        if (value_day->date_value == day) {
                            value_day->info.flights++;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void increment_reservation_conteudo(int year, int month, int day, GArray *arr_ptr) {
    Date_value value_year = NULL;
    Date_value value_month = NULL;
    Date_value value_day = NULL;
    for (int i = 0; i < (int)arr_ptr->len; i++) {
        value_year = g_array_index(arr_ptr, Date_value, i);
        if (value_year->date_value == year) {
            value_year->info.reservations++;
            for (int j = 0; j < (int)value_year->lower_array->len; j++) {
                value_month = g_array_index(value_year->lower_array, Date_value, j);
                if (value_month->date_value == month) {
                    value_month->info.reservations++;
                    for (int k = 0; k < (int)value_month->lower_array->len; k++) {
                        value_day = g_array_index(value_month->lower_array, Date_value, k);
                        if (value_day->date_value == day) {
                            value_day->info.reservations++;
                            return;
                        }
                    }
                }
            }
        }
    }
}

void increment_passangers_conteudo(int year, int month, int day, GArray *arr_ptr) {
    Date_value value_year = NULL;
    Date_value value_month = NULL;
    Date_value value_day = NULL;
    for (int i = 0; i < (int)arr_ptr->len; i++) {
        value_year = g_array_index(arr_ptr, Date_value, i);
        if (value_year->date_value == year) {
            value_year->info.passengers++;
            for (int j = 0; j < (int)value_year->lower_array->len; j++) {
                value_month = g_array_index(value_year->lower_array, Date_value, j);
                if (value_month->date_value == month) {
                    value_month->info.passengers++;
                    for (int k = 0; k < (int)value_month->lower_array->len; k++) {
                        value_day = g_array_index(value_month->lower_array, Date_value, k);
                        if (value_day->date_value == day) {
                            value_day->info.passengers++;
                            return;
                        }
                    }
                }
            }
        }
    }
}

void increment_unique_passenger_by_year(int year, GArray *arr_ptr) {
    Date_value value_year = NULL;
    for (int i = 0; i < (int)arr_ptr->len; i++) {
        value_year = g_array_index(arr_ptr, Date_value, i);
        if (value_year->date_value == year) {
            value_year->info.unique_passengers++;
            break;
        }
    }
}

void increment_unique_passenger_by_month(int year, int month, GArray *arr_ptr) {
    Date_value value_year = NULL;
    Date_value value_month = NULL;
    for (int i = 0; i < (int)arr_ptr->len; i++) {
        value_year = g_array_index(arr_ptr, Date_value, i);
        if (value_year->date_value == year) {
            for (int j = 0; j < (int)value_year->lower_array->len; j++) {
                value_month = g_array_index(value_year->lower_array, Date_value, j);
                if (value_month->date_value == month) {
                    value_month->info.unique_passengers++;
                    break;
                }
            }
        }
    }
}

void increment_unique_passenger_by_day(int year, int month, int day, GArray *arr_ptr) {
    Date_value value_year = NULL;
    Date_value value_month = NULL;
    Date_value value_day = NULL;
    for (int i = 0; i < (int)arr_ptr->len; i++) {
        value_year = g_array_index(arr_ptr, Date_value, i);
        if (value_year->date_value == year) {
            for (int j = 0; j < (int)value_year->lower_array->len; j++) {
                value_month = g_array_index(value_year->lower_array, Date_value, j);
                if (value_month->date_value == month) {
                    for (int k = 0; k < (int)value_month->lower_array->len; k++) {
                        value_day = g_array_index(value_month->lower_array, Date_value, k);
                        if (value_day->date_value == day) {
                            value_day->info.unique_passengers++;
                            return;
                        }
                    }
                }
            }
        }
    }
}

// Function to print the CONTEUDO structure
void printConteudo(Conteudo c) {
    printf("CONTEUDO:\n");
    printf("  Users: %d\n", c->users);
    printf("  Flights: %d\n", c->flights);
    printf("  Passengers: %d\n", c->passengers);
    // printf("  Unique Passengers: %d\n", c->unique_passengers);
    printf("  Reservations: %d\n", c->reservations);
}

// Function to print the DATE_VALUE structure
void printDateValue(Date_value dv) {
    printf("DATE_VALUE:\n");
    printf("  Date Value: %d\n", dv->date_value);
    printf("  Info:\n");
    printConteudo(&(dv->info));
    printf("  Unique passengers: %d\n", g_hash_table_size(dv->unique_passengers));
    // Add printing for unique_passengers and lower_array if needed
}

void print_year_info(GArray *arr_ptr) {
    for (int i = 0; i < (int)arr_ptr->len; i++) {
        Date_value value_year = g_array_index(arr_ptr, Date_value, i);
        printDateValue(value_year);
    }
}