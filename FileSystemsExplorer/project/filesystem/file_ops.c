#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include "file_ops.h"
#include "../../include/constants.h"

static bool ends_with(const char* s, const char* suffix) {
    if (!s || !suffix) return false;
    size_t sl = strlen(s), tl = strlen(suffix);
    if (tl > sl) return false;
    return strncmp(s + sl - tl, suffix, tl) == 0;
}

ErrorCode fo_open_and_inspect(GameState* state, const char* path, bool* isTreasure, bool* isTrap) {
    if (!state || !path) return EC_INVALID;
    if (isTreasure) *isTreasure = false;
    if (isTrap) *isTrap = false;

    struct stat st;
    if (stat(path, &st) != 0) {
        if (errno == ENOENT) return EC_NOT_FOUND;
        if (errno == EACCES) return EC_PERMISSION;
        return EC_ERR;
    }
    if (S_ISDIR(st.st_mode)) {
        return EC_INVALID; // Not a file
    }

    FILE* f = fopen(path, "rb");
    if (!f) {
        if (errno == ENOENT) return EC_NOT_FOUND;
        if (errno == EACCES) return EC_PERMISSION;
        return EC_ERR;
    }

    unsigned char buf[FILE_PREVIEW_BYTES];
    size_t n = fread(buf, 1, sizeof(buf), f);
    (void)n;
    fclose(f);

    if (ends_with(path, ".txt")) {
        if (strstr(path, "treasure") || strstr(path, "gem")) {
            if (isTreasure) *isTreasure = true;
            state->score += TREASURE_SCORE;
            state->treasuresFound += 1;
        }
    }
    if (strstr(path, "trap")) {
        if (isTrap) *isTrap = true;
        state->lives -= TRAP_LIFE_PENALTY;
        state->trapsTriggered += 1;
    }

    return EC_OK;
}
