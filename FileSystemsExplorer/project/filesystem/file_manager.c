#include "file_manager.h"
#include "directory_ops.h"
#include "file_ops.h"
#include "../../include/utils.h"
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

// Main file manager implementation - delegates to specific modules

int list_files(FileInfo files[], int *count) {
    return list_directory_contents(files, count);
}

int change_directory(const char *path, GameState_t *game_state) {
    return change_directory(path, game_state);
}

int open_file(const char *filename, GameState_t *game_state) {
    return open_file(filename, game_state);
}

int is_treasure(const char *filepath) {
    return is_treasure(filepath);
}

int is_trap(const char *filepath) {
    return is_trap(filepath);
}

void decode_treasure(char *content, int length, char key) {
    decode_treasure(content, length, key);
}

FileType get_file_type(const char *filepath) {
    return get_file_type(filepath);
}