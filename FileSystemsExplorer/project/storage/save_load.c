#include <stdio.h>
#include <string.h>
#include "save_load.h"
#include "../../include/constants.h"

ErrorCode save_game(const GameState* state) {
    if (!state) return EC_INVALID;
    FILE* f = fopen(SAVE_FILE, "wb");
    if (!f) return EC_ERR;
    size_t n = fwrite(state, sizeof(*state), 1, f);
    fclose(f);
    return n == 1 ? EC_OK : EC_ERR;
}

ErrorCode load_game(GameState* state) {
    if (!state) return EC_INVALID;
    FILE* f = fopen(SAVE_FILE, "rb");
    if (!f) return EC_NOT_FOUND;
    size_t n = fread(state, sizeof(*state), 1, f);
    fclose(f);
    return n == 1 ? EC_OK : EC_ERR;
}
