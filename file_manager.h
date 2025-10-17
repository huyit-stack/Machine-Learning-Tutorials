#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "game_config.h"

// File information structure
typedef struct {
    char name[MAX_FILENAME_LEN];
    char path[MAX_PATH_LEN];
    off_t size;
    mode_t permissions;
    FileType type;
    int is_directory;
    int is_hidden;
} FileInfo;

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

/**
 * Format file size for display
 * @param size File size in bytes
 * @param buffer Buffer to store formatted string
 * @param buffer_size Size of buffer
 */
void format_file_size(off_t size, char *buffer, size_t buffer_size);

/**
 * Get file permissions string
 * @param permissions File permissions
 * @param buffer Buffer to store permissions string
 * @param buffer_size Size of buffer
 */
void get_permissions_string(mode_t permissions, char *buffer, size_t buffer_size);

/**
 * Check if file is hidden (starts with '.')
 * @param filename Name of file
 * @return 1 if hidden, 0 otherwise
 */
int is_hidden_file(const char *filename);

#endif // FILE_MANAGER_H