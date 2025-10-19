#ifndef VIRUS_MANAGER_H
#define VIRUS_MANAGER_H

#include <stdbool.h>
#include <sys/types.h>
#include "../../include/error_codes.h"
#include "../../include/game_structs.h"

ErrorCode virus_start(GameState* state, const char* message);
ErrorCode virus_stop(GameState* state);
bool virus_is_running(const GameState* state);

#endif // VIRUS_MANAGER_H
