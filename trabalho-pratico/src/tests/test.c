#include "tests/test.h"

void tests_validation(const char* correct_output_path) {
    char* filename_results = "Resultados";
    char* path_results = resolve_to_cwd(filename_results);
    char* output_path = join_paths(2, path_results, "test_report.txt");
    FILE* test_report = OPEN_FILE(output_path, "a");

    char* line = NULL;
    char* line2 = NULL;
    size_t len = 0, len2 = 0;
    ssize_t read, read2;
    int num_files = get_files((char*)correct_output_path)->len;
    int index = 0;
    while(index < num_files) {
        char* output_file = isnprintf("command%d_output.txt", index+1);
        char* file_correct_path = join_paths(2, correct_output_path, output_file);
        char* file_query_path = join_paths(2, path_results, output_file);
        FILE* stream = OPEN_FILE(file_correct_path, "r");
        FILE* stream2 = OPEN_FILE(file_query_path, "r");

        int line_counter = 1;
        while (((read = getline(&line, &len, stream)) != -1) && ((read2 = getline(&line2, &len2, stream2)) != -1)) {
            if(strcmp(line, line2) != 0) {
                printf("\n ! Command output invalid detected. --=[ File: %s | Line Number: %d ==--\n", output_file, line_counter);
                fprintf(test_report, "\n ! Command output invalid detected. --=[ File: %s | Line Number: %d ==--\n", output_file, line_counter);
                printf(GREEN "Expected output: %s" RESET, line);
                fprintf(test_report, "Expected output: %s", line);
                printf(RED "Obtained output: %s" RESET, line2);
                fprintf(test_report, "Obtained output: %s", line2);
                break;
            }
            line_counter++;
        }

        CLOSE_FILE(stream);
        CLOSE_FILE(stream2);

        index++;
    }

    CLOSE_FILE(test_report);
}