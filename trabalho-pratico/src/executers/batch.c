#include "executers/batch.h"
#include "executers/datasets.h"
#include "tests/test.h"

#ifdef MAKE_TEST
// Refer to main.c
extern double batch_test_parsing_time;
extern double batch_test_sorting_time;
extern char* batch_test_output_path;
extern FILE* batch_test_test_report;
#endif

void batch(const char* arg1, const char* arg2) {
    setlocale(LC_COLLATE, "en_US.UTF-8");
    setenv("TZ", "", 1);

    DatasetData dd = make_dataset_data(arg1);
    dataset_data_load(dd);

    void** catalogues = dataset_data_get_catalog_array(dd);

    TEST_EXPR(printf("\n----===[  QUERY EXECUTION METRICS  ]===----\n\n");)
    TEST_EXPR(fprintf(batch_test_test_report, "\n----===[  QUERY EXECUTION METRICS  ]===----\n\n");)
    // TEST_EXPR(CLOSE_FILE(batch_test_test_report);)

    // Execute the queries
    query_run_bulk((char*)arg2, "Resultados", catalogues);

#ifdef MAKE_TEST
    // char* output_path = join_paths(2, get_cwd()->str, "Resultados/test_report.txt");
    // FILE* test_report = OPEN_FILE(batch_test_output_path, "a");
    
    printf(" -> Execution time for parsing all collections datasets: %.4f seconds.\n", batch_test_parsing_time);
    fprintf(
        batch_test_test_report, 
        " -> Execution time for parsing all collections datasets: %.4f seconds.\n", 
        batch_test_parsing_time
    );
    printf(" -> Execution time for sorting all collections datasets: %.4f seconds.\n", batch_test_sorting_time);
    fprintf(
        batch_test_test_report, 
        " -> Execution time for sorting all collections datasets: %.4f seconds.\n", 
        batch_test_sorting_time
    );

    // CLOSE_FILE(batch_test_test_report);
    // free(output_path);
#endif

    destroy_dataset_data(dd);
}