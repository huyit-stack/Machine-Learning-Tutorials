#include "directory_ops.h"
#include "../../include/utils.h"
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

int list_directory_contents(FileInfo files[], int *count) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char full_path[MAX_PATH_LEN];
    int file_index = 0;
    
    // Open current directory
    dir = opendir(".");
    if (dir == NULL) {
        display_error("Không thể mở thư mục hiện tại");
        return ERROR_SYSTEM_CALL;
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
        safe_strcpy(files[file_index].name, entry->d_name, MAX_FILENAME_LEN);
        safe_strcpy(files[file_index].path, full_path, MAX_PATH_LEN);
        
        files[file_index].size = file_stat.st_size;
        files[file_index].permissions = file_stat.st_mode;
        files[file_index].is_directory = S_ISDIR(file_stat.st_mode);
        files[file_index].is_hidden = is_hidden_file(entry->d_name);
        files[file_index].type = get_file_type(full_path);
        
        file_index++;
    }
    
    closedir(dir);
    *count = file_index;
    
    return SUCCESS;
}

int change_directory(const char *path, GameState_t *game_state) {
    struct stat path_stat;
    
    // Check if path exists
    if (stat(path, &path_stat) == -1) {
        display_error(ERR_MSG_FILE_NOT_FOUND);
        return ERROR_FILE_NOT_FOUND;
    }
    
    // Check if it's a directory
    if (!S_ISDIR(path_stat.st_mode)) {
        display_error("Đường dẫn không phải là thư mục");
        return ERROR_INVALID_INPUT;
    }
    
    // Check access permissions
    if (access(path, R_OK | X_OK) == -1) {
        display_error(ERR_MSG_PERMISSION_DENIED);
        return ERROR_PERMISSION_DENIED;
    }
    
    // Change directory
    if (chdir(path) == -1) {
        display_error("Không thể thay đổi thư mục");
        return ERROR_SYSTEM_CALL;
    }
    
    // Update current directory in game state
    if (getcwd(game_state->current_dir, sizeof(game_state->current_dir)) == NULL) {
        display_error("Không thể lấy đường dẫn thư mục hiện tại");
        return ERROR_SYSTEM_CALL;
    }
    
    display_success("Đã chuyển đến thư mục mới");
    return SUCCESS;
}

int get_current_directory(char *buffer, size_t size) {
    if (getcwd(buffer, size) == NULL) {
        return ERROR_SYSTEM_CALL;
    }
    return SUCCESS;
}

int is_directory_accessible(const char *path) {
    struct stat path_stat;
    
    if (stat(path, &path_stat) == -1) {
        return 0;
    }
    
    if (!S_ISDIR(path_stat.st_mode)) {
        return 0;
    }
    
    if (access(path, R_OK | X_OK) == -1) {
        return 0;
    }
    
    return 1;
}

int create_directory(const char *path, mode_t permissions) {
    if (mkdir(path, permissions) == -1) {
        display_error("Không thể tạo thư mục");
        return ERROR_SYSTEM_CALL;
    }
    
    display_success("Đã tạo thư mục mới");
    return SUCCESS;
}

int remove_directory(const char *path) {
    if (rmdir(path) == -1) {
        display_error("Không thể xóa thư mục");
        return ERROR_SYSTEM_CALL;
    }
    
    display_success("Đã xóa thư mục");
    return SUCCESS;
}