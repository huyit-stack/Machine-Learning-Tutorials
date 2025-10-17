#ifndef RANKING_SYSTEM_H
#define RANKING_SYSTEM_H

#include "../../include/game_structs.h"
#include "../../include/constants.h"
#include "../../include/error_codes.h"

// Ranking system function declarations

/**
 * Initialize ranking system
 * @param ranking Pointer to ranking system
 */
void init_ranking(RankingSystem *ranking);

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
 * Get top N players
 * @param ranking Pointer to ranking system
 * @param top_players Array to store top players
 * @param n Number of top players to get
 * @return Number of players returned
 */
int get_top_players(RankingSystem *ranking, PlayerRecord top_players[], int n);

/**
 * Check if new high score
 * @param ranking Pointer to ranking system
 * @param score Score to check
 * @return 1 if high score, 0 otherwise
 */
int is_high_score(RankingSystem *ranking, int score);

/**
 * Free ranking system memory
 * @param ranking Pointer to ranking system
 */
void free_ranking(RankingSystem *ranking);

#endif // RANKING_SYSTEM_H