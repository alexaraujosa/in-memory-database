#include <stdio.h>
#include <time.h>
#include "executers/batch.h"
#include "executers/interactive/interactive.h"
#include "data/data.h"
#include "data/locale.h"
#include "tests/test.h"
#include <sys/resource.h>

#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"

#ifdef MAKE_TEST
    // Define these global variables to share data between the DatasetData builder 
    // and the batch without modifying the signatures. And, because they're defined already,
    // at least we make use of them and use them as global variables on the rest of the program.
    double batch_test_parsing_time = 0;
    double batch_test_sorting_time = 0;
    char* batch_test_output_path = NULL;
    FILE* batch_test_test_report = NULL;

    int main(int argc, char const *argv[]) {
        batch_test_output_path = join_paths(2, get_cwd()->str, "Resultados/test_report.txt");
        batch_test_test_report = OPEN_FILE(batch_test_output_path, "w");

        if(argc < 4) {
            printf("For testing the program it's needed 3 inputs. Fuck off and come back later. <datasets> <input> <outputs>\n");
            exit(EXIT_FAILURE);
        } else {
            setlocale(LC_COLLATE, "en_US.UTF-8");
            setenv("TZ", "", 1);
            
            // Start total execution time
            clock_t start_time = clock();
            
            // Execute the batch mode
            batch(argv[1], argv[2]);

            // Calculate the execution time of the batch mode
            clock_t end_time = clock();
            double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

            // char* output_path = join_paths(2, get_cwd()->str, "Resultados/test_report.txt");
            // FILE* test_report = OPEN_FILE(output_path, "a");

            printf(" -> Execution time of the program: %.4f seconds.\n", elapsed_time);
            fprintf(batch_test_test_report, " -> Execution time of the program: %.4f seconds.\n", elapsed_time);

            // Get resource usage
            struct rusage r_usage;
            getrusage(RUSAGE_SELF, &r_usage);

            printf(" -> Memory usage of the program: %ld MB.\n", r_usage.ru_maxrss/1000);
            fprintf(batch_test_test_report, " -> Memory usage of the program: %ld MB.\n", r_usage.ru_maxrss/1000);

            printf("\n----===[  OUTPUT VALIDATION  ]===----\n\n");
            fprintf(batch_test_test_report, "\n----===[  OUTPUT VALIDATION  ]===----\n\n");
            // CLOSE_FILE(test_report);

            // Run the test suite - validation
            tests_validation(argv[3]);

            // Cleanup get_cwd
            g_free(get_cwd()->str);
            g_free(get_cwd());

            // Test Cleanup
            CLOSE_FILE(batch_test_test_report);
            free(batch_test_output_path);
        }
        
        return 0;
    }
#else
    int main(int argc, char const *argv[]) {
        clock_t start_time = clock();

        // Initialize asset path, further calls should use NULL.
        char* asset_path = get_asset_dir("assets");
        DataLocales locales = load_locales();

        if(argc > 2) {
            batch(argv[1], argv[2]);
        } else {
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
#endif