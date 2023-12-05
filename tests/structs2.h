#ifndef TESTS_STRUCTS2
#define TESTS_STRUCTS2

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
#include <time.h>

#define BILLION  1000000000L

#define MAX_USER_ID_LEN 60

// Lenght of the largest portuguese proper noun: 
// "Manuel Maria Filipe Carlos Amélio Luís Miguel Rafael Gabriel Gonzaga Xavier "
// "Francisco de Assis Eugénio de Bragança Orleães Sabóia e Saxe-Coburgo-Gotha"
#define MAX_NAME_LEN 157

#define MAX_HOTEL_NAME_LEN 60

#define MAX_PLANE_NAME_LEN 60

#define MAX_AIRLINE_NAME_LEN 60

#define LOCATION_LEN 3

#define UserId(_id) char _id[MAX_USER_ID_LEN]

#endif