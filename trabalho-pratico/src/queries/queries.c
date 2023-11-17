#include "queries/queries.h"

// Queries: 1, 3, 4, 7, 8, 9

void query1(char flag, int argc, char** argv, Catalog** catalogues, FILE** output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    fputs("1", output_file);
}

void query2(char flag, int argc, char** argv, Catalog** catalogues, FILE** output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    fputs("2", output_file);
}

void query3(char flag, int argc, char** argv, Catalog** catalogues, FILE** output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    fputs("3", output_file);
}

void query4(char flag, int argc, char** argv, Catalog** catalogues, FILE** output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    fputs("4", output_file);
}

void query5(char flag, int argc, char** argv, Catalog** catalogues, FILE** output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    fputs("5", output_file);
}

void query6(char flag, int argc, char** argv, Catalog** catalogues, FILE** output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    fputs("6", output_file);
}

void query7(char flag, int argc, char** argv, Catalog** catalogues, FILE** output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    fputs("7", output_file);

    // Catalog* flights = catalogues[1];
    // int size = catalog_get_item_count(flights);
    // for (int i = 0; i < size; i++) {

    // }
}

void query8(char flag, int argc, char** argv, Catalog** catalogues, FILE** output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    fputs("8", output_file);
}

void query9(char flag, int argc, char** argv, Catalog** catalogues, FILE** output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);

    GArray *arrTemp = g_array_new(FALSE, FALSE, sizeof(gpointer));
    guint matched_index = 0;
    gboolean exists = catalog_exists_in_array(catalogues[0], *argv, &user_username_compare_func, &matched_index);
    void *data1, *data2;
    if (exists) {
        guint cur_index = 0;
        void *data = catalog_search_in_array(catalogues[0], matched_index);
        g_array_append_val(arrTemp,data);
        guint matched_index_down = matched_index - 1;
        guint matched_index_up = matched_index + 1;
        void *data1 = catalog_search_in_array(catalogues[0], matched_index_down--);
        void *data2 = catalog_search_in_array(catalogues[0], matched_index_up++);
        while (strncasecmp(*argv, get_user_name((User*)data1), strlen(*argv)) == 0 && matched_index_down > 0) {
            g_array_append_val(arrTemp,data1);
            data1 = catalog_search_in_array(catalogues[0], matched_index_down--);
        };
        while (strncasecmp(*argv, get_user_name((User*)data2), strlen(*argv)) == 0 && matched_index_up != catalog_get_item_count(catalogues[0])) {
            g_array_append_val(arrTemp,data2);
            data2 = catalog_search_in_array(catalogues[0], matched_index_up++);
        };
        g_array_sort(arrTemp, &users_full_compare_func);

        int count = 1;
        for (int i = 0; i < arrTemp->len; i++) {
           const User user_temp = (const User*)(g_array_index(arrTemp, gpointer, i));
           if(get_user_account_status(user_temp) == TRUE && flag == 'F'){
                fprintf(output_file, "--- %d ---\n", count);
                if(i != arrTemp->len - 1)
                    fprintf(output_file, "id: %s\nname: %s\n\n", get_user_id(user_temp), get_user_name(user_temp));
                else 
                    fprintf(output_file, "id: %s\nname: %s", get_user_id(user_temp), get_user_name(user_temp));
                count++;
           } else if(get_user_account_status(user_temp) == TRUE && flag == '\0') {
                if(i != arrTemp->len - 1)
                    fprintf(output_file, "%s;%s\n", get_user_id(user_temp), get_user_name(user_temp));
                else
                    fprintf(output_file, "%s;%s", get_user_id(user_temp), get_user_name(user_temp));
           }
        };
    }
}

void query10(char flag, int argc, char** argv, Catalog** catalogues, FILE** output_file) {
    IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    IGNORE_ARG(argv);
    fputs("10", output_file);
}