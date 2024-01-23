#ifndef TP_UTIL_DATE_H
#define TP_UTIL_DATE_H

// Remove warning
#define __USE_XOPEN
#define _GNU_SOURCE

#include "common.h"
#include <stdbool.h>
#include "time.h"

/* System date (1/10/2023) in time_t already with DATE_OFFSET */
#define TIME_T_SYSTEM  1792236800

/* Date Offset in time_t for avoiding the usage of long int (Support dates from 1900- until 2023+) */
#define DATE_OFFSET 96118400


/**
 * @brief Verify if the year of the date is equals to the second arg.
 * 
 * @param date Date in integer (Without system offset).
 * @param year Year.
 * 
 * @return True if it's verified, otherwise False.
*/
bool is_year(int date, int year);

/**
 * @brief Verify if the month of the date is equals to the second arg.
 * 
 * @param date Date in integer (Without system offset).
 * @param month Month.
 * 
 * @return True if it's verified, otherwise False.
*/
bool is_month(int date, int month);

/**
 * @brief Verify if the day of the date is equals to the second arg.
 * 
 * @param date Date in integer (Without system offset).
 * @param day Day.
 * 
 * @return True if it's verified, otherwise False.
*/
bool is_day(int date, int day);

/**
 * @brief Get the year of the date.
 * 
 * @param date Date in integer (Without system offset).
 * 
 * @return Year of the provided date.
*/
int get_year(int date);

/**
 * @brief Get the month of the date.
 * 
 * @param date Date in integer (Without system offset).
 * 
 * @return Month of the provided date.
*/
int get_month(int date);

/**
 * @brief Get the day of the date.
 * 
 * @param date Date in integer (Without system offset).
 * 
 * @return Day of the provided date.
*/
int get_day(int date);

/**
 * @brief Get the hour of the date.
 * 
 * @param date Date in integer (Without system offset).
 * 
 * @return hour of the provided date.
*/
int get_hour(int date);

/**
 * @brief Get the minute of the date.
 * 
 * @param date Date in integer (Without system offset).
 * 
 * @return minute of the provided date.
*/
int get_minute(int date);

/**
 * @brief Get the second of the date.
 * 
 * @param date Date in integer (Without system offset).
 * 
 * @return second of the provided date.
*/
int get_second(int date);

/**
 * @brief Get the age of the date.
 * 
 * @param date Date in integer (Without system offset).
 * 
 * @return Age of the provided date.
*/
int get_age(int date);

/**
 * @brief Offset a given year and month.
 * 
 * @param year Year of the date.
 * @param month Month of the date.
 * 
 * @return Offset date.
*/
int offset_year_month(int year, int month);


/**
 * @brief Offset a given year, month and day.
 * 
 * @param year Year of the date.
 * @param month Month of the date.
 * @param day Day of the date.
 * 
 * @return Offset date.
*/
int offset_year_month_day(int year, int month, int day);

/**
 * @brief Converts the date without time string to an integer with system offset.
 * 
 * @param date Date without time in string.
 * 
 * @return Date without time in integer with system offset.
*/
int date_string_notime_to_int(char* parameter);

/**
 * @brief Converts the date with time string to an integer with system offset.
 * 
 * @param date Date with time in string.
 * 
 * @return Date with time in integer with system offset.
*/
int date_string_withtime_to_int(char* parameter);

/**
 * @brief Converts the date without time (Without system offset) in integer to a string.
 * 
 * @param date Date without time in integer (Without system offset).
 * 
 * @return Date without time in string.
*/
char* date_int_notime_to_string(int time);

/**
 * @brief Converts the date with time (Without system offset) in integer to a string.
 * 
 * @param date Date with time in integer (Without system offset).
 * 
 * @return Date with time in string.
*/
char* date_int_withtime_to_string(int time);

#endif