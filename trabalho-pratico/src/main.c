#include <stdio.h>
#include <time.h>
#include "executers/batch.h"

#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"

int main(int argc, char const *argv[]) {
    clock_t start_time = clock();
    if(argc > 2) {
        batch(argv[1], argv[2]);
    } else {
        printf("Interactive mode not implemented.\n");
        exit(EXIT_FAILURE);
    }
    
    clock_t end_time = clock();

    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf(RED "\n\nExecution Time: %f seconds\n" RESET, elapsed_time);

    return 0;
}
