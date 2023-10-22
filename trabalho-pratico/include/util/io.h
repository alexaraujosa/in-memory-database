#ifndef TP_UTIL_IO_H
#define TP_UTIL_IO_H

#include "common.h"
#include <limits.h>
#include <unistd.h>
#include <libgen.h>
#include "util/error.h"
#include "debug.h"

#define FS_PATH_SEPARATOR '/'

#define OPEN_FILE(filename, mode) ({\
    FILE* stream = fopen(path, mode);\
    if (stream == NULL) {\
        fprintf(stderr, "Error opening file '%s'.\n- %s\n", filename, strerror(errno));\
        exit(EXIT_FAILURE);\
    }\
    stream;\
})

#define CLOSE_FILE(stream) ({\
    if (fclose(stream) == EOF) {\
        fprintf(stderr, "Error closing file.\n %s\n", strerror(errno));\
        exit(EXIT_FAILURE);\
    }\
})

/*
 * Concatenates an array of strings into a single filesystem path.
 */
char* join_paths(char** paths, int len);

/*
 * Returns the Current Working Directory of this program (CWD).
 */
GString* get_cwd();

/*
 * Attemps to skip N lines of a given FILE.
 */
int skip_n_lines(FILE* file, int lines);

#endif