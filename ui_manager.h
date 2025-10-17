#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "game_config.h"
#include "file_manager.h"

// UI state structure
typedef struct {
    int screen_height;
    int screen_width;
    int current_menu;
    int selected_item;
    char input_buffer[MAX_COMMAND_LEN];
    int input_pos;
} UIState;

// Function declarations for Module UI

/**
 * Initialize ncurses UI
 * @param ui_state Pointer to UI state
 * @return 0 on success, -1 on error
 */
int init_ui(UIState *ui_state);

/**
 * Cleanup ncurses UI
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
 * Parse user command
 * @param command Command string to parse
 * @param args Array to store command arguments
 * @param max_args Maximum number of arguments
 * @return Number of arguments parsed
 */
int parse_command(const char *command, char args[][MAX_COMMAND_LEN], int max_args);

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
 * Get user input
 * @param ui_state Pointer to UI state
 * @return 0 on success, -1 on error
 */
int get_user_input(UIState *ui_state);

/**
 * Display help information
 * @param ui_state Pointer to UI state
 */
void display_help(UIState *ui_state);

/**
 * Display error message
 * @param message Error message to display
 */
void display_error(const char *message);

/**
 * Display success message
 * @param message Success message to display
 */
void display_success(const char *message);

/**
 * Display warning message
 * @param message Warning message to display
 */
void display_warning(const char *message);

/**
 * Clear screen
 */
void clear_screen(void);

/**
 * Refresh display
 */
void refresh_display(void);

#endif // UI_MANAGER_H