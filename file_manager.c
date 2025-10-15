#include "file_manager.h"
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

// Function prototypes
void display_error(const char *message);
void display_success(const char *message);
void display_warning(const char *message);

// XOR key for treasure decoding
#define TREASURE_XOR_KEY 0x42

int list_files(FileInfo files[], int *count) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char full_path[MAX_PATH_LEN];
    int file_index = 0;
    
    // Open current directory
    dir = opendir(".");
    if (dir == NULL) {
        display_error("Không thể mở thư mục hiện tại");
        return -1;
    }
    
    // Read directory entries
    while ((entry = readdir(dir)) != NULL && file_index < MAX_RECORDS) {
        // Skip . and .. entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        // Build full path
        snprintf(full_path, sizeof(full_path), "./%s", entry->d_name);
        
        // Get file statistics
        if (stat(full_path, &file_stat) == -1) {
            continue; // Skip files we can't stat
        }
        
        // Fill file information
        strncpy(files[file_index].name, entry->d_name, MAX_FILENAME_LEN - 1);
        files[file_index].name[MAX_FILENAME_LEN - 1] = '\0';
        
        strncpy(files[file_index].path, full_path, MAX_PATH_LEN - 1);
        files[file_index].path[MAX_PATH_LEN - 1] = '\0';
        
        files[file_index].size = file_stat.st_size;
        files[file_index].permissions = file_stat.st_mode;
        files[file_index].is_directory = S_ISDIR(file_stat.st_mode);
        files[file_index].is_hidden = is_hidden_file(entry->d_name);
        files[file_index].type = get_file_type(full_path);
        
        file_index++;
    }
    
    closedir(dir);
    *count = file_index;
    
    return 0;
}

int change_directory(const char *path, GameState_t *game_state) {
    struct stat path_stat;
    
    // Check if path exists
    if (stat(path, &path_stat) == -1) {
        display_error("Thư mục không tồn tại");
        return -1;
    }
    
    // Check if it's a directory
    if (!S_ISDIR(path_stat.st_mode)) {
        display_error("Đường dẫn không phải là thư mục");
        return -1;
    }
    
    // Check access permissions
    if (access(path, R_OK | X_OK) == -1) {
        display_error("Không có quyền truy cập thư mục");
        return -1;
    }
    
    // Change directory
    if (chdir(path) == -1) {
        display_error("Không thể thay đổi thư mục");
        return -1;
    }
    
    // Update current directory in game state
    if (getcwd(game_state->current_dir, sizeof(game_state->current_dir)) == NULL) {
        display_error("Không thể lấy đường dẫn thư mục hiện tại");
        return -1;
    }
    
    display_success("Đã chuyển đến thư mục mới");
    return 0;
}

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
        display_error("File không tồn tại");
        return -1;
    }
    
    // Check if it's a regular file
    if (!S_ISREG(file_stat.st_mode)) {
        display_error("Không phải là file thông thường");
        return -1;
    }
    
    // Open file
    fd = open(full_path, O_RDONLY);
    if (fd == -1) {
        display_error("Không thể mở file");
        return -1;
    }
    
    // Read file content
    bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        close(fd);
        display_error("Không thể đọc file");
        return -1;
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
    
    return 0;
}

int is_treasure(const char *filepath) {
    int fd;
    char buffer[32];
    ssize_t bytes_read;
    
    fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        return 0;
    }
    
    bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        return strstr(buffer, "TREASURE") != NULL;
    }
    
    return 0;
}

int is_trap(const char *filepath) {
    int fd;
    char buffer[32];
    ssize_t bytes_read;
    
    fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        return 0;
    }
    
    bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
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

void format_file_size(off_t size, char *buffer, size_t buffer_size) {
    const char *units[] = {"B", "KB", "MB", "GB"};
    int unit_index = 0;
    double file_size = (double)size;
    
    while (file_size >= 1024.0 && unit_index < 3) {
        file_size /= 1024.0;
        unit_index++;
    }
    
    snprintf(buffer, buffer_size, "%.1f %s", file_size, units[unit_index]);
}

void get_permissions_string(mode_t permissions, char *buffer, size_t buffer_size) {
    char perm_str[10];
    int index = 0;
    
    // Owner permissions
    perm_str[index++] = (permissions & S_IRUSR) ? 'r' : '-';
    perm_str[index++] = (permissions & S_IWUSR) ? 'w' : '-';
    perm_str[index++] = (permissions & S_IXUSR) ? 'x' : '-';
    
    // Group permissions
    perm_str[index++] = (permissions & S_IRGRP) ? 'r' : '-';
    perm_str[index++] = (permissions & S_IWGRP) ? 'w' : '-';
    perm_str[index++] = (permissions & S_IXGRP) ? 'x' : '-';
    
    // Other permissions
    perm_str[index++] = (permissions & S_IROTH) ? 'r' : '-';
    perm_str[index++] = (permissions & S_IWOTH) ? 'w' : '-';
    perm_str[index++] = (permissions & S_IXOTH) ? 'x' : '-';
    
    perm_str[index] = '\0';
    strncpy(buffer, perm_str, buffer_size - 1);
    buffer[buffer_size - 1] = '\0';
}

int is_hidden_file(const char *filename) {
    return filename[0] == '.';
}

// Helper functions for UI integration
void display_error(const char *message) {
    printf("✗ %s\n", message);
}

void display_success(const char *message) {
    printf("✓ %s\n", message);
}

void display_warning(const char *message) {
    printf("⚠ %s\n", message);
}