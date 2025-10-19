#ifndef TRAPS_H
#define TRAPS_H

#include <stdbool.h>
#include "../../include/error_codes.h"
#include "../../include/game_structs.h"

ErrorCode traps_handle(GameState* state, const char* path, bool* triggered);

#endif // TRAPS_H
