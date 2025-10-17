#ifndef FILE_OPS_H
#define FILE_OPS_H

#include "../../include/game_structs.h"
#include "../../include/constants.h"
#include "../../include/error_codes.h"

// File operations function declarations

/**
 * Open and read file content
 * @param filename Name of file to open
 * @param game_state Pointer to game state
 * @return 0 on success, -1 on error
 */
int open_file(const char *filename, GameState_t *game_state);

/**
 * Read file content into buffer
 * @param filepath Path to file
 * @param buffer Buffer to store content
 * @param buffer_size Size of buffer
 * @return Number of bytes read, -1 on error
 */
ssize_t read_file_content(const char *filepath, char *buffer, size_t buffer_size);

/**
 * Write content to file
 * @param filepath Path to file
 * @param content Content to write
 * @param content_size Size of content
 * @return 0 on success, -1 on error
 */
int write_file_content(const char *filepath, const char *content, size_t content_size);

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
 * Get file information
 * @param filepath Path to file
 * @param file_info Pointer to store file information
 * @return 0 on success, -1 on error
 */
int get_file_info(const char *filepath, FileInfo *file_info);

/**
 * Check if file exists
 * @param filepath Path to file
 * @return 1 if exists, 0 otherwise
 */
int file_exists(const char *filepath);

/**
 * Get file size
 * @param filepath Path to file
 * @return File size in bytes, -1 on error
 */
off_t get_file_size(const char *filepath);

#endif // FILE_OPS_H