#include "util/io.h"
#include "stdint.h"

int is_path_absolute(char* path) {
    return *path == '/';
}

// char* join_paths(char** paths, int len) {
char* join_paths(int len, ...) {
    va_list args;
    va_start(args, len);

    char* parts[len];
    int totalLen = len;
    for (int i = 0; i < len; i++) {
        char* part = va_arg(args, char*);
        parts[i] = part;
        totalLen += strlen(part);
    }

    // Some fuckery to shut GCC (https://gcc.gnu.org/bugzilla//show_bug.cgi?id=85783)
    size_t totalLenBits = totalLen * sizeof(char); 
    if (totalLenBits >= PTRDIFF_MAX) return NULL;

    char* fullPath = (char*)malloc(totalLenBits);
    int offset = 0;
    for (int i = 0; i < len; i++) {
        int partLen = strlen(parts[i]);

        memcpy(fullPath + offset, parts[i], partLen);
        offset += partLen + 1;
        if (i < len - 1) fullPath[offset - 1] = FS_PATH_SEPARATOR;
    }

    if (fullPath[totalLen - 1] != '\0') fullPath[totalLen - 1] = '\0';

    return fullPath;
}

GArray* get_files(char* path) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) return NULL;

    int is_dir = S_ISDIR(path_stat.st_mode);
    if (!is_dir) return NULL;

    GArray* paths = g_array_new(FALSE, FALSE, sizeof(char*));

    DIR* dirptr = opendir(path);
    struct dirent* dp;

    while ((dp = readdir(dirptr)) != NULL) {
        if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;
        if (dp->d_type != DT_REG) continue;

        char* full_path = join_paths(2, get_cwd()->str, dp->d_name);
        g_array_append_vals(paths, &full_path, 1);
    }
    closedir(dirptr);

    return paths;
}

GArray* get_subdirs(char* path) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) return NULL;

    int is_dir = S_ISDIR(path_stat.st_mode);
    if (!is_dir) return NULL;

    GArray* paths = g_array_new(FALSE, FALSE, sizeof(char*));

    DIR* dirptr = opendir(path);
    struct dirent* dp;

    while ((dp = readdir(dirptr)) != NULL) {
        struct stat st;
        if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;
        if (dp->d_type == DT_REG) continue;

        if (fstatat(dirfd(dirptr), dp->d_name, &st, 0) < 0) {
            perror(dp->d_name);
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            char* full_path = join_paths(2, get_cwd()->str, dp->d_name);
            g_array_append_vals(paths, &full_path, 1);
        }
    }
    closedir(dirptr);

    return paths;
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

char* dirname_absolute(char* path) {
    char* dpath;

    if (is_path_absolute(path)) dpath = strdup(path);
    else dpath = join_paths(2, get_cwd()->str, path);

    int len = strlen(dpath);

    for (int i = len - 1; i >= 0; i--) {
        if (dpath[i] == FS_PATH_SEPARATOR) {
            if (i == len - 1) continue;
            dpath[i] = '\0';
            break;
        }
    }

    return dpath;
}

char* resolve_to_cwd(char* path) {
    if (is_path_absolute(path)) return strdup(path);
    else return join_paths(2, get_cwd()->str, path);
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