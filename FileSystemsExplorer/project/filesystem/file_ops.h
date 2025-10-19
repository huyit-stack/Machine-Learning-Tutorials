#ifndef FILE_OPS_H
#define FILE_OPS_H

#include <stdbool.h>
#include "../../include/error_codes.h"
#include "../../include/game_structs.h"

ErrorCode fo_open_and_inspect(GameState* state, const char* path, bool* isTreasure, bool* isTrap);

#endif // FILE_OPS_H
