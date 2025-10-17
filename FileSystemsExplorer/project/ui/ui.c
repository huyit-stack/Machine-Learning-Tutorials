#include "ui.h"
#include "console_ui.h"
#include "command_parser.h"
#include "../../include/utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int init_ui(UIState *ui_state) {
    // Initialize simple console UI
    ui_state->screen_height = 24;
    ui_state->screen_width = 80;
    ui_state->current_menu = 0;
    ui_state->selected_item = 0;
    ui_state->input_pos = 0;
    memset(ui_state->input_buffer, 0, sizeof(ui_state->input_buffer));
    
    printf("UI initialized (simple console mode)\n");
    return SUCCESS;
}

void cleanup_ui(void) {
    printf("\nUI cleanup completed\n");
}

void display_main_menu(UIState *ui_state, GameState_t *game_state) {
    clear_screen();
    display_welcome_art();
    
    printf("1. Bắt đầu game\n");
    printf("2. Xem ranking\n");
    printf("3. Hướng dẫn\n");
    printf("4. Thoát\n");
    printf("\n");
    printf("Chọn chức năng (1-4): ");
}

void display_game_interface(UIState *ui_state, GameState_t *game_state, 
                           FileInfo files[], int file_count) {
    clear_screen();
    
    printf("File System Adventure Game - Level %d\n", game_state->level);
    printf("Thư mục hiện tại: %s\n", game_state->current_dir);
    display_stats(game_state, 0, 0);
    printf("\n");
    
    printf("Files trong thư mục:\n");
    display_file_list(files, file_count, 0, 20);
    
    printf("\nLệnh: ");
}

void display_ranking(UIState *ui_state, RankingSystem *ranking) {
    clear_screen();
    
    printf("BẢNG XẾP HẠNG\n");
    printf("Rank  Tên                  Điểm    Kho báu  Bẫy    Virus   Thời gian\n");
    printf("--------------------------------------------------------------------\n");
    
    int display_count = (ranking->count < 20) ? ranking->count : 20;
    for (int i = 0; i < display_count; i++) {
        PlayerRecord *record = &ranking->records[i];
        char time_str[32];
        strftime(time_str, sizeof(time_str), "%H:%M:%S", localtime(&record->play_time));
        
        printf("%-4d  %-20s %-8d %-6d  %-6d  %-6d  %-8s\n",
               i + 1, record->name, record->score, record->treasures_found,
               record->traps_hit, record->viruses_killed, time_str);
    }
    
    printf("\nNhấn Enter để quay lại menu chính...");
    getchar();
}

void display_game_over(UIState *ui_state, GameState_t *game_state, int final_score) {
    clear_screen();
    display_game_over_art();
    
    printf("GAME OVER!\n");
    printf("Điểm cuối cùng: %d\n", final_score);
    printf("Kho báu tìm được: %d\n", game_state->treasures_found);
    printf("Bẫy gặp phải: %d\n", game_state->traps_hit);
    printf("Virus tiêu diệt: %d\n", game_state->viruses_killed);
    
    printf("\nNhấn Enter để quay lại menu chính...");
    getchar();
}

int get_user_input(UIState *ui_state) {
    if (fgets(ui_state->input_buffer, sizeof(ui_state->input_buffer), stdin) != NULL) {
        // Remove newline
        ui_state->input_buffer[strcspn(ui_state->input_buffer, "\n")] = 0;
        return SUCCESS; // Input complete
    }
    return ERROR_SYSTEM_CALL; // Continue input
}