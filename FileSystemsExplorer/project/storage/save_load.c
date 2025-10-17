#include "save_load.h"
#include "../../include/utils.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>

int save_game_state(const GameState_t *game_state, const char *filename) {
    if (game_state == NULL || filename == NULL) {
        return ERROR_INVALID_INPUT;
    }
    
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return ERROR_SYSTEM_CALL;
    }
    
    size_t written = fwrite(game_state, sizeof(GameState_t), 1, fp);
    fclose(fp);
    
    return (written == 1) ? SUCCESS : ERROR_SYSTEM_CALL;
}

int load_game_state(GameState_t *game_state, const char *filename) {
    if (game_state == NULL || filename == NULL) {
        return ERROR_INVALID_INPUT;
    }
    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return ERROR_FILE_NOT_FOUND;
    }
    
    size_t read = fread(game_state, sizeof(GameState_t), 1, fp);
    fclose(fp);
    
    return (read == 1) ? SUCCESS : ERROR_SYSTEM_CALL;
}

int save_ranking(RankingSystem *ranking, const char *filename) {
    if (ranking == NULL || filename == NULL) {
        return ERROR_INVALID_INPUT;
    }
    
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return ERROR_SYSTEM_CALL;
    }
    
    // Write count first
    if (fwrite(&ranking->count, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return ERROR_SYSTEM_CALL;
    }
    
    // Write records
    size_t written = fwrite(ranking->records, sizeof(PlayerRecord), ranking->count, fp);
    fclose(fp);
    
    return (written == (size_t)ranking->count) ? SUCCESS : ERROR_SYSTEM_CALL;
}

int load_ranking(RankingSystem *ranking, const char *filename) {
    if (ranking == NULL || filename == NULL) {
        return ERROR_INVALID_INPUT;
    }
    
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        // File doesn't exist, initialize empty ranking
        init_ranking(ranking);
        return SUCCESS;
    }
    
    // Read count
    if (fread(&ranking->count, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        init_ranking(ranking);
        return ERROR_SYSTEM_CALL;
    }
    
    // Validate count
    if (ranking->count < 0 || ranking->count > MAX_RECORDS) {
        fclose(fp);
        init_ranking(ranking);
        return ERROR_INVALID_INPUT;
    }
    
    // Read records
    size_t read = fread(ranking->records, sizeof(PlayerRecord), ranking->count, fp);
    fclose(fp);
    
    if (read != (size_t)ranking->count) {
        init_ranking(ranking);
        return ERROR_SYSTEM_CALL;
    }
    
    // Sort ranking after loading
    sort_ranking(ranking);
    
    return SUCCESS;
}

int generate_report(const GameState_t *game_state, RankingSystem *ranking, 
                   char *report_buffer, size_t buffer_size) {
    if (game_state == NULL || ranking == NULL || report_buffer == NULL) {
        return ERROR_INVALID_INPUT;
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
    
    return SUCCESS;
}

int export_ranking_csv(RankingSystem *ranking, const char *filename) {
    if (ranking == NULL || filename == NULL) {
        return ERROR_INVALID_INPUT;
    }
    
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        return ERROR_SYSTEM_CALL;
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
    return SUCCESS;
}

int backup_ranking(RankingSystem *ranking, const char *backup_filename) {
    if (ranking == NULL || backup_filename == NULL) {
        return ERROR_INVALID_INPUT;
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
        return ERROR_INVALID_INPUT;
    }
    
    return load_ranking(ranking, backup_filename);
}