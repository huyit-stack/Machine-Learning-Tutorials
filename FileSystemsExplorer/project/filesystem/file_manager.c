#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "file_manager.h"
#include "../../include/utils.h"
#include "../../include/constants.h"

ErrorCode fm_change_directory(GameState* state, const char* path) {
    if (!state || !path) return EC_INVALID;
    if (chdir(path) != 0) {
        if (errno == ENOENT) return EC_NOT_FOUND;
        if (errno == EACCES) return EC_PERMISSION;
        return EC_ERR;
    }
    char cwd[MAX_PATH_LEN];
    if (!getcwd(cwd, sizeof(cwd))) return EC_ERR;
    snprintf(state->currentPath, sizeof(state->currentPath), "%s", cwd);
    return EC_OK;
}

ErrorCode fm_get_current_directory(GameState* state, char* out, size_t out_sz) {
    if (!state || !out || out_sz == 0) return EC_INVALID;
    char cwd[MAX_PATH_LEN];
    if (!getcwd(cwd, sizeof(cwd))) return EC_ERR;
    snprintf(out, out_sz, "%s", cwd);
    snprintf(state->currentPath, sizeof(state->currentPath), "%s", cwd);
    return EC_OK;
}
