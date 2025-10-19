#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "directory_ops.h"

ErrorCode dir_list(const char* path) {
    const char* target = path && *path ? path : ".";
    DIR* dir = opendir(target);
    if (!dir) {
        if (errno == ENOENT) return EC_NOT_FOUND;
        if (errno == EACCES) return EC_PERMISSION;
        return EC_ERR;
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
    return EC_OK;
}
