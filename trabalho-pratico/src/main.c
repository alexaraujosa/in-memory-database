#include <stdio.h>
#include <time.h>
#include "executers/batch.h"
#include "executers/interactive/interactive.h"
#include "data/data.h"
#include "data/locale.h"

#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"

#pragma GCC push_options
#pragma GCC optimize ("O0")
int main(int argc, char const *argv[]) {
    clock_t start_time = clock();

    // Initialize asset path, further calls should use NULL.
    char* asset_path = get_asset_dir("assets");
    DataLocales locales = load_locales();

    if(argc > 2) {
        batch(argv[1], argv[2]);
    } else {
        // printf("Interactive mode not implemented.\n");
        // exit(EXIT_FAILURE);
        interactive(locales);
    }

    // Cleanup
    destroy_locales(locales);
    g_string_free(get_cwd(), TRUE);
    free(asset_path);
    
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf(RED "\n\nExecution Time: %f seconds\n" RESET, elapsed_time);

    return 0;
}
#pragma GCC pop_options
