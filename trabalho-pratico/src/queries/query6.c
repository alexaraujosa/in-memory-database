#include "queries/query6.h"

/*
6 <year> <N>
nome;passageiros
*/
gint sort_Q6(gconstpointer a, gconstpointer b){
    Q_info6 value1 = (Q_info6)a;
    Q_info6 value2 = (Q_info6)b;

    if(value1->passangers > value2->passangers) return -1;
    if(value1->passangers < value2->passangers) return 1;
    return(strcmp(value1->origin, value2->origin));
}

/*VER onde encaixa isto*/
void initialize_aeroport_passengers(GHashTable *original) {

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, original);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        int *value = g_hash_table_lookup(original, key);
        *value = 0;
    }
}

void query6(char flag, int argc, char** argv, void** catalogues, FILE* output_file, QueryWriter writer) {
    //IGNORE_ARG(flag);
    IGNORE_ARG(argc);
    //IGNORE_ARG(argv);
    //IGNORE_ARG(catalogues);
    //IGNORE_ARG(output_file);

    Q_INFO6 information;
    GArray* arr_temp = g_array_new(FALSE, FALSE, sizeof(Q_INFO6));

    GHashTableIter iter;
    gpointer key, value;

    Stats_info stats = catalogues[4];
    int year = atoi(argv[0]);

    initialize_aeroport_passengers(stats->aeroports);

    calculate_aeroport_n_passengers(stats->aeroports, catalogues[1], &year);
    
    g_hash_table_iter_init(&iter, stats->aeroports);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        strcpy(information.origin, key);
        //int *value = g_hash_table_lookup(stats->aeroports, key);
        information.passangers = *(int*)value;
        g_array_append_val(arr_temp, information);
    }
    g_array_sort(arr_temp, (GCompareFunc) sort_Q6);

    for(int i = 0; i < (int)arr_temp->len && i < (atoi(argv[1])); i++){
        information = g_array_index(arr_temp, Q_INFO6, i);
        writer(6, flag, &information, output_file, i+1);
    }

    g_array_free(arr_temp, TRUE);
}

char* query6_write(void* query_info) {
    char* output_buffer = NULL;

    Q_info6 q_info = (Q_info6)query_info;
    output_buffer = isnprintf(
        "name: %s;passengers: %d",
        q_info->origin,
        q_info->passangers
    );
    
    return output_buffer;
}
