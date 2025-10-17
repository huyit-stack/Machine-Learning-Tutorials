#include "console_ui.h"
#include "../../include/utils.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void display_welcome_art(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    FILE SYSTEM ADVENTURE                    ║\n");
    printf("║                    Game Console Linux                     ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

void display_game_over_art(void) {
    printf("  ██████  █████  ███    ███ ███████     ██████  ██    ██ ███████ ██████\n");
    printf(" ██      ██   ██ ████  ████ ██          ██   ██ ██    ██ ██      ██   ██\n");
    printf(" ██      ███████ ██ ████ ██ █████       ██   ██ ██    ██ █████   ██████\n");
    printf(" ██      ██   ██ ██  ██  ██ ██          ██   ██  ██  ██  ██      ██   ██\n");
    printf("  ██████ ██   ██ ██      ██ ███████     ██████   ████   ███████ ██   ██\n");
    printf("\n");
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

void display_help(UIState *ui_state) {
    clear_screen();
    
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