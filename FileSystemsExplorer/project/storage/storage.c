#include "storage.h"
#include "game_state.h"
#include "ranking_system.h"
#include "save_load.h"
#include "../../include/utils.h"
#include <string.h>

int init_storage_module(void) {
    return SUCCESS;
}

void cleanup_storage_module(void) {
    // Cleanup if needed
}

int process_storage_command(const char *command, GameState_t *game_state, RankingSystem *ranking) {
    char args[10][MAX_COMMAND_LEN];
    int arg_count = 0;
    char command_copy[MAX_COMMAND_LEN];
    
    // Parse command
    safe_strcpy(command_copy, command, sizeof(command_copy));
    char *token = strtok(command_copy, " \t\n");
    
    while (token != NULL && arg_count < 10) {
        safe_strcpy(args[arg_count], token, MAX_COMMAND_LEN);
        arg_count++;
        token = strtok(NULL, " \t\n");
    }
    
    if (arg_count == 0) {
        return ERROR_INVALID_INPUT;
    }
    
    // Process different storage commands
    if (strcmp(args[0], "save") == 0) {
        const char *filename = (arg_count > 1) ? args[1] : "game_save.dat";
        int result = save_game_state(game_state, filename);
        if (result == SUCCESS) {
            display_success("Game đã được lưu!");
        }
        return result;
    }
    else if (strcmp(args[0], "load") == 0) {
        const char *filename = (arg_count > 1) ? args[1] : "game_save.dat";
        int result = load_game_state(game_state, filename);
        if (result == SUCCESS) {
            display_success("Game đã được tải!");
        }
        return result;
    }
    else if (strcmp(args[0], "ranking") == 0) {
        display_success("Hiển thị ranking...");
        return SUCCESS;
    }
    else if (strcmp(args[0], "export") == 0) {
        const char *filename = (arg_count > 1) ? args[1] : "ranking.csv";
        int result = export_ranking_csv(ranking, filename);
        if (result == SUCCESS) {
            display_success("Ranking đã được export!");
        }
        return result;
    }
    else {
        display_error("Lệnh storage không hợp lệ");
        return ERROR_INVALID_INPUT;
    }
}