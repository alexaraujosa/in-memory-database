#include "util/date.h"

bool is_year(int date, int year) {
    time_t param = (time_t)date;
    param += TIME_T_SYSTEM;
    struct tm *temporary = localtime(&param);

    return ((temporary->tm_year+1900) == year);
}

bool is_month(int date, int month) {
    time_t param = (time_t)date;
    param += TIME_T_SYSTEM;
    struct tm *temporary = localtime(&param);

    return ((temporary->tm_mon+1) == month);
}

bool is_day(int date, int day) {
    time_t param = (time_t)date;
    param += TIME_T_SYSTEM;
    struct tm *temporary = localtime(&param);

    return (temporary->tm_mday == day);
}

int get_year(int date) {
    time_t param = (time_t)date;
    param += TIME_T_SYSTEM;
    struct tm *temporary = localtime(&param);

    return temporary->tm_year + 1900;
}

int get_month(int date) {
    time_t param = (time_t)date;
    param += TIME_T_SYSTEM;
    struct tm *temporary = localtime(&param);

    return temporary->tm_mon + 1;
}

int get_day(int date) {
    time_t param = (time_t)date;
    param += TIME_T_SYSTEM;
    struct tm *temporary = localtime(&param);

    return temporary->tm_mday;
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
        printf("ERROR! Failed to create a date.\n");
        exit(EXIT_FAILURE);
    }

    time_t date = mktime(&temp);
    time_t system = TIME_T_SYSTEM;
    int res = difftime(date, system);

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
        printf("ERROR! Failed to create a date with time.\n");
        exit(EXIT_FAILURE);
    }

    time_t date = mktime(&temp);
    time_t system = TIME_T_SYSTEM;
    int res = difftime(date, system);

    return res;
}

char* date_int_notime_to_string(int time) {
    time_t converted = (time_t)time;
    converted += TIME_T_SYSTEM;
    struct tm* temp = localtime(&converted);
    
    char* buf = (char*)malloc(11 * sizeof(char));
    strftime(buf, 11, "%Y/%m/%d", temp);

    return buf;
}

char* date_int_withtime_to_string(int time) {
    time_t converted = (time_t)time;
    converted += TIME_T_SYSTEM;
    struct tm* temp = localtime(&converted);
    
    char* buf = (char*)malloc(20 * sizeof(char));
    strftime(buf, 20, "%Y/%m/%d %T", temp);

    return buf;
}