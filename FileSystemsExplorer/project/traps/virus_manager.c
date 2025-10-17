#include "virus_manager.h"
#include "../../include/utils.h"
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

// Global virus processes array
static VirusProcess virus_processes[MAX_VIRUSES];
static int virus_count = 0;

int scan_viruses(VirusProcess viruses[], int *count) {
    // Check if user is admin
    if (!is_admin_user(NULL)) {
        display_error("Chỉ Admin mới có thể scan virus");
        return ERROR_ADMIN_REQUIRED;
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
    
    return SUCCESS;
}

int kill_virus(int virus_pid, GameState_t *game_state) {
    // Check if user is admin
    if (!is_admin_user(game_state)) {
        display_error("Chỉ Admin mới có thể kill virus");
        return ERROR_ADMIN_REQUIRED;
    }
    
    // Check if process exists
    if (kill(virus_pid, 0) == -1) {
        display_error("Virus process không tồn tại");
        return ERROR_FILE_NOT_FOUND;
    }
    
    // Kill the virus process
    if (kill(virus_pid, SIGKILL) == -1) {
        display_error("Không thể kill virus");
        return ERROR_SYSTEM_CALL;
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
    
    return SUCCESS;
}

int create_test_virus(const char *virus_name, int level) {
    int pid = fork();
    
    if (pid == -1) {
        display_error("Không thể tạo virus process");
        return ERROR_SYSTEM_CALL;
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
            safe_strcpy(virus_processes[virus_count].name, virus_name, 31);
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
        return ERROR_SYSTEM_CALL;
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
    return SUCCESS;
}

int calculate_virus_difficulty(int base_level) {
    long total_mem, free_mem;
    
    if (get_memory_info(&total_mem, &free_mem) == ERROR_SYSTEM_CALL) {
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

int update_virus_kill_count(GameState_t *game_state) {
    if (game_state == NULL) {
        return ERROR_INVALID_INPUT;
    }
    
    game_state->viruses_killed++;
    return SUCCESS;
}

int list_active_viruses(VirusProcess viruses[], int *count) {
    int active_count = 0;
    
    for (int i = 0; i < virus_count && active_count < MAX_VIRUSES; i++) {
        if (virus_processes[i].pid > 0) {
            // Check if process still exists
            if (kill(virus_processes[i].pid, 0) == 0) {
                viruses[active_count] = virus_processes[i];
                active_count++;
            } else {
                // Process no longer exists, remove from list
                virus_processes[i].pid = 0;
            }
        }
    }
    
    *count = active_count;
    return SUCCESS;
}

void cleanup_virus_processes(void) {
    for (int i = 0; i < virus_count; i++) {
        if (virus_processes[i].pid > 0) {
            kill(virus_processes[i].pid, SIGKILL);
            waitpid(virus_processes[i].pid, NULL, 0);
        }
    }
    virus_count = 0;
}