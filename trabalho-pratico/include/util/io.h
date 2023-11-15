#ifndef TP_UTIL_IO_H
#define TP_UTIL_IO_H

#include "common.h"
#include <limits.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>
#include <dirent.h>
#include "util/error.h"
#include "debug.h"

#define FS_PATH_SEPARATOR '/'

#define OPEN_FILE(filename, mode) ({\
    FILE* stream = fopen(filename, mode);\
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
// char* join_paths(char** paths, int len);
char* join_paths(int len, ...);

/*
 * Returns a GArray containing the full path of all files within a directory.
 */
GArray* get_files(char* path);

/*
 * Returns a GArray containing the full path of all subdirectories within a directory.
 */
GArray* get_subdirs(char* path);

/*
 * Returns the Current Working Directory of this program (CWD).
 */
GString* get_cwd();

/*
 * Returns the directory part of a path. If the path is relative, it is resolved against the CWD.
 */
char* dirname_absolute(char* path);

/*
 * Resolves the path against the CWD. If the path is absolute, the path itself is return.
 */
char* resolve_to_cwd(char* path);

/*
 * Attemps to skip N lines of a given FILE.
 */
int skip_n_lines(FILE* file, int lines);

/*
 * Determines wheather a path is absolute or not.
 */
int is_path_absolute(char* path);

#endif