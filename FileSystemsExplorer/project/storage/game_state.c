#include <string.h>
#include <stdio.h>
#include "game_state.h"
#include "../../include/constants.h"

void game_state_init(GameState* state, const char* playerName) {
    if (!state) return;
    memset(state, 0, sizeof(*state));
    state->lives = DEFAULT_LIVES;
    state->score = 0;
    snprintf(state->playerName, sizeof(state->playerName), "%s", playerName ? playerName : "Player");
}

bool game_state_is_alive(const GameState* state) {
    return state && state->lives > 0;
}
