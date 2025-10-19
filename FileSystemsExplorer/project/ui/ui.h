#ifndef UI_H
#define UI_H

#include "../../include/game_structs.h"
#include "../../include/error_codes.h"

ErrorCode ui_init(GameState* state, const char* playerName);
void ui_teardown(void);
void ui_render(const GameState* state);

#endif // UI_H
