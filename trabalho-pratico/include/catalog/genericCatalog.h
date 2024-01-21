#ifndef GENERIC_CATALOG_H
#define GENERIC_CATALOG_H

#include "catalog/catalogManager.h"
#include "common.h"

typedef struct conteudo CONTEUDO, *Conteudo;

typedef struct date_value DATE_VALUE, *Date_value;

int get_date_value(Date_value info);
int get_conteudo_unique_passengers(Date_value info);

GArray *generate_genCat();

GArray *generate_genCat_days();

GArray *generate_genCat_months();

void genCat_add(int year, GArray *arr_ptr);

Conteudo conteudo_by_year(int year, GArray *arr_ptr);

Conteudo conteudo_by_month(int year, int month, GArray *arr_ptr);

Conteudo conteudo_by_day(int year, int month, int day, GArray *arr_ptr);

void add_to_hashtable(int year, int month, int day, char *userID, GArray *arr_ptr);

void print_year_info(GArray *arr_ptr);

void increment_conteudo_flights(Conteudo cont, int num);

void increment_user_conteudo(int year, int month, int day, GArray *arr_ptr);
void increment_flight_conteudo(int year, int month, int day, GArray *arr_ptr);
void increment_reservation_conteudo(int year, int month, int day, GArray *arr_ptr);
void increment_passenger_conteudo(int year, int month, int day, int n_passengers, GArray *arr_ptr);
int unique_passengers_by_year(int year, GArray *arr_ptr);
int unique_passengers_by_month(int year, int month, GArray *arr_ptr);
int unique_passengers_by_day(int year, int month, int day, GArray *arr_ptr);
int get_flights_by_year(int year, GArray* arr);
#endif