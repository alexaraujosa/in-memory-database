#include <stdio.h>
#include <time.h>
#include "executers/batch.h"
#include "tests/test.h"
#include <sys/resource.h>

#ifdef MAKE_TEST

    int main(int argc, char const *argv[]) {
        if(argc < 4) {
            printf("For testing the program it's needed 3 inputs. Fuck off and come back later. <datasets> <input> <outputs>\n");
            exit(EXIT_FAILURE);
        } else {
            setlocale(LC_COLLATE, "en_US.UTF-8");
            setenv("TZ", "", 1);
            clock_t start_time = clock();
            
            batch(argv[1], argv[2]);

            clock_t end_time = clock();
            double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
            char* output_path = join_paths(2, get_cwd()->str, "Resultados/test_report.txt");
            FILE* test_report = OPEN_FILE(output_path, "a");
            printf(" -> Execution time of the program: %.4f seconds.\n", elapsed_time);
            fprintf(test_report, " -> Execution time of the program: %.4f seconds.\n", elapsed_time);
            struct rusage r_usage;
            getrusage(RUSAGE_SELF, &r_usage);
            printf(" -> Memory usage of the program: %ld MB.\n", r_usage.ru_maxrss/1000);
            fprintf(test_report, " -> Memory usage of the program: %ld MB.\n", r_usage.ru_maxrss/1000);
            printf("\n----===[  OUTPUT VALIDATION  ]===----\n\n");
            fprintf(test_report, "\n----===[  OUTPUT VALIDATION  ]===----\n\n");
            CLOSE_FILE(test_report);
            tests_validation(argv[3]);

            g_free(get_cwd()->str);
            g_free(get_cwd());
        }
        
        return 0;
    }

#else

    int main(int argc, char const *argv[]) {
        setlocale(LC_COLLATE, "en_US.UTF-8");
        setenv("TZ", "", 1);
        if(argc > 2) {
            batch(argv[1], argv[2]);
        } else {
            printf("Interactive mode not implemented.\n");
            exit(EXIT_FAILURE);
        }
        g_free(get_cwd()->str);
        g_free(get_cwd());
        return 0;
    }

#endif