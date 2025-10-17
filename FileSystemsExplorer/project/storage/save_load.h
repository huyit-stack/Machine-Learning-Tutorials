#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include "../../include/game_structs.h"
#include "../../include/constants.h"
#include "../../include/error_codes.h"

// Save/Load function declarations

/**
 * Save game state to file
 * @param game_state Pointer to game state
 * @param filename Name of file to save to
 * @return 0 on success, -1 on error
 */
int save_game_state(const GameState_t *game_state, const char *filename);

/**
 * Load game state from file
 * @param game_state Pointer to game state
 * @param filename Name of file to load from
 * @return 0 on success, -1 on error
 */
int load_game_state(GameState_t *game_state, const char *filename);

/**
 * Save ranking system to file
 * @param ranking Pointer to ranking system
 * @param filename Name of file to save to
 * @return 0 on success, -1 on error
 */
int save_ranking(RankingSystem *ranking, const char *filename);

/**
 * Load ranking system from file
 * @param ranking Pointer to ranking system
 * @param filename Name of file to load from
 * @return 0 on success, -1 on error
 */
int load_ranking(RankingSystem *ranking, const char *filename);

/**
 * Generate game report
 * @param game_state Pointer to game state
 * @param ranking Pointer to ranking system
 * @param report_buffer Buffer to store report
 * @param buffer_size Size of buffer
 * @return 0 on success, -1 on error
 */
int generate_report(const GameState_t *game_state, RankingSystem *ranking, 
                   char *report_buffer, size_t buffer_size);

/**
 * Export ranking to CSV
 * @param ranking Pointer to ranking system
 * @param filename Name of CSV file
 * @return 0 on success, -1 on error
 */
int export_ranking_csv(RankingSystem *ranking, const char *filename);

/**
 * Backup ranking data
 * @param ranking Pointer to ranking system
 * @param backup_filename Name of backup file
 * @return 0 on success, -1 on error
 */
int backup_ranking(RankingSystem *ranking, const char *backup_filename);

/**
 * Restore ranking data from backup
 * @param ranking Pointer to ranking system
 * @param backup_filename Name of backup file
 * @return 0 on success, -1 on error
 */
int restore_ranking(RankingSystem *ranking, const char *backup_filename);

#endif // SAVE_LOAD_H