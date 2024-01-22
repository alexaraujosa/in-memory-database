#include "util/date.h"

bool is_year(int date, int year) {
    time_t param = (time_t)date;
    param -= DATE_OFFSET ;
    struct tm *temporary = localtime(&param);

    return ((temporary->tm_year+1900) == year);
}

bool is_month(int date, int month) {
    time_t param = (time_t)date;
    param -= DATE_OFFSET ;
    struct tm *temporary = localtime(&param);

    return ((temporary->tm_mon+1) == month);
}

bool is_day(int date, int day) {
    time_t param = (time_t)date;
    param -= DATE_OFFSET ;
    struct tm *temporary = localtime(&param);

    return (temporary->tm_mday == day);
}

int get_year(int date) {
    time_t param = (time_t)date;
    param -= DATE_OFFSET ;
    struct tm *temporary = localtime(&param);

    return temporary->tm_year + 1900;
}

int get_month(int date) {
    time_t param = (time_t)date;
    param -= DATE_OFFSET ;
    struct tm *temporary = localtime(&param);

    return temporary->tm_mon + 1;
}

int get_day(int date) {
    time_t param = (time_t)date;
    param -= DATE_OFFSET ;
    struct tm *temporary = localtime(&param);

    return temporary->tm_mday;
}

int get_hour(int date) {
    time_t param = (time_t)date;
    param -= DATE_OFFSET;
    struct tm *temporary = localtime(&param);

    return temporary->tm_hour;
}

int get_minute(int date) {
    time_t param = (time_t)date;
    param -= DATE_OFFSET;
    struct tm *temporary = localtime(&param);

    return temporary->tm_min;
}

int get_second(int date) {
    time_t param = (time_t)date;
    param -= DATE_OFFSET;
    struct tm *temporary = localtime(&param);

    return temporary->tm_sec;
}

int get_age(int date) {
    time_t difference = difftime(TIME_T_SYSTEM, date);
    int age = difference/(3600*24*365.25);

    return age;
}

time_t time_to_epoch(const struct tm *ltm) {
    const int mon_days [] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    long tyears, tdays, leaps, utc_hrs;
    int i;

    tyears = ltm->tm_year - 70;
    leaps = (tyears + 2) / 4;
    tdays = 0;
    for (i=0; i < ltm->tm_mon; i++) tdays += mon_days[i];

    tdays += ltm->tm_mday - 1;
    tdays = tdays + (tyears * 365) + leaps;

    utc_hrs = ltm->tm_hour;
    return (tdays * 86400) + (utc_hrs * 3600) + (ltm->tm_min * 60) + ltm->tm_sec;
}

int offset_year_month(int year, int month) {
    struct tm temp;
    temp.tm_sec = 0;
    temp.tm_min = 0;
    temp.tm_hour = 0;
    temp.tm_wday = 0;
    temp.tm_yday = 0;
    temp.tm_isdst = 0;
    temp.tm_year = year;
    temp.tm_mon = month;
    temp.tm_mday = 0;

    time_t date = time_to_epoch(&temp);
    time_t system = DATE_OFFSET ;
    int res = date+system;

    return res;
}

int offset_year_month_day(int year, int month, int day) {
        
    struct tm temp;
    temp.tm_sec = 0;
    temp.tm_min = 0;
    temp.tm_hour = 0;
    temp.tm_wday = 0;
    temp.tm_yday = 0;
    temp.tm_isdst = 0;
    temp.tm_year = year;
    temp.tm_mon = month;
    temp.tm_mday = day;

    time_t date = time_to_epoch(&temp);
    time_t system = DATE_OFFSET ;
    int res = date+system;

    return res;
}

int date_string_notime_to_int(char* parameter) {
    struct tm temp;
    temp.tm_sec = 0;
    temp.tm_min = 0;
    temp.tm_hour = 0;
    temp.tm_wday = 0;
    temp.tm_yday = 0;
    temp.tm_isdst = 0;

    if(strptime(parameter, "%Y/%m/%d", &temp) == NULL) {
        printf("ERROR1! Failed to create a date.\n");
        exit(EXIT_FAILURE);
    }

    time_t date = time_to_epoch(&temp);
    time_t system = DATE_OFFSET ;
    int res = date+system;

    return res;
}

int date_string_withtime_to_int(char* parameter) {
    struct tm temp;
    temp.tm_sec = 0;
    temp.tm_min = 0;
    temp.tm_hour = 0;
    temp.tm_wday = 0;
    temp.tm_yday = 0;
    temp.tm_isdst = 0;

    if(strptime(parameter, "%Y/%m/%d %T", &temp) == NULL) {
        printf("ERROR2! Failed to create a date with time.\n");
        exit(EXIT_FAILURE);
    }

    time_t date = time_to_epoch(&temp);
    time_t system = DATE_OFFSET ;
    int res = date + system;

    return res;
}

char* date_int_notime_to_string(int time) {
    time_t converted = (time_t)time;
    converted -= DATE_OFFSET ;
    struct tm* temp = localtime(&converted);
    
    char* buf = (char*)malloc(11 * sizeof(char));
    strftime(buf, 11, "%Y/%m/%d", temp);

    return buf;
}

char* date_int_withtime_to_string(int time) {
    time_t converted = (time_t)time;
    converted -= DATE_OFFSET ;
    struct tm* temp = localtime(&converted);
    
    char* buf = (char*)malloc(20 * sizeof(char));
    strftime(buf, 20, "%Y/%m/%d %T", temp);

    return buf;
}