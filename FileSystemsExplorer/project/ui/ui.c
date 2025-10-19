#include <stdio.h>
#include <string.h>
#include "ui.h"
#include "../../include/constants.h"

ErrorCode ui_init(GameState* state, const char* playerName) {
    if (!state) return EC_INVALID;
    memset(state, 0, sizeof(*state));
    state->lives = DEFAULT_LIVES;
    state->score = 0;
    snprintf(state->playerName, sizeof(state->playerName), "%s", playerName ? playerName : "Player");
    return EC_OK;
}

void ui_teardown(void) {
}

void ui_render(const GameState* state) {
    if (!state) return;
    printf("=== %s v%s ===\n", PROJECT_NAME, PROJECT_VERSION);
    printf("Player: %s | Score: %d | Lives: %d | Treasures: %d | Traps: %d\n",
           state->playerName, state->score, state->lives, state->treasuresFound, state->trapsTriggered);
    printf("CWD: %s\n", state->currentPath);
}
