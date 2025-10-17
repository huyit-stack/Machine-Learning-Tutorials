#ifndef HACK_SYSTEM_H
#define HACK_SYSTEM_H

#include "../../include/game_structs.h"
#include "../../include/constants.h"
#include "../../include/error_codes.h"

// Hack system function declarations

/**
 * Hack file permissions (Admin only)
 * @param filepath Path to file to hack
 * @param new_permissions New permissions to set
 * @param game_state Pointer to game state
 * @return 0 on success, -1 on error
 */
int hack_file_permissions(const char *filepath, mode_t new_permissions, GameState_t *game_state);

/**
 * Check if user has admin privileges
 * @param game_state Pointer to game state
 * @return 1 if admin, 0 otherwise
 */
int is_admin_user(const GameState_t *game_state);

/**
 * Update hack count in game state
 * @param game_state Pointer to game state
 * @return 0 on success, -1 on error
 */
int update_hack_count(GameState_t *game_state);

/**
 * Check if hack limit exceeded
 * @param game_state Pointer to game state
 * @return 1 if limit exceeded, 0 otherwise
 */
int is_hack_limit_exceeded(const GameState_t *game_state);

/**
 * Reset hack count for new level
 * @param game_state Pointer to game state
 */
void reset_hack_count(GameState_t *game_state);

/**
 * Get remaining hack attempts
 * @param game_state Pointer to game state
 * @return Number of remaining hack attempts
 */
int get_remaining_hack_attempts(const GameState_t *game_state);

#endif // HACK_SYSTEM_H