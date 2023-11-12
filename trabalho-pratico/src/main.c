#include <stdio.h>
#include "executers/batch.h"

int main(int argc, char const *argv[]) {

    if(argc > 2) {
        batch(argv[1], argv[2]);
    } else {
        printf("Interactive mode not implemented.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
