#include "util/helpers.h"

int get_age(int date) {

    int age = 0;
    int year = date/10000;
    int month = (date/10000)%100;
    int day = date%100;

    if(YEAR - year > 0) {
        age = YEAR - year;
    } else if(YEAR - year == 0 && MONTH - month > 0) {
        age = YEAR - year - 1;
    } else if(YEAR - year == 0 && MONTH - month == 0 && DAY - day > 0) {
        age = YEAR - year - 1;
    }

    return age;
}

int get_date_offset(int date) {

    int date_offset = 0;

    //TODO: Continue.

    return date_offset;
}