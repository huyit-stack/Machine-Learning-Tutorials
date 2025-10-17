#ifndef VIRUS_MANAGER_H
#define VIRUS_MANAGER_H

#include "../../include/game_structs.h"
#include "../../include/constants.h"
#include "../../include/error_codes.h"

// Virus management function declarations

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
int kill_virus(int virus_pid, GameState_t *game_state);

/**
 * Create a test virus process (for testing)
 * @param virus_name Name of virus
 * @param level Virus level
 * @return PID of created virus, -1 on error
 */
int create_test_virus(const char *virus_name, int level);

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
 * Update virus kill count in game state
 * @param game_state Pointer to game state
 * @return 0 on success, -1 on error
 */
int update_virus_kill_count(GameState_t *game_state);

/**
 * List all active viruses
 * @param viruses Array to store virus information
 * @param count Pointer to store number of viruses
 * @return 0 on success, -1 on error
 */
int list_active_viruses(VirusProcess viruses[], int *count);

/**
 * Cleanup virus processes
 */
void cleanup_virus_processes(void);

#endif // VIRUS_MANAGER_H