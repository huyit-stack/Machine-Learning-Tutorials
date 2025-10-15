#ifndef TRAP_MANAGER_H
#define TRAP_MANAGER_H

#include "game_config.h"

// Virus process structure
typedef struct {
    pid_t pid;
    char name[32];
    int level;
    time_t created_time;
} VirusProcess;

// Function declarations for Module Traps

/**
 * Hack file permissions (Admin only)
 * @param filepath Path to file to hack
 * @param new_permissions New permissions to set
 * @param game_state Pointer to game state
 * @return 0 on success, -1 on error
 */
int hack_file_permissions(const char *filepath, mode_t new_permissions, GameState_t *game_state);

/**
 * Scan for viruses in system
 * @param viruses Array to store found viruses
 * @param count Pointer to store number of viruses found
 * @return 0 on success, -1 on error
 */
int scan_viruses(VirusProcess viruses[], int *count);

/**
 * Kill a virus process
 * @param virus_pid PID of virus to kill
 * @param game_state Pointer to game state
 * @return 0 on success, -1 on error
 */
int kill_virus(pid_t virus_pid, GameState_t *game_state);

/**
 * Create a test virus process (for testing)
 * @param virus_name Name of virus
 * @param level Virus level
 * @return PID of created virus, -1 on error
 */
pid_t create_test_virus(const char *virus_name, int level);

/**
 * Get system memory info for difficulty adjustment
 * @param total_mem Pointer to store total memory
 * @param free_mem Pointer to store free memory
 * @return 0 on success, -1 on error
 */
int get_memory_info(long *total_mem, long *free_mem);

/**
 * Calculate virus difficulty based on system resources
 * @param base_level Base difficulty level
 * @return Adjusted difficulty level
 */
int calculate_virus_difficulty(int base_level);

/**
 * Check if user has admin privileges
 * @param game_state Pointer to game state
 * @return 1 if admin, 0 otherwise
 */
int is_admin_user(const GameState_t *game_state);

/**
 * Update hack count in game state
 * @param game_state Pointer to game state
 * @return 0 on success, -1 on error
 */
int update_hack_count(GameState_t *game_state);

/**
 * Update virus kill count in game state
 * @param game_state Pointer to game state
 * @return 0 on success, -1 on error
 */
int update_virus_kill_count(GameState_t *game_state);

/**
 * Check if hack limit exceeded
 * @param game_state Pointer to game state
 * @return 1 if limit exceeded, 0 otherwise
 */
int is_hack_limit_exceeded(const GameState_t *game_state);

/**
 * Reset hack count for new level
 * @param game_state Pointer to game state
 */
void reset_hack_count(GameState_t *game_state);

#endif // TRAP_MANAGER_H