#ifndef TP_TESTS_H
#define TP_TESTS_H

#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"

#include "util/io.h"
#include "util/string.h"
#include "common.h"

#ifdef MAKE_TEST
    #define test_trace(format, ...) printf(format, ##__VA_ARGS__)
    #define TEST_EXPR(expr) expr;
    
    /**
     * @brief Validates the output from the queries.
     * 
     * @param correct_output_path Path to correct output files.
     * 
     * @return void.
    */
    void tests_validation(const char* correct_output_path);
#else
    #define test_trace(format, ...) ;
    #define TEST_EXPR(expr) ;
#endif

#endif