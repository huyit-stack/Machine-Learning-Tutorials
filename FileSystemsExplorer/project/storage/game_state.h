#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "../../include/game_structs.h"
#include "../../include/constants.h"
#include "../../include/error_codes.h"

// Game state management function declarations

/**
 * Calculate final score based on game statistics
 * @param game_state Pointer to game state
 * @param difficulty_multiplier Difficulty multiplier
 * @return Calculated final score
 */
int calculate_final_score(const GameState_t *game_state, float difficulty_multiplier);

/**
 * Update game statistics
 * @param game_state Pointer to game state
 * @param action Action performed
 * @param points Points gained/lost
 */
void update_game_stats(GameState_t *game_state, const char *action, int points);

/**
 * Initialize game state
 * @param game_state Pointer to game state
 */
void init_game_state(GameState_t *game_state);

/**
 * Reset game state for new game
 * @param game_state Pointer to game state
 */
void reset_game_state(GameState_t *game_state);

/**
 * Check if game over condition is met
 * @param game_state Pointer to game state
 * @return 1 if game over, 0 otherwise
 */
int is_game_over(const GameState_t *game_state);

/**
 * Get difficulty multiplier from system load
 * @return Difficulty multiplier
 */
float get_difficulty_multiplier(void);

/**
 * Calculate level based on score
 * @param score Current score
 * @return Current level
 */
int calculate_level(int score);

/**
 * Check if level up
 * @param game_state Pointer to game state
 * @return 1 if level up, 0 otherwise
 */
int check_level_up(GameState_t *game_state);

#endif // GAME_STATE_H