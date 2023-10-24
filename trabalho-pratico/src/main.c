#include <stdio.h>

#include "./collections/user.h"
#include "./collections/reservation.h"
#include "./collections/flight.h"
#include "util/io.h"
#include "./parser/parser.h"

typedef struct test {
    char* id;
} TEST, *Test;

int ind = 0;

int testVerifier(Tokens tokens) {
    for (int i = 0; i < tokens->len; i++) printf("VERIFIER TOKENS %d: '%s'\n", i, tokens->data[i]);
    if (++ind == 3) return 0; // FYIP
    return 1;
}

void* testParser(Tokens tokens) {
    Test test = (Test)malloc(sizeof(TEST));
    test->id = strdup(tokens->data[0]);

    return test;
}

void testWriter(void* raw_data) {
    Test data = (Test)raw_data;
    printf("WRITE: %s\n", data->id);
    fflush(stdout);
}

void testDiscarder(Tokens tokens) {
    int totalLen = tokens->len - 1;
    for (int i = 0; i < tokens->len; i++) totalLen += strlen(tokens->data[i]);

    char* joint = (char*)malloc(totalLen * sizeof(char));
    memset(joint, 0, (totalLen) * sizeof(char));

    for (int i = 0; i < tokens->len; i++) {
        strcat(joint, tokens->data[i]);
        if (i != tokens->len - 1) strcat(joint, ";");
        free(tokens->data[i]);
    }

    free(tokens);

    printf("DISCARD: '%s'\n", joint);
    fflush(stdout);

    free(joint);
}

int main(int argc, char const *argv[]) {
    printf("Hello World!\n");

    RESERVATION res = makeReservation(
        1, 
        "123456789012345678901234567890123456789012345678901234567890",
        100,
        "098765432109876543210987654321098765432109876543210987654321",
        5,
        4,
        123456,
        4321,
        196,
        false,
        1
    );

    USER user = makeUser(
        "123456789012345678901234567890123456789012345678901234567890",
        "Manuel Maria Filipe Carlos Amélio Luís Miguel Rafael Gabriel Gonzaga Xavier "
            "Francisco de Assis Eugénio de Bragança Orleães Sabóia e Saxe-Coburgo-Gotha",
        false,
        "PT",
        12345,
        true
    );

    FLIGHT flight = makeFlight(
        1,
        "Fuckery Airlines",
        "Fuckonov 69420",
        "PQP",
        "CDC",
        12345,
        23456,
        34567
    );

    GString* bin = get_cwd();
    printf("BIN1: %s\n", bin->str);

    GString* bin2 = get_cwd();
    printf("BIN2: %s\n", bin2->str);

    parse("../test2.txt", &testVerifier, &testParser, &testWriter, &testDiscarder);
    
    return 0;
}
