#define _POSIX_C_SOURCE 200809L
#include "trap_manager.h"
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <stdarg.h>
#include <unistd.h>

// Function prototypes
void display_error(const char *message);
void display_success(const char *message);
void display_warning(const char *message);

// Global virus processes array
static VirusProcess virus_processes[MAX_VIRUSES];
static int virus_count = 0;

int hack_file_permissions(const char *filepath, mode_t new_permissions, GameState_t *game_state) {
    // Check if user is admin
    if (!is_admin_user(game_state)) {
        display_error("Chỉ Admin mới có thể hack file permissions");
        return -1;
    }
    
    // Check hack limit
    if (is_hack_limit_exceeded(game_state)) {
        display_error("Đã vượt quá giới hạn hack cho level này");
        return -1;
    }
    
    // Check if file exists
    struct stat file_stat;
    if (stat(filepath, &file_stat) == -1) {
        display_error("File không tồn tại");
        return -1;
    }
    
    // Attempt to change permissions
    if (chmod(filepath, new_permissions) == -1) {
        display_error("Không thể thay đổi quyền file");
        return -1;
    }
    
    // Update game state
    update_hack_count(game_state);
    game_state->score += 50;
    
    display_success(MSG_HACK_SUCCESS);
    printf("Đã hack file: %s\n", filepath);
    
    return 0;
}

int scan_viruses(VirusProcess viruses[], int *count) {
    // Check if user is admin
    if (!is_admin_user(NULL)) {
        display_error("Chỉ Admin mới có thể scan virus");
        return -1;
    }
    
    int found_count = 0;
    
    // Scan for virus processes (simplified - in real implementation would scan /proc)
    for (int i = 0; i < virus_count && found_count < MAX_VIRUSES; i++) {
        if (virus_processes[i].pid > 0) {
            // Check if process still exists
            if (kill(virus_processes[i].pid, 0) == 0) {
                viruses[found_count] = virus_processes[i];
                found_count++;
            } else {
                // Process no longer exists, remove from list
                virus_processes[i].pid = 0;
            }
        }
    }
    
    *count = found_count;
    
    if (found_count > 0) {
        char warning_msg[256];
        snprintf(warning_msg, sizeof(warning_msg), "Tìm thấy %d virus trong hệ thống", found_count);
        display_warning(warning_msg);
    } else {
        display_success("Không tìm thấy virus nào");
    }
    
    return 0;
}

int kill_virus(pid_t virus_pid, GameState_t *game_state) {
    // Check if user is admin
    if (!is_admin_user(game_state)) {
        display_error("Chỉ Admin mới có thể kill virus");
        return -1;
    }
    
    // Check if process exists
    if (kill(virus_pid, 0) == -1) {
        display_error("Virus process không tồn tại");
        return -1;
    }
    
    // Kill the virus process
    if (kill(virus_pid, SIGKILL) == -1) {
        display_error("Không thể kill virus");
        return -1;
    }
    
    // Wait for process to terminate
    int status;
    waitpid(virus_pid, &status, 0);
    
    // Update game state
    update_virus_kill_count(game_state);
    game_state->score += 200;
    
    // Remove from virus list
    for (int i = 0; i < virus_count; i++) {
        if (virus_processes[i].pid == virus_pid) {
            virus_processes[i].pid = 0;
            break;
        }
    }
    
    display_success(MSG_VIRUS_KILLED);
    printf("Đã kill virus PID: %d\n", virus_pid);
    
    return 0;
}

pid_t create_test_virus(const char *virus_name, int level) {
    pid_t pid = fork();
    
    if (pid == -1) {
        display_error("Không thể tạo virus process");
        return -1;
    }
    
    if (pid == 0) {
        // Child process (virus)
        printf("Virus %s (Level %d) đang chạy với PID: %d\n", virus_name, level, getpid());
        
        // Simulate virus behavior (create temporary files)
        char temp_file[64];
        snprintf(temp_file, sizeof(temp_file), "/tmp/virus_%d_%d.tmp", getpid(), level);
        
        FILE *fp = fopen(temp_file, "w");
        if (fp) {
            fprintf(fp, "Virus %s - Level %d\n", virus_name, level);
            fprintf(fp, "PID: %d\n", getpid());
            fprintf(fp, "Time: %ld\n", time(NULL));
            fclose(fp);
        }
        
        // Keep running until killed
        while (1) {
            sleep(1);
        }
    } else {
        // Parent process - add to virus list
        if (virus_count < MAX_VIRUSES) {
            virus_processes[virus_count].pid = pid;
            strncpy(virus_processes[virus_count].name, virus_name, 31);
            virus_processes[virus_count].name[31] = '\0';
            virus_processes[virus_count].level = level;
            virus_processes[virus_count].created_time = time(NULL);
            virus_count++;
        }
    }
    
    return pid;
}

int get_memory_info(long *total_mem, long *free_mem) {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) {
        return -1;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "MemTotal: %ld kB", total_mem) == 1) {
            *total_mem *= 1024; // Convert to bytes
        } else if (sscanf(line, "MemAvailable: %ld kB", free_mem) == 1) {
            *free_mem *= 1024; // Convert to bytes
        }
    }
    
    fclose(fp);
    return 0;
}

int calculate_virus_difficulty(int base_level) {
    long total_mem, free_mem;
    
    if (get_memory_info(&total_mem, &free_mem) == -1) {
        return base_level; // Default difficulty if can't get memory info
    }
    
    // Calculate memory usage percentage
    double memory_usage = (double)(total_mem - free_mem) / total_mem;
    
    // Adjust difficulty based on memory usage
    int adjusted_level = base_level;
    if (memory_usage > 0.8) {
        adjusted_level += 2; // Higher difficulty when memory is low
    } else if (memory_usage > 0.6) {
        adjusted_level += 1;
    }
    
    return adjusted_level;
}

int is_admin_user(const GameState_t *game_state) {
    if (game_state == NULL) {
        return getuid() == 0; // Check if running as root
    }
    return game_state->role == ROLE_ADMIN;
}

int update_hack_count(GameState_t *game_state) {
    if (game_state == NULL) {
        return -1;
    }
    
    game_state->hack_count++;
    return 0;
}

int update_virus_kill_count(GameState_t *game_state) {
    if (game_state == NULL) {
        return -1;
    }
    
    game_state->viruses_killed++;
    return 0;
}

int is_hack_limit_exceeded(const GameState_t *game_state) {
    if (game_state == NULL) {
        return 0;
    }
    
    return game_state->hack_count >= MAX_HACK_ATTEMPTS;
}

void reset_hack_count(GameState_t *game_state) {
    if (game_state != NULL) {
        game_state->hack_count = 0;
    }
}

// Helper functions are defined in file_manager.c