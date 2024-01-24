#include "queries/query9.h"

void query9(char flag, int argc, char** argv, void** catalogues, FILE* output_file, QueryWriter writer) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);

    Q_INFO9 information;

    GArray* arrTemp = g_array_new(FALSE, FALSE, sizeof(gpointer));
    guint matched_index = 0;
    gboolean exists = catalog_exists_in_array(catalogues[0], *argv, &usersCatalog_name_compare_func, &matched_index);

    // void *data1, *data2;
    char* user_name;

    if (exists) {
        int matched_index_down = matched_index;
        void* data1 = catalog_search_in_array(catalogues[0], matched_index);
        user_name = get_user_name((User)data1);

        g_array_append_val(arrTemp, data1);

        while (strncasecmp(*argv, user_name, strlen(*argv)) == 0 && matched_index_down > 0) {
            data1 = catalog_search_in_array(catalogues[0], --matched_index_down);
            g_array_append_val(arrTemp, data1);
            free(user_name);
            user_name = get_user_name((User)data1);
        };
        if (strncasecmp(*argv, user_name, strlen(*argv)) != 0) g_array_remove_index(arrTemp, arrTemp->len - 1);
        free(user_name);

        int matched_index_up = matched_index;
        void* data2 = catalog_search_in_array(catalogues[0], matched_index_up);
        user_name = get_user_name((User)data2);
        while (strncasecmp(*argv, user_name, strlen(*argv)) == 0 && (int)matched_index_up < catalog_get_item_count(catalogues[0]) - 1) {
            data2 = catalog_search_in_array(catalogues[0], ++matched_index_up);
            g_array_append_val(arrTemp, data2);

            free(user_name);
            user_name = get_user_name((User)data2);
        };
        if (strncasecmp(*argv, user_name, strlen(*argv)) != 0) g_array_remove_index(arrTemp, arrTemp->len - 1);
        free(user_name);

        g_array_sort(arrTemp, &usersCatalog_strcoll_compare_func);

        // bool activated = FALSE;
        int count = 1;
        for (int i = 0; i < (int)arrTemp->len; i++) {
            const User user_temp = (const User)(g_array_index(arrTemp, gpointer, i));

            if (get_user_account_status(user_temp) == TRUE) {
                // activated = TRUE;

                user_name = get_user_name(user_temp);
                information.userID = get_user_id(user_temp);
                information.user_name = user_name;

                writer(9, flag, &information, output_file, count);

                free(information.userID);
                free(user_name);

                count++;
            }
            // if(i == (int)arrTemp->len - 1 && activated == TRUE) fprintf(output_file, "\n");
        };
    }

    g_array_free(arrTemp, TRUE);
}

char* query9_write(void* query_info) {
    char* output_buffer = NULL;

    Q_info9 q_info = (Q_info9)query_info;
    output_buffer = isnprintf(
        "id: %s;name: %s",
        q_info->userID,
        q_info->user_name
    );
    
    return output_buffer;
}
