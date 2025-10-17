#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include "../../include/game_structs.h"
#include "../../include/constants.h"
#include "../../include/error_codes.h"

// Console UI function declarations

/**
 * Display ASCII art welcome screen
 */
void display_welcome_art(void);

/**
 * Display ASCII art game over screen
 */
void display_game_over_art(void);

/**
 * Display level up animation
 * @param new_level New level reached
 */
void display_level_up(int new_level);

/**
 * Display file listing with colors
 * @param files Array of files
 * @param count Number of files
 * @param start_row Starting row for display
 * @param max_rows Maximum rows to display
 */
void display_file_list(FileInfo files[], int count, int start_row, int max_rows);

/**
 * Display game statistics
 * @param game_state Pointer to game state
 * @param row Starting row
 * @param col Starting column
 */
void display_stats(GameState_t *game_state, int row, int col);

/**
 * Display command prompt
 * @param ui_state Pointer to UI state
 * @param game_state Pointer to game state
 */
void display_prompt(UIState *ui_state, GameState_t *game_state);

/**
 * Display help information
 * @param ui_state Pointer to UI state
 */
void display_help(UIState *ui_state);

/**
 * Clear screen
 */
void clear_screen(void);

/**
 * Refresh display
 */
void refresh_display(void);

#endif // CONSOLE_UI_H