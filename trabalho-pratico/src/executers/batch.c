#include "executers/batch.h"
#include "executers/datasets.h"
#include "tests/test.h"

#ifdef MAKE_TEST
// Define these global variables to share data between the DatasetData builder 
// and the batch without modifying the signatures.
double batch_test_parsing_time = 0;
double batch_test_sorting_time = 0;
char* batch_test_output_path = NULL;
FILE* batch_test_test_report = NULL;
#endif

void batch(const char* arg1, const char* arg2) {
    setlocale(LC_COLLATE, "en_US.UTF-8");
    setenv("TZ", "", 1);

    DatasetData dd = make_dataset_data(arg1);
    dataset_data_load(dd);

    void** catalogues = dataset_data_get_catalog_array(dd);

    TEST_EXPR(printf("\n----===[  QUERY EXECUTION METRICS  ]===----\n\n");)
    TEST_EXPR(fprintf(batch_test_test_report, "\n----===[  QUERY EXECUTION METRICS  ]===----\n\n");)
    TEST_EXPR(CLOSE_FILE(batch_test_test_report);)

    // Execute the queries
    query_run_bulk((char*)arg2, "Resultados", catalogues);

    TEST_EXPR(batch_test_test_report = OPEN_FILE(batch_test_output_path, "a");)
    TEST_EXPR(
        printf(" -> Execution time for parsing all collections datasets: %.4f seconds.\n", batch_test_parsing_time);
    )
    TEST_EXPR(
        fprintf(
            batch_test_test_report, 
            " -> Execution time for parsing all collections datasets: %.4f seconds.\n", 
            batch_test_parsing_time
        );
    )
    TEST_EXPR(printf(" -> Execution time for sorting all collections datasets: %.4f seconds.\n", batch_test_sorting_time);)
    TEST_EXPR(
        fprintf(
            batch_test_test_report, 
            " -> Execution time for sorting all collections datasets: %.4f seconds.\n", 
            batch_test_sorting_time
        );
    )
    TEST_EXPR(CLOSE_FILE(batch_test_test_report);)

    destroy_dataset_data(dd);
}