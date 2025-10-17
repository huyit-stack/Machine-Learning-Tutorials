#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include "game_config.h"

// Function declarations for Module Storage/Ranking

/**
 * Calculate final score based on game statistics
 * @param game_state Pointer to game state
 * @param difficulty_multiplier Difficulty multiplier
 * @return Calculated final score
 */
int calculate_final_score(const GameState_t *game_state, float difficulty_multiplier);

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
 * Add player record to ranking
 * @param ranking Pointer to ranking system
 * @param record Player record to add
 * @return 0 on success, -1 on error
 */
int add_player_record(RankingSystem *ranking, const PlayerRecord *record);

/**
 * Sort ranking by score (descending)
 * @param ranking Pointer to ranking system
 */
void sort_ranking(RankingSystem *ranking);

/**
 * Get player rank
 * @param ranking Pointer to ranking system
 * @param player_name Name of player
 * @return Rank (1-based), 0 if not found
 */
int get_player_rank(RankingSystem *ranking, const char *player_name);

/**
 * Filter ranking by role
 * @param ranking Pointer to ranking system
 * @param role Role to filter by
 * @param filtered_ranking Pointer to store filtered results
 * @return Number of filtered records
 */
int filter_ranking_by_role(RankingSystem *ranking, PlayerRole role, 
                          PlayerRecord filtered_ranking[]);

/**
 * Export ranking to CSV
 * @param ranking Pointer to ranking system
 * @param filename Name of CSV file
 * @return 0 on success, -1 on error
 */
int export_ranking_csv(RankingSystem *ranking, const char *filename);

/**
 * Get difficulty multiplier from system load
 * @return Difficulty multiplier
 */
float get_difficulty_multiplier(void);

/**
 * Update game statistics
 * @param game_state Pointer to game state
 * @param action Action performed
 * @param points Points gained/lost
 */
void update_game_stats(GameState_t *game_state, const char *action, int points);

/**
 * Check if new high score
 * @param ranking Pointer to ranking system
 * @param score Score to check
 * @return 1 if high score, 0 otherwise
 */
int is_high_score(RankingSystem *ranking, int score);

/**
 * Get top N players
 * @param ranking Pointer to ranking system
 * @param top_players Array to store top players
 * @param n Number of top players to get
 * @return Number of players returned
 */
int get_top_players(RankingSystem *ranking, PlayerRecord top_players[], int n);

/**
 * Initialize ranking system
 * @param ranking Pointer to ranking system
 */
void init_ranking(RankingSystem *ranking);

/**
 * Free ranking system memory
 * @param ranking Pointer to ranking system
 */
void free_ranking(RankingSystem *ranking);

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

#endif // STORAGE_MANAGER_H