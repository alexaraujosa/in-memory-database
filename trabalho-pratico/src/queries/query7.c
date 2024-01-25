#include "queries/query7.h"

#define ERROR(code) {\
    *error = strdup(code);\
    return 1;\
}

int query7_verify(Query query, void* catalogues, char** error) {
    if (query->argc != 1) ERROR(LOCALE_QUERIES_ARGC1);

    if (!is_integer_positive(query->argv[0])) ERROR(LOCALE_QUERIES_QUERY6_N_NAN);

    *error = NULL;
    return 0;
}

void query7(char flag, int argc, char** argv, void** catalogues, FILE* output_file, QueryWriter writer) {
    IGNORE_ARG(argc);

    Q_INFO7 information;
    Stats_info stats = catalogues[4];

    for(int i = 0; i < atoi(argv[0]) && i < (int)stats->origin_delay->len; i++){
        Origin_delay info = g_array_index(stats->origin_delay, Origin_delay, i);
        information.origin = info->origin;
        information.median = info->delay;

        writer(7, flag, &information, output_file, i + 1);
    }
}

char* query7_write(void* query_info) {
    char* output_buffer = NULL;

    Q_info7 q_info = (Q_info7)query_info;
    output_buffer = isnprintf(
        "name: %s;median: %d",
        q_info->origin,
        q_info->median
    );

    return output_buffer;
}
