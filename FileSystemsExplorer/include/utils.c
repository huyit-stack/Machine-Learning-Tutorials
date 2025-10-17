#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>

void display_error(const char *message) {
    printf("✗ %s\n", message);
}

void display_success(const char *message) {
    printf("✓ %s\n", message);
}

void display_warning(const char *message) {
    printf("⚠ %s\n", message);
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
    safe_strcpy(buffer, perm_str, buffer_size);
}

int is_hidden_file(const char *filename) {
    return filename[0] == '.';
}

void clear_screen(void) {
    system("clear");
}

void refresh_display(void) {
    fflush(stdout);
}

char* format_time(time_t time_val) {
    static char time_str[32];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", localtime(&time_val));
    return time_str;
}

time_t get_current_time(void) {
    return time(NULL);
}

int safe_strcpy(char *dest, const char *src, size_t dest_size) {
    if (dest == NULL || src == NULL || dest_size == 0) {
        return ERROR_INVALID_INPUT;
    }
    
    strncpy(dest, src, dest_size - 1);
    dest[dest_size - 1] = '\0';
    return SUCCESS;
}

int safe_strcat(char *dest, const char *src, size_t dest_size) {
    if (dest == NULL || src == NULL || dest_size == 0) {
        return ERROR_INVALID_INPUT;
    }
    
    size_t dest_len = strlen(dest);
    if (dest_len >= dest_size) {
        dest_len = dest_size - 1;
    }
    if (dest_len >= dest_size - 1) {
        return ERROR_LIMIT_EXCEEDED;
    }
    
    strncat(dest, src, dest_size - dest_len - 1);
    return SUCCESS;
}

void trim_string(char *str) {
    if (str == NULL) return;
    
    char *start = str;
    char *end = str + strlen(str) - 1;
    
    // Trim leading whitespace
    while (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r') {
        start++;
    }
    
    // Trim trailing whitespace
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        end--;
    }
    
    // Move trimmed string to beginning
    if (start != str) {
        memmove(str, start, end - start + 1);
    }
    
    str[end - start + 1] = '\0';
}

int is_valid_filename(const char *filename) {
    if (filename == NULL || strlen(filename) == 0) {
        return 0;
    }
    
    // Check for invalid characters
    const char *invalid_chars = "/\\:*?\"<>|";
    for (int i = 0; invalid_chars[i] != '\0'; i++) {
        if (strchr(filename, invalid_chars[i]) != NULL) {
            return 0;
        }
    }
    
    return 1;
}

int is_valid_path(const char *path) {
    if (path == NULL || strlen(path) == 0) {
        return 0;
    }
    
    // Basic path validation
    if (strlen(path) >= MAX_PATH_LEN) {
        return 0;
    }
    
    return 1;
}

int is_admin_user(void) {
    return getuid() == 0;
}

void* safe_malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    void *ptr = malloc(size);
    if (ptr == NULL) {
        display_error("Lỗi cấp phát bộ nhớ");
    }
    
    return ptr;
}

void safe_free(void *ptr) {
    if (ptr != NULL) {
        free(ptr);
    }
}