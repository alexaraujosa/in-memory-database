/**
 * @file io.h
 * 
 * IO
 * 
 * THis module defines miscellaeous functions related to IO operations. 
 */

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

// FUCK OFF, NCURSES
#undef lines

/**
 * @brief The filesystem path separator for Unix systems. 
 */
#define FS_PATH_SEPARATOR '/'

/**
 * @brief Opens a file stream and exits the program if it failed.
 * 
 * @param filename The path of the file to open.
 * @param mode The mode to open this file on.
 * 
 * @return The file stream for the file.
 */
#define OPEN_FILE(filename, mode) ({\
    FILE* stream = fopen(filename, mode);\
    if (stream == NULL) {\
        fprintf(stderr, "Error opening file '%s'.\n- %s\n", filename, strerror(errno));\
        exit(EXIT_FAILURE);\
    }\
    stream;\
})

/**
 * @brief CLoses a file stream and exits the program if it failed.
 * 
 * @param stream The file stream to close.
 */
#define CLOSE_FILE(stream) ({\
    if (fclose(stream) == EOF) {\
        fprintf(stderr, "Error closing file.\n %s\n", strerror(errno));\
        exit(EXIT_FAILURE);\
    }\
})

/**
 * @brief Concatenates an array of strings into a single filesystem path.
 * 
 * @param len The number of path parts to join.
 * @param ... The path parts to be joined.
 * 
 * @return The joint path (not resolved).
 */
char* join_paths(int len, ...);

/**
 * @brief Returns a ::GArray containing the full path of all files within a directory.
 * 
 * @param path The absolute path to the directory.
 * 
 * @return A ::GArray containing the path of every file within the directory.
 */
GArray* get_files(char* path);

/**
 * @brief Returns a GArray containing the full path of all subdirectories within a directory.
 * 
 * @param path The absolute path to the directory.
 * 
 * @return A ::GArray containing the path of every subdirectory within the directory.
 */
GArray* get_subdirs(char* path);

/**
 * @brief Returns the Current Working Directory of this program (CWD).
 * 
 * @note This function is memoized.
 * 
 * @return The CWD of the program.
 */
GString* get_cwd();

/**
 * @brief Returns the directory part of a path. If the path is relative, it is resolved against the CWD.
 * 
 * @return The dirname of the path.
 */
char* dirname_absolute(char* path);

/**
 * @brief Resolves the path against the CWD. If the path is absolute, the path itself is returned.
 * 
 * @return The resolved path.
 */
char* resolve_to_cwd(char* path);

/**
 * @brief Attemps to skip N lines of a given FILE.
 * 
 * @return 0 if everything succeeded, 1 otherwise.
 */
int skip_n_lines(FILE* file, int str_lines);

/**
 * @brief Determines wheather a path is absolute or not.
 * 
 * @return 0 if the path is not absolute, 1 otherwise.
 */
int is_path_absolute(char* path);

typedef enum path_exists_stat {
    PATH_DOES_NOT_EXIST,
    PATH_EXISTS,
    PATH_PROTECTED,
    PATH_ERROR
} PathExistsStat;
/**
 * @brief Checkes whether a path exists in the filesystem.
 * 
 * @return PATH_EXISTS if the path exists, PATH_DOES_NOT_EXIST otherwise.
 * If an error occurs, PATH_ERROR or PATH_PROTECTED is returned.
 */
PathExistsStat path_exists(char* path);

typedef enum path_type {
    PATH_TYPE_UNKNOWN,
    PATH_TYPE_FILE,
    PATH_TYPE_DIR
} PathType;
PathType path_type(char* path);

#endif