#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "../../include/game_structs.h"
#include "../../include/constants.h"
#include "../../include/error_codes.h"

// Function declarations for Module Filesystem

/**
 * List files and directories in current directory
 * @param files Array to store file information
 * @param count Pointer to store number of files found
 * @return 0 on success, -1 on error
 */
int list_files(FileInfo files[], int *count);

/**
 * Change directory
 * @param path Target directory path
 * @param game_state Pointer to game state
 * @return 0 on success, -1 on error
 */
int change_directory(const char *path, GameState_t *game_state);

/**
 * Open and read file content
 * @param filename Name of file to open
 * @param game_state Pointer to game state
 * @return 0 on success, -1 on error
 */
int open_file(const char *filename, GameState_t *game_state);

/**
 * Check if file is a treasure (contains special marker)
 * @param filepath Path to file
 * @return 1 if treasure, 0 otherwise
 */
int is_treasure(const char *filepath);

/**
 * Check if file is a trap (contains trap marker)
 * @param filepath Path to file
 * @return 1 if trap, 0 otherwise
 */
int is_trap(const char *filepath);

/**
 * Decode treasure content using XOR
 * @param content Content to decode
 * @param length Length of content
 * @param key XOR key
 */
void decode_treasure(char *content, int length, char key);

/**
 * Get file type based on content and name
 * @param filepath Path to file
 * @return FileType enum value
 */
FileType get_file_type(const char *filepath);

#endif // FILE_MANAGER_H