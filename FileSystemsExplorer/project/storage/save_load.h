#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include "../../include/error_codes.h"
#include "../../include/game_structs.h"

ErrorCode save_game(const GameState* state);
ErrorCode load_game(GameState* state);

#endif // SAVE_LOAD_H
