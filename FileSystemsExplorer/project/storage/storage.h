#ifndef STORAGE_H
#define STORAGE_H

#include "../../include/game_structs.h"
#include "../../include/constants.h"
#include "../../include/error_codes.h"

// Main storage module header

/**
 * Initialize storage module
 * @return 0 on success, -1 on error
 */
int init_storage_module(void);

/**
 * Cleanup storage module
 */
void cleanup_storage_module(void);

/**
 * Process storage-related command
 * @param command Command string
 * @param game_state Pointer to game state
 * @param ranking Pointer to ranking system
 * @return 0 on success, -1 on error
 */
int process_storage_command(const char *command, GameState_t *game_state, RankingSystem *ranking);

#endif // STORAGE_H