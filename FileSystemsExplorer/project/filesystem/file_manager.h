#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <stdbool.h>
#include "../../include/game_structs.h"
#include "../../include/error_codes.h"

ErrorCode fm_change_directory(GameState* state, const char* path);
ErrorCode fm_get_current_directory(GameState* state, char* out, size_t out_sz);

#endif // FILE_MANAGER_H
