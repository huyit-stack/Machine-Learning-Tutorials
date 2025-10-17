#include "ui_manager.h"
#include "file_manager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Simple console-based UI implementation (without ncurses for compatibility)

int init_ui(UIState *ui_state) {
    // Initialize simple console UI
    ui_state->screen_height = 24;
    ui_state->screen_width = 80;
    ui_state->current_menu = 0;
    ui_state->selected_item = 0;
    ui_state->input_pos = 0;
    memset(ui_state->input_buffer, 0, sizeof(ui_state->input_buffer));
    
    printf("UI initialized (simple console mode)\n");
    return 0;
}

void cleanup_ui(void) {
    printf("\nUI cleanup completed\n");
}

void display_main_menu(UIState *ui_state, GameState_t *game_state) {
    system("clear");
    
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    FILE SYSTEM ADVENTURE                    ║\n");
    printf("║                    Game Console Linux                     ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    printf("1. Bắt đầu game\n");
    printf("2. Xem ranking\n");
    printf("3. Hướng dẫn\n");
    printf("4. Thoát\n");
    printf("\n");
    printf("Chọn chức năng (1-4): ");
}

void display_game_interface(UIState *ui_state, GameState_t *game_state, 
                           FileInfo files[], int file_count) {
    system("clear");
    
    printf("File System Adventure Game - Level %d\n", game_state->level);
    printf("Thư mục hiện tại: %s\n", game_state->current_dir);
    printf("Điểm: %d | Mạng: %d | Kho báu: %d | Bẫy: %d\n", 
           game_state->score, game_state->lives, 
           game_state->treasures_found, game_state->traps_hit);
    printf("Virus: %d | Hack: %d/%d | Level: %d\n", 
           game_state->viruses_killed, game_state->hack_count, 
           MAX_HACK_ATTEMPTS, game_state->level);
    printf("\n");
    
    printf("Files trong thư mục:\n");
    for (int i = 0; i < file_count && i < 20; i++) {
        char size_str[16];
        char perm_str[10];
        
        format_file_size(files[i].size, size_str, sizeof(size_str));
        get_permissions_string(files[i].permissions, perm_str, sizeof(perm_str));
        
        printf("%-20s %-10s %-10s %s\n",
               files[i].name, size_str, perm_str,
               files[i].is_directory ? "[DIR]" : 
               files[i].type == FILE_TYPE_TREASURE ? "[TREASURE]" :
               files[i].type == FILE_TYPE_TRAP ? "[TRAP]" : "");
    }
    
    printf("\nLệnh: ");
}

void display_ranking(UIState *ui_state, RankingSystem *ranking) {
    system("clear");
    
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
    system("clear");
    
    printf("  ██████  █████  ███    ███ ███████     ██████  ██    ██ ███████ ██████\n");
    printf(" ██      ██   ██ ████  ████ ██          ██   ██ ██    ██ ██      ██   ██\n");
    printf(" ██      ███████ ██ ████ ██ █████       ██   ██ ██    ██ █████   ██████\n");
    printf(" ██      ██   ██ ██  ██  ██ ██          ██   ██  ██  ██  ██      ██   ██\n");
    printf("  ██████ ██   ██ ██      ██ ███████     ██████   ████   ███████ ██   ██\n");
    printf("\n");
    
    printf("GAME OVER!\n");
    printf("Điểm cuối cùng: %d\n", final_score);
    printf("Kho báu tìm được: %d\n", game_state->treasures_found);
    printf("Bẫy gặp phải: %d\n", game_state->traps_hit);
    printf("Virus tiêu diệt: %d\n", game_state->viruses_killed);
    
    printf("\nNhấn Enter để quay lại menu chính...");
    getchar();
}

int parse_command(const char *command, char args[][MAX_COMMAND_LEN], int max_args) {
    char command_copy[MAX_COMMAND_LEN];
    strncpy(command_copy, command, sizeof(command_copy) - 1);
    command_copy[sizeof(command_copy) - 1] = '\0';
    
    int arg_count = 0;
    char *token = strtok(command_copy, " \t\n");
    
    while (token != NULL && arg_count < max_args) {
        strncpy(args[arg_count], token, MAX_COMMAND_LEN - 1);
        args[arg_count][MAX_COMMAND_LEN - 1] = '\0';
        arg_count++;
        token = strtok(NULL, " \t\n");
    }
    
    return arg_count;
}

void display_welcome_art(void) {
    // Already handled in display_main_menu
}

void display_game_over_art(void) {
    // Already handled in display_game_over
}

void display_level_up(int new_level) {
    printf("🎉 LEVEL UP! Bạn đã lên cấp %d! 🎉\n", new_level);
    sleep(2);
}

void display_file_list(FileInfo files[], int count, int start_row, int max_rows) {
    int display_count = (count < max_rows) ? count : max_rows;
    
    for (int i = 0; i < display_count; i++) {
        char size_str[16];
        char perm_str[10];
        
        format_file_size(files[i].size, size_str, sizeof(size_str));
        get_permissions_string(files[i].permissions, perm_str, sizeof(perm_str));
        
        printf("%-20s %-10s %-10s %s\n",
               files[i].name, size_str, perm_str,
               files[i].is_directory ? "[DIR]" : 
               files[i].type == FILE_TYPE_TREASURE ? "[TREASURE]" :
               files[i].type == FILE_TYPE_TRAP ? "[TRAP]" : "");
    }
}

void display_stats(GameState_t *game_state, int row, int col) {
    printf("Điểm: %d | Mạng: %d | Kho báu: %d | Bẫy: %d\n", 
           game_state->score, game_state->lives, 
           game_state->treasures_found, game_state->traps_hit);
    printf("Virus: %d | Hack: %d/%d | Level: %d\n", 
           game_state->viruses_killed, game_state->hack_count, 
           MAX_HACK_ATTEMPTS, game_state->level);
}

void display_prompt(UIState *ui_state, GameState_t *game_state) {
    printf("Lệnh: ");
}

int get_user_input(UIState *ui_state) {
    if (fgets(ui_state->input_buffer, sizeof(ui_state->input_buffer), stdin) != NULL) {
        // Remove newline
        ui_state->input_buffer[strcspn(ui_state->input_buffer, "\n")] = 0;
        return 0; // Input complete
    }
    return 1; // Continue input
}

void display_help(UIState *ui_state) {
    system("clear");
    
    printf("HƯỚNG DẪN CHƠI\n");
    printf("\n");
    printf("Các lệnh có sẵn:\n");
    printf("  ls          - Liệt kê files trong thư mục\n");
    printf("  cd <dir>    - Chuyển thư mục\n");
    printf("  open <file> - Mở file (có thể là kho báu hoặc bẫy)\n");
    printf("  hack <file> - Hack quyền file (Admin only)\n");
    printf("  scan        - Scan virus (Admin only)\n");
    printf("  kill <pid>  - Kill virus (Admin only)\n");
    printf("  quit        - Thoát game\n");
    printf("\n");
    printf("Mục tiêu:\n");
    printf("  - Tìm kho báu để tăng điểm\n");
    printf("  - Tránh bẫy để không mất mạng\n");
    printf("  - Kill virus để tăng điểm (Admin)\n");
    printf("  - Hack file để tăng điểm (Admin)\n");
    
    printf("\nNhấn Enter để quay lại...");
    getchar();
}

void clear_screen(void) {
    system("clear");
}

void refresh_display(void) {
    fflush(stdout);
}