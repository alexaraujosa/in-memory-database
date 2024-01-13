#include "data/data.h"
#include "util/io.h"

char* get_asset_dir(char* dir) {
    #define SCOPE "get_asset_dir"
    static char* cached_dir = NULL;
    if (cached_dir != NULL) return cached_dir;

    GString* cwd = get_cwd();

    if (dir == NULL) {
        cached_dir = join_paths(2, cwd->str, "assets");
    } else {
        if (is_path_absolute(dir)) {
            cached_dir = strdup(dir);
        } else {
            cached_dir = join_paths(2, cwd->str, "assets");
        }
    }

    rt_assert_dyn(
        (path_exists(cached_dir) == PATH_EXISTS),
        isnprintf("%s%s", trace_msg(SCOPE, "Asset path does not exist: "), cached_dir),
        free
    );
    rt_assert_dyn(
        (path_type(cached_dir) == PATH_TYPE_DIR),
        isnprintf("%s%s", trace_msg(SCOPE, "Asset path is not a directory: "), cached_dir),
        free
    );

    return cached_dir;
    #undef SCOPE
}