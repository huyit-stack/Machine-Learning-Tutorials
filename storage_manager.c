#define _GNU_SOURCE
#include "storage_manager.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>

int calculate_final_score(const GameState_t *game_state, float difficulty_multiplier) {
    if (game_state == NULL) {
        return 0;
    }
    
    int base_score = game_state->score;
    int treasure_bonus = game_state->treasures_found * 100;
    int virus_bonus = game_state->viruses_killed * 200;
    int hack_bonus = game_state->hack_count * 50;
    int trap_penalty = game_state->traps_hit * 50;
    int level_bonus = game_state->levels_completed * 500;
    
    // Time bonus (faster completion = higher bonus)
    time_t play_time = time(NULL) - game_state->start_time;
    int time_bonus = (play_time < 300) ? 1000 : (play_time < 600) ? 500 : 0;
    
    int final_score = (base_score + treasure_bonus + virus_bonus + hack_bonus + 
                      level_bonus + time_bonus - trap_penalty) * difficulty_multiplier;
    
    return (final_score > 0) ? final_score : 0;
}

int generate_report(const GameState_t *game_state, RankingSystem *ranking, 
                   char *report_buffer, size_t buffer_size) {
    if (game_state == NULL || ranking == NULL || report_buffer == NULL) {
        return -1;
    }
    
    time_t play_time = time(NULL) - game_state->start_time;
    float difficulty_multiplier = get_difficulty_multiplier();
    int final_score = calculate_final_score(game_state, difficulty_multiplier);
    
    snprintf(report_buffer, buffer_size,
             "╔══════════════════════════════════════════════════════════════╗\n"
             "║                    BÁO CÁO KẾT QUẢ GAME                      ║\n"
             "╠══════════════════════════════════════════════════════════════╣\n"
             "║ Điểm cuối cùng: %-40d ║\n"
             "║ Kho báu tìm được: %-36d ║\n"
             "║ Bẫy gặp phải: %-40d ║\n"
             "║ Virus tiêu diệt: %-37d ║\n"
             "║ Lần hack: %-43d ║\n"
             "║ Level hoàn thành: %-35d ║\n"
             "║ Thời gian chơi: %-37ld giây ║\n"
             "║ Hệ số độ khó: %-39.2f ║\n"
             "║ Vai trò: %-45s ║\n"
             "╚══════════════════════════════════════════════════════════════╝\n",
             final_score,
             game_state->treasures_found,
             game_state->traps_hit,
             game_state->viruses_killed,
             game_state->hack_count,
             game_state->levels_completed,
             play_time,
             difficulty_multiplier,
             game_state->role == ROLE_ADMIN ? "Admin" : "User");
    
    return 0;
}

int save_game_state(const GameState_t *game_state, const char *filename) {
    if (game_state == NULL || filename == NULL) {
        return -1;
    }
    
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return -1;
    }
    
    size_t written = fwrite(game_state, sizeof(GameState_t), 1, fp);
    fclose(fp);
    
    return (written == 1) ? 0 : -1;
}

int load_game_state(GameState_t *game_state, const char *filename) {
    if (game_state == NULL || filename == NULL) {
        return -1;
    }
    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return -1;
    }
    
    size_t read = fread(game_state, sizeof(GameState_t), 1, fp);
    fclose(fp);
    
    return (read == 1) ? 0 : -1;
}

int save_ranking(RankingSystem *ranking, const char *filename) {
    if (ranking == NULL || filename == NULL) {
        return -1;
    }
    
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return -1;
    }
    
    // Write count first
    if (fwrite(&ranking->count, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return -1;
    }
    
    // Write records
    size_t written = fwrite(ranking->records, sizeof(PlayerRecord), ranking->count, fp);
    fclose(fp);
    
    return (written == ranking->count) ? 0 : -1;
}

int load_ranking(RankingSystem *ranking, const char *filename) {
    if (ranking == NULL || filename == NULL) {
        return -1;
    }
    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        // File doesn't exist, initialize empty ranking
        init_ranking(ranking);
        return 0;
    }
    
    // Read count
    if (fread(&ranking->count, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        init_ranking(ranking);
        return -1;
    }
    
    // Validate count
    if (ranking->count < 0 || ranking->count > MAX_RECORDS) {
        fclose(fp);
        init_ranking(ranking);
        return -1;
    }
    
    // Read records
    size_t read = fread(ranking->records, sizeof(PlayerRecord), ranking->count, fp);
    fclose(fp);
    
    if (read != ranking->count) {
        init_ranking(ranking);
        return -1;
    }
    
    // Sort ranking after loading
    sort_ranking(ranking);
    
    return 0;
}

int add_player_record(RankingSystem *ranking, const PlayerRecord *record) {
    if (ranking == NULL || record == NULL) {
        return -1;
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
            return 0; // Not high enough to be added
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
    
    return 0;
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

int export_ranking_csv(RankingSystem *ranking, const char *filename) {
    if (ranking == NULL || filename == NULL) {
        return -1;
    }
    
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        return -1;
    }
    
    // Write CSV header
    fprintf(fp, "Rank,Name,Score,Treasures,Traps,Viruses,Hacks,Levels,PlayTime,Role\n");
    
    // Write records
    for (int i = 0; i < ranking->count; i++) {
        PlayerRecord *record = &ranking->records[i];
        fprintf(fp, "%d,%s,%d,%d,%d,%d,%d,%d,%ld,%s\n",
                i + 1,
                record->name,
                record->score,
                record->treasures_found,
                record->traps_hit,
                record->viruses_killed,
                record->hack_count,
                record->levels_completed,
                record->play_time,
                record->role == ROLE_ADMIN ? "Admin" : "User");
    }
    
    fclose(fp);
    return 0;
}

float get_difficulty_multiplier(void) {
    double loadavg[3];
    
    if (getloadavg(loadavg, 3) == -1) {
        return 1.0f; // Default multiplier if can't get load average
    }
    
    // Use 1-minute load average
    double load = loadavg[0];
    
    // Adjust multiplier based on system load
    if (load > 2.0) {
        return 1.5f; // High load = higher difficulty
    } else if (load > 1.0) {
        return 1.2f; // Medium load = slightly higher difficulty
    } else {
        return 1.0f; // Low load = normal difficulty
    }
}

void update_game_stats(GameState_t *game_state, const char *action, int points) {
    if (game_state == NULL || action == NULL) {
        return;
    }
    
    game_state->score += points;
    
    if (strcmp(action, "treasure") == 0) {
        game_state->treasures_found++;
    } else if (strcmp(action, "trap") == 0) {
        game_state->traps_hit++;
        game_state->lives--;
    } else if (strcmp(action, "virus") == 0) {
        game_state->viruses_killed++;
    } else if (strcmp(action, "hack") == 0) {
        game_state->hack_count++;
    } else if (strcmp(action, "level") == 0) {
        game_state->levels_completed++;
    }
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

void init_ranking(RankingSystem *ranking) {
    if (ranking == NULL) {
        return;
    }
    
    ranking->count = 0;
    memset(ranking->records, 0, sizeof(ranking->records));
}

void free_ranking(RankingSystem *ranking) {
    // No dynamic memory to free in current implementation
    // This function is here for future extensibility
    (void)ranking;
}

int backup_ranking(RankingSystem *ranking, const char *backup_filename) {
    if (ranking == NULL || backup_filename == NULL) {
        return -1;
    }
    
    // Create backup filename with timestamp
    char timestamp[32];
    time_t now = time(NULL);
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", localtime(&now));
    
    char full_backup_name[256];
    snprintf(full_backup_name, sizeof(full_backup_name), "%s_%s", backup_filename, timestamp);
    
    return save_ranking(ranking, full_backup_name);
}

int restore_ranking(RankingSystem *ranking, const char *backup_filename) {
    if (ranking == NULL || backup_filename == NULL) {
        return -1;
    }
    
    return load_ranking(ranking, backup_filename);
}