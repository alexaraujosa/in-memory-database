#include "util/io.h"

char* join_paths(char** paths, int len) {
    int totalLen = len;
    for (int i = 0; i < len; i++) totalLen += strlen(paths[i]);

    char* fullPath = (char*)malloc(totalLen * sizeof(char));
    int offset = 0;
    for (int i = 0; i < len; i++) {
        int partLen = strlen(paths[i]);

        strncpy(fullPath + offset, paths[i], partLen);
        offset += partLen + 1;
        if (i < len - 1) fullPath[offset - 1] = FS_PATH_SEPARATOR;
    }

    if (fullPath[totalLen - 1] != '\0') fullPath[totalLen - 1] = '\0';

    return fullPath;
}

// Memoized, CWD will not change during program execution, no need to recalculate it.
GString* get_cwd() {
    static GString* BIN_PATH;

    if (BIN_PATH == NULL || BIN_PATH->len == 0) {
        char BIN_PATH_TMP[PATH_MAX];

        if (!readlink("/proc/self/exe", BIN_PATH_TMP, PATH_MAX)) {
            printf("Unable to read current executable path.\n");
            exit(1);
        }

        BIN_PATH = g_string_new(dirname(BIN_PATH_TMP));
    }
    
    return BIN_PATH; 
}

int skip_n_lines(FILE* file, int lines) {
    if (file == NULL) {
        printf("No file provided.\n");
        return 1;
    }

    int c, i = 0;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            i++;
            if (i == lines) break;
        }
    }

    if (i != lines) {
        printf("Could not skip lines: Reached EOF before skipping all lines.\n");
        return 1;
    }

    return 0;
}