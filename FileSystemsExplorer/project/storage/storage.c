#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include "storage.h"
#include "../../include/constants.h"

static void ensure_file_exists(const char* path) {
    if (access(path, F_OK) != 0) {
        FILE* f = fopen(path, "ab");
        if (f) fclose(f);
    }
}

ErrorCode storage_init(void) {
    // Ensure data files exist
    ensure_file_exists(RANKING_FILE);
    // SAVE_FILE is created on first save
    return EC_OK;
}

void storage_shutdown(void) {
}
