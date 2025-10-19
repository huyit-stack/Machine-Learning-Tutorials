#include <sys/stat.h>
#include <errno.h>
#include "hack_system.h"

ErrorCode hack_chmod(const char* path, unsigned int mode) {
    if (!path) return EC_INVALID;
    if (chmod(path, mode) != 0) {
        if (errno == ENOENT) return EC_NOT_FOUND;
        if (errno == EACCES) return EC_PERMISSION;
        return EC_ERR;
    }
    return EC_OK;
}
