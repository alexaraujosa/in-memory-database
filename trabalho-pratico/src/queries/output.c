#include "queries/output.h"

/*interna*/
char* build_query_buffer(int query, void* query_info) {
    char* output_buffer = NULL;

    switch (query) {
        case 1:  { output_buffer = query1_write(query_info); break; }
        case 2:  { output_buffer = query2_write(query_info); break; }
        case 3:  { output_buffer = query3_write(query_info); break; }
        case 4:  { output_buffer = query4_write(query_info); break; }
        case 5:  { output_buffer = query5_write(query_info); break; }
        case 6:  { output_buffer = query6_write(query_info); break; }
        case 7:  { output_buffer = query7_write(query_info); break; }
        case 8:  { output_buffer = query8_write(query_info); break; }
        case 9:  { output_buffer = query9_write(query_info); break; }
        case 10: { output_buffer = query10_write(query_info); break; }
    }

    return output_buffer;
}

char* _extractValueInPlace(char* keyValuePair) {
    char* colonPosition = strchr(keyValuePair, ':');

    if (colonPosition != NULL) {
        return colonPosition + 2;
    } else {
        return keyValuePair;
    }
}

/*interna*/
char* format_query_output(char flag, char* line, int n_element) {
    int line_len = strlen(line);
    Tokens tokens = tokenize_csv(line, line_len);
    char* output = NULL;

    char** parameter = tokens->data;
    int i = 0;

    if (flag == 'F') {
        int total_len = line_len + (n_element > 1) + tokens->len + 1;
        total_len += snprintf(NULL, 0, "--- %d ---\n", n_element);

        int ind = (n_element > 1);

        output = (char*)malloc(total_len * sizeof(char));
        memset(output, 0, total_len);

        if (n_element > 1) strcpy(output, "\n");
        ind += sprintf(output + ind, "--- %d ---\n", n_element);

        while (i < tokens->len) {
            ind += sprintf(output + ind, "%s\n", parameter[i]);
            i++;
        }
    } else {
        GString* _out = g_string_new(NULL);

        while (i < tokens->len) {
            char* newPointer = _extractValueInPlace(parameter[i]);

            g_string_append(_out, newPointer);
            i++;

            if (i < tokens->len) g_string_append(_out, ";");
        }

        g_string_append(_out, "\n");
        output = g_string_free(_out, FALSE);
    }

    destroy_tokens(tokens);

    return output;
}

void output_query_to_file(int query, char flag, void* query_info, FILE* output_file, int n_element) {
    char* output_buffer = build_query_buffer(query, query_info);
    char* output = format_query_output(flag, output_buffer, n_element);

    fprintf(output_file, "%s", output);

    free(output);
    free(output_buffer);
}

char* output_query_to_str(int query, char flag, void* query_info, FILE* output_file, int n_element) {
    char* output_buffer = build_query_buffer(query, query_info);
    char* output = format_query_output(flag, output_buffer, n_element);

    free(output_buffer);
    return output;
}