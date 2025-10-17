#ifndef TRAPS_H
#define TRAPS_H

#include "../../include/game_structs.h"
#include "../../include/constants.h"
#include "../../include/error_codes.h"

// Main traps module header

/**
 * Initialize traps module
 * @return 0 on success, -1 on error
 */
int init_traps_module(void);

/**
 * Cleanup traps module
 */
void cleanup_traps_module(void);

/**
 * Process trap-related command
 * @param command Command string
 * @param game_state Pointer to game state
 * @return 0 on success, -1 on error
 */
int process_trap_command(const char *command, GameState_t *game_state);

#endif // TRAPS_H