#include "file_ops.h"
#include "../../include/utils.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

int open_file(const char *filename, GameState_t *game_state) {
    int fd;
    char buffer[1024];
    ssize_t bytes_read;
    struct stat file_stat;
    char full_path[MAX_PATH_LEN];
    
    // Build full path
    snprintf(full_path, sizeof(full_path), "./%s", filename);
    
    // Check if file exists
    if (stat(full_path, &file_stat) == -1) {
        display_error(ERR_MSG_FILE_NOT_FOUND);
        return ERROR_FILE_NOT_FOUND;
    }
    
    // Check if it's a regular file
    if (!S_ISREG(file_stat.st_mode)) {
        display_error("Không phải là file thông thường");
        return ERROR_INVALID_INPUT;
    }
    
    // Open file
    fd = open(full_path, O_RDONLY);
    if (fd == -1) {
        display_error("Không thể mở file");
        return ERROR_SYSTEM_CALL;
    }
    
    // Read file content
    bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        close(fd);
        display_error("Không thể đọc file");
        return ERROR_SYSTEM_CALL;
    }
    buffer[bytes_read] = '\0';
    
    close(fd);
    
    // Check file type and update game state
    FileType file_type = get_file_type(full_path);
    
    switch (file_type) {
        case FILE_TYPE_TREASURE:
            // Decode treasure
            decode_treasure(buffer, bytes_read, TREASURE_XOR_KEY);
            game_state->treasures_found++;
            game_state->score += 100;
            display_success(MSG_TREASURE_FOUND);
            printf("Nội dung kho báu: %s\n", buffer);
            break;
            
        case FILE_TYPE_TRAP:
            game_state->traps_hit++;
            game_state->score -= 50;
            game_state->lives--;
            display_warning(MSG_TRAP_HIT);
            printf("Bẫy kích hoạt! Nội dung: %s\n", buffer);
            break;
            
        case FILE_TYPE_HIDDEN:
            printf("File ẩn: %s\n", buffer);
            break;
            
        default:
            printf("Nội dung file: %s\n", buffer);
            break;
    }
    
    return SUCCESS;
}

ssize_t read_file_content(const char *filepath, char *buffer, size_t buffer_size) {
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        return ERROR_SYSTEM_CALL;
    }
    
    ssize_t bytes_read = read(fd, buffer, buffer_size - 1);
    close(fd);
    
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
    }
    
    return bytes_read;
}

int write_file_content(const char *filepath, const char *content, size_t content_size) {
    int fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        return ERROR_SYSTEM_CALL;
    }
    
    ssize_t bytes_written = write(fd, content, content_size);
    close(fd);
    
    if (bytes_written != (ssize_t)content_size) {
        return ERROR_SYSTEM_CALL;
    }
    
    return SUCCESS;
}

int is_treasure(const char *filepath) {
    char buffer[32];
    ssize_t bytes_read = read_file_content(filepath, buffer, sizeof(buffer));
    
    if (bytes_read > 0) {
        return strstr(buffer, "TREASURE") != NULL;
    }
    
    return 0;
}

int is_trap(const char *filepath) {
    char buffer[32];
    ssize_t bytes_read = read_file_content(filepath, buffer, sizeof(buffer));
    
    if (bytes_read > 0) {
        return strstr(buffer, "TRAP") != NULL;
    }
    
    return 0;
}

void decode_treasure(char *content, int length, char key) {
    for (int i = 0; i < length; i++) {
        content[i] ^= key;
    }
}

FileType get_file_type(const char *filepath) {
    if (is_treasure(filepath)) {
        return FILE_TYPE_TREASURE;
    } else if (is_trap(filepath)) {
        return FILE_TYPE_TRAP;
    } else if (is_hidden_file(filepath)) {
        return FILE_TYPE_HIDDEN;
    } else {
        return FILE_TYPE_NORMAL;
    }
}

int get_file_info(const char *filepath, FileInfo *file_info) {
    struct stat file_stat;
    
    if (stat(filepath, &file_stat) == -1) {
        return ERROR_FILE_NOT_FOUND;
    }
    
    // Extract filename from path
    const char *filename = strrchr(filepath, '/');
    if (filename == NULL) {
        filename = filepath;
    } else {
        filename++; // Skip the '/'
    }
    
    safe_strcpy(file_info->name, filename, MAX_FILENAME_LEN);
    safe_strcpy(file_info->path, filepath, MAX_PATH_LEN);
    
    file_info->size = file_stat.st_size;
    file_info->permissions = file_stat.st_mode;
    file_info->is_directory = S_ISDIR(file_stat.st_mode);
    file_info->is_hidden = is_hidden_file(filename);
    file_info->type = get_file_type(filepath);
    
    return SUCCESS;
}

int file_exists(const char *filepath) {
    struct stat file_stat;
    return stat(filepath, &file_stat) == 0;
}

off_t get_file_size(const char *filepath) {
    struct stat file_stat;
    if (stat(filepath, &file_stat) == -1) {
        return -1;
    }
    return file_stat.st_size;
}