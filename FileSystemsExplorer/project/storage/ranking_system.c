#include "ranking_system.h"
#include "../../include/utils.h"
#include <string.h>

void init_ranking(RankingSystem *ranking) {
    if (ranking == NULL) {
        return;
    }
    
    ranking->count = 0;
    memset(ranking->records, 0, sizeof(ranking->records));
}

int add_player_record(RankingSystem *ranking, const PlayerRecord *record) {
    if (ranking == NULL || record == NULL) {
        return ERROR_INVALID_INPUT;
    }
    
    // Check if we have space
    if (ranking->count >= MAX_RECORDS) {
        // Remove lowest score if we're at capacity
        int min_score = ranking->records[0].score;
        int min_index = 0;
        
        for (int i = 1; i < ranking->count; i++) {
            if (ranking->records[i].score < min_score) {
                min_score = ranking->records[i].score;
                min_index = i;
            }
        }
        
        // Only add if new score is higher than minimum
        if (record->score <= min_score) {
            return SUCCESS; // Not high enough to be added
        }
        
        // Replace minimum score record
        ranking->records[min_index] = *record;
    } else {
        // Add new record
        ranking->records[ranking->count] = *record;
        ranking->count++;
    }
    
    // Sort ranking
    sort_ranking(ranking);
    
    return SUCCESS;
}

void sort_ranking(RankingSystem *ranking) {
    if (ranking == NULL || ranking->count <= 1) {
        return;
    }
    
    // Simple bubble sort by score (descending)
    for (int i = 0; i < ranking->count - 1; i++) {
        for (int j = 0; j < ranking->count - i - 1; j++) {
            if (ranking->records[j].score < ranking->records[j + 1].score) {
                PlayerRecord temp = ranking->records[j];
                ranking->records[j] = ranking->records[j + 1];
                ranking->records[j + 1] = temp;
            }
        }
    }
}

int get_player_rank(RankingSystem *ranking, const char *player_name) {
    if (ranking == NULL || player_name == NULL) {
        return 0;
    }
    
    for (int i = 0; i < ranking->count; i++) {
        if (strcmp(ranking->records[i].name, player_name) == 0) {
            return i + 1; // 1-based ranking
        }
    }
    
    return 0; // Player not found
}

int filter_ranking_by_role(RankingSystem *ranking, PlayerRole role, 
                          PlayerRecord filtered_ranking[]) {
    if (ranking == NULL || filtered_ranking == NULL) {
        return 0;
    }
    
    int filtered_count = 0;
    
    for (int i = 0; i < ranking->count && filtered_count < MAX_RECORDS; i++) {
        if (ranking->records[i].role == role) {
            filtered_ranking[filtered_count] = ranking->records[i];
            filtered_count++;
        }
    }
    
    return filtered_count;
}

int get_top_players(RankingSystem *ranking, PlayerRecord top_players[], int n) {
    if (ranking == NULL || top_players == NULL || n <= 0) {
        return 0;
    }
    
    int count = (n < ranking->count) ? n : ranking->count;
    
    for (int i = 0; i < count; i++) {
        top_players[i] = ranking->records[i];
    }
    
    return count;
}

int is_high_score(RankingSystem *ranking, int score) {
    if (ranking == NULL) {
        return 0;
    }
    
    // Check if score would be in top 10
    if (ranking->count < 10) {
        return 1;
    }
    
    return score > ranking->records[9].score;
}

void free_ranking(RankingSystem *ranking) {
    // No dynamic memory to free in current implementation
    // This function is here for future extensibility
    (void)ranking;
}