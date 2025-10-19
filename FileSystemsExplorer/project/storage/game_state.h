#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdbool.h>
#include "../../include/game_structs.h"

void game_state_init(GameState* state, const char* playerName);
bool game_state_is_alive(const GameState* state);

#endif // GAME_STATE_H
