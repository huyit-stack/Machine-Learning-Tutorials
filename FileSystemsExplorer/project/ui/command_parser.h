#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "../../include/game_structs.h"
#include "../../include/constants.h"
#include "../../include/error_codes.h"

// Command parser function declarations

/**
 * Parse user command
 * @param command Command string to parse
 * @param args Array to store command arguments
 * @param max_args Maximum number of arguments
 * @return Number of arguments parsed
 */
int parse_command(const char *command, char args[][MAX_COMMAND_LEN], int max_args);

/**
 * Process filesystem commands
 * @param args Command arguments
 * @param arg_count Number of arguments
 * @param game_state Pointer to game state
 * @return 0 on success, -1 on error
 */
int process_filesystem_command(char args[][MAX_COMMAND_LEN], int arg_count, GameState_t *game_state);

// Trap commands are handled by traps module

/**
 * Process UI commands
 * @param args Command arguments
 * @param arg_count Number of arguments
 * @param game_state Pointer to game state
 * @return 0 on success, -1 on error
 */
int process_ui_command(char args[][MAX_COMMAND_LEN], int arg_count, GameState_t *game_state);

/**
 * Validate command
 * @param command Command string
 * @return 1 if valid, 0 otherwise
 */
int is_valid_command(const char *command);

/**
 * Get command help
 * @param command Command string
 * @return Help string, NULL if not found
 */
const char* get_command_help(const char *command);

/**
 * List all available commands
 */
void list_commands(void);

#endif // COMMAND_PARSER_H