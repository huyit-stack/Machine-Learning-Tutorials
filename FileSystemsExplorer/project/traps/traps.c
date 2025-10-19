#include <string.h>
#include "traps.h"

ErrorCode traps_handle(GameState* state, const char* path, bool* triggered) {
    if (!state || !path) return EC_INVALID;
    bool isTrap = strstr(path, "trap") != NULL;
    if (triggered) *triggered = isTrap;
    if (isTrap) {
        state->lives -= 1;
        state->trapsTriggered += 1;
    }
    return EC_OK;
}
