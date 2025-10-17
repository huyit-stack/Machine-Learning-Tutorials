#ifndef UI_H
#define UI_H

#include "../../include/game_structs.h"
#include "../../include/constants.h"
#include "../../include/error_codes.h"

// Main UI module header

/**
 * Initialize UI module
 * @param ui_state Pointer to UI state
 * @return 0 on success, -1 on error
 */
int init_ui(UIState *ui_state);

/**
 * Cleanup UI module
 */
void cleanup_ui(void);

/**
 * Display main menu
 * @param ui_state Pointer to UI state
 * @param game_state Pointer to game state
 */
void display_main_menu(UIState *ui_state, GameState_t *game_state);

/**
 * Display game interface
 * @param ui_state Pointer to UI state
 * @param game_state Pointer to game state
 * @param files Array of files in current directory
 * @param file_count Number of files
 */
void display_game_interface(UIState *ui_state, GameState_t *game_state, 
                           FileInfo files[], int file_count);

/**
 * Display ranking screen
 * @param ui_state Pointer to UI state
 * @param ranking Pointer to ranking system
 */
void display_ranking(UIState *ui_state, RankingSystem *ranking);

/**
 * Display game over screen
 * @param ui_state Pointer to UI state
 * @param game_state Pointer to game state
 * @param final_score Final score achieved
 */
void display_game_over(UIState *ui_state, GameState_t *game_state, int final_score);

/**
 * Get user input
 * @param ui_state Pointer to UI state
 * @return 0 on success, -1 on error
 */
int get_user_input(UIState *ui_state);

#endif // UI_H