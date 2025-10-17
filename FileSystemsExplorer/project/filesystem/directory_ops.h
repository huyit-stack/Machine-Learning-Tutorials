#ifndef DIRECTORY_OPS_H
#define DIRECTORY_OPS_H

#include "../../include/game_structs.h"
#include "../../include/constants.h"
#include "../../include/error_codes.h"

// Directory operations function declarations

/**
 * List files and directories in current directory
 * @param files Array to store file information
 * @param count Pointer to store number of files found
 * @return 0 on success, -1 on error
 */
int list_directory_contents(FileInfo files[], int *count);

/**
 * Change to specified directory
 * @param path Target directory path
 * @param game_state Pointer to game state
 * @return 0 on success, -1 on error
 */
int change_directory(const char *path, GameState_t *game_state);

/**
 * Get current working directory
 * @param buffer Buffer to store current directory
 * @param size Size of buffer
 * @return 0 on success, -1 on error
 */
int get_current_directory(char *buffer, size_t size);

/**
 * Check if directory exists and is accessible
 * @param path Directory path to check
 * @return 1 if accessible, 0 otherwise
 */
int is_directory_accessible(const char *path);

/**
 * Create a new directory
 * @param path Directory path to create
 * @param permissions Directory permissions
 * @return 0 on success, -1 on error
 */
int create_directory(const char *path, mode_t permissions);

/**
 * Remove a directory
 * @param path Directory path to remove
 * @return 0 on success, -1 on error
 */
int remove_directory(const char *path);

#endif // DIRECTORY_OPS_H