#include "ui_manager.h"
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

// Global UI state
extern UIState g_ui_state;

int init_ui(UIState *ui_state) {
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);
    
    // Enable colors
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_BLUE, COLOR_BLACK);
        init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(6, COLOR_CYAN, COLOR_BLACK);
        init_pair(7, COLOR_WHITE, COLOR_BLACK);
    }
    
    // Get screen dimensions
    getmaxyx(stdscr, ui_state->screen_height, ui_state->screen_width);
    
    // Initialize UI state
    ui_state->current_menu = 0;
    ui_state->selected_item = 0;
    ui_state->input_pos = 0;
    memset(ui_state->input_buffer, 0, sizeof(ui_state->input_buffer));
    
    return 0;
}

void cleanup_ui(void) {
    endwin();
}

void display_main_menu(UIState *ui_state, GameState_t *game_state) {
    clear();
    
    // Display welcome art
    display_welcome_art();
    
    // Display menu options
    mvprintw(ui_state->screen_height / 2 + 5, ui_state->screen_width / 2 - 15, 
             "1. Bắt đầu game");
    mvprintw(ui_state->screen_height / 2 + 6, ui_state->screen_width / 2 - 15, 
             "2. Xem ranking");
    mvprintw(ui_state->screen_height / 2 + 7, ui_state->screen_width / 2 - 15, 
             "3. Hướng dẫn");
    mvprintw(ui_state->screen_height / 2 + 8, ui_state->screen_width / 2 - 15, 
             "4. Thoát");
    
    // Highlight selected item
    attron(A_REVERSE);
    mvprintw(ui_state->screen_height / 2 + 5 + ui_state->selected_item, 
             ui_state->screen_width / 2 - 15, 
             "%d. %s", 
             ui_state->selected_item + 1,
             ui_state->selected_item == 0 ? "Bắt đầu game" :
             ui_state->selected_item == 1 ? "Xem ranking" :
             ui_state->selected_item == 2 ? "Hướng dẫn" : "Thoát");
    attroff(A_REVERSE);
    
    refresh();
}

void display_game_interface(UIState *ui_state, GameState_t *game_state, 
                           FileInfo files[], int file_count) {
    clear();
    
    // Display game header
    attron(COLOR_PAIR(6) | A_BOLD);
    mvprintw(0, 0, "File System Adventure Game - Level %d", game_state->level);
    attroff(COLOR_PAIR(6) | A_BOLD);
    
    // Display current directory
    mvprintw(1, 0, "Thư mục hiện tại: %s", game_state->current_dir);
    
    // Display game statistics
    display_stats(game_state, 2, 0);
    
    // Display file list
    mvprintw(6, 0, "Files trong thư mục:");
    display_file_list(files, file_count, 7, ui_state->screen_height - 10);
    
    // Display command prompt
    display_prompt(ui_state, game_state);
    
    refresh();
}

void display_ranking(UIState *ui_state, RankingSystem *ranking) {
    clear();
    
    attron(COLOR_PAIR(4) | A_BOLD);
    mvprintw(0, ui_state->screen_width / 2 - 10, "BẢNG XẾP HẠNG");
    attroff(COLOR_PAIR(4) | A_BOLD);
    
    mvprintw(2, 0, "%-4s %-20s %-8s %-6s %-6s %-6s %-8s", 
             "Rank", "Tên", "Điểm", "Kho báu", "Bẫy", "Virus", "Thời gian");
    mvprintw(3, 0, "--------------------------------------------------------");
    
    int start_row = 4;
    int max_display = ui_state->screen_height - start_row - 2;
    int display_count = (ranking->count < max_display) ? ranking->count : max_display;
    
    for (int i = 0; i < display_count; i++) {
        PlayerRecord *record = &ranking->records[i];
        char time_str[32];
        strftime(time_str, sizeof(time_str), "%H:%M:%S", localtime(&record->play_time));
        
        attron(COLOR_PAIR(i < 3 ? 2 : 7));
        mvprintw(start_row + i, 0, "%-4d %-20s %-8d %-6d %-6d %-6d %-8s",
                 i + 1, record->name, record->score, record->treasures_found,
                 record->traps_hit, record->viruses_killed, time_str);
        attroff(COLOR_PAIR(i < 3 ? 2 : 7));
    }
    
    mvprintw(ui_state->screen_height - 1, 0, "Nhấn bất kỳ phím nào để quay lại menu chính...");
    refresh();
}

void display_game_over(UIState *ui_state, GameState_t *game_state, int final_score) {
    clear();
    
    display_game_over_art();
    
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(ui_state->screen_height / 2 + 3, ui_state->screen_width / 2 - 10, 
             "GAME OVER!");
    attroff(COLOR_PAIR(1) | A_BOLD);
    
    mvprintw(ui_state->screen_height / 2 + 5, ui_state->screen_width / 2 - 15, 
             "Điểm cuối cùng: %d", final_score);
    mvprintw(ui_state->screen_height / 2 + 6, ui_state->screen_width / 2 - 15, 
             "Kho báu tìm được: %d", game_state->treasures_found);
    mvprintw(ui_state->screen_height / 2 + 7, ui_state->screen_width / 2 - 15, 
             "Bẫy gặp phải: %d", game_state->traps_hit);
    mvprintw(ui_state->screen_height / 2 + 8, ui_state->screen_width / 2 - 15, 
             "Virus tiêu diệt: %d", game_state->viruses_killed);
    
    mvprintw(ui_state->screen_height - 1, 0, "Nhấn bất kỳ phím nào để quay lại menu chính...");
    refresh();
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
    attron(COLOR_PAIR(6) | A_BOLD);
    mvprintw(2, g_ui_state.screen_width / 2 - 20, 
             "╔══════════════════════════════════════════════════════════════╗");
    mvprintw(3, g_ui_state.screen_width / 2 - 20, 
             "║                    FILE SYSTEM ADVENTURE                    ║");
    mvprintw(4, g_ui_state.screen_width / 2 - 20, 
             "║                    Game Console Linux                     ║");
    mvprintw(5, g_ui_state.screen_width / 2 - 20, 
             "╚══════════════════════════════════════════════════════════════╝");
    attroff(COLOR_PAIR(6) | A_BOLD);
}

void display_game_over_art(void) {
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(2, ui_state->screen_width / 2 - 15, 
             "  ██████  █████  ███    ███ ███████     ██████  ██    ██ ███████ ██████");
    mvprintw(3, ui_state->screen_width / 2 - 15, 
             " ██      ██   ██ ████  ████ ██          ██   ██ ██    ██ ██      ██   ██");
    mvprintw(4, ui_state->screen_width / 2 - 15, 
             " ██      ███████ ██ ████ ██ █████       ██   ██ ██    ██ █████   ██████");
    mvprintw(5, ui_state->screen_width / 2 - 15, 
             " ██      ██   ██ ██  ██  ██ ██          ██   ██  ██  ██  ██      ██   ██");
    mvprintw(6, ui_state->screen_width / 2 - 15, 
             "  ██████ ██   ██ ██      ██ ███████     ██████   ████   ███████ ██   ██");
    attroff(COLOR_PAIR(1) | A_BOLD);
}

void display_level_up(int new_level) {
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(ui_state->screen_height / 2, ui_state->screen_width / 2 - 15, 
             "🎉 LEVEL UP! Bạn đã lên cấp %d! 🎉", new_level);
    attroff(COLOR_PAIR(2) | A_BOLD);
    refresh();
    napms(2000); // Pause for 2 seconds
}

void display_file_list(FileInfo files[], int count, int start_row, int max_rows) {
    int display_count = (count < max_rows) ? count : max_rows;
    
    for (int i = 0; i < display_count; i++) {
        char size_str[16];
        char perm_str[10];
        
        format_file_size(files[i].size, size_str, sizeof(size_str));
        get_permissions_string(files[i].permissions, perm_str, sizeof(perm_str));
        
        // Choose color based on file type
        int color_pair = 7; // Default white
        if (files[i].is_directory) {
            color_pair = 4; // Blue for directories
        } else if (files[i].type == FILE_TYPE_TREASURE) {
            color_pair = 2; // Green for treasures
        } else if (files[i].type == FILE_TYPE_TRAP) {
            color_pair = 1; // Red for traps
        } else if (files[i].is_hidden) {
            color_pair = 3; // Yellow for hidden files
        }
        
        attron(COLOR_PAIR(color_pair));
        mvprintw(start_row + i, 0, "%-20s %-10s %-10s %s",
                 files[i].name, size_str, perm_str,
                 files[i].is_directory ? "[DIR]" : 
                 files[i].type == FILE_TYPE_TREASURE ? "[TREASURE]" :
                 files[i].type == FILE_TYPE_TRAP ? "[TRAP]" : "");
        attroff(COLOR_PAIR(color_pair));
    }
}

void display_stats(GameState_t *game_state, int row, int col) {
    mvprintw(row, col, "Điểm: %d", game_state->score);
    mvprintw(row, col + 15, "Mạng: %d", game_state->lives);
    mvprintw(row, col + 30, "Kho báu: %d", game_state->treasures_found);
    mvprintw(row, col + 50, "Bẫy: %d", game_state->traps_hit);
    mvprintw(row + 1, col, "Virus: %d", game_state->viruses_killed);
    mvprintw(row + 1, col + 15, "Hack: %d/%d", game_state->hack_count, MAX_HACK_ATTEMPTS);
    mvprintw(row + 1, col + 30, "Level: %d", game_state->level);
}

void display_prompt(UIState *ui_state, GameState_t *game_state) {
    mvprintw(ui_state->screen_height - 3, 0, "Lệnh: ");
    mvprintw(ui_state->screen_height - 2, 0, "> %s_", ui_state->input_buffer);
}

int get_user_input(UIState *ui_state) {
    int ch = getch();
    
    switch (ch) {
        case KEY_BACKSPACE:
        case 127:
            if (ui_state->input_pos > 0) {
                ui_state->input_pos--;
                ui_state->input_buffer[ui_state->input_pos] = '\0';
            }
            break;
            
        case '\n':
        case '\r':
            return 0; // Input complete
            
        default:
            if (ui_state->input_pos < MAX_COMMAND_LEN - 1 && isprint(ch)) {
                ui_state->input_buffer[ui_state->input_pos] = ch;
                ui_state->input_pos++;
                ui_state->input_buffer[ui_state->input_pos] = '\0';
            }
            break;
    }
    
    return 1; // Continue input
}

void display_help(UIState *ui_state) {
    clear();
    
    attron(COLOR_PAIR(4) | A_BOLD);
    mvprintw(0, ui_state->screen_width / 2 - 10, "HƯỚNG DẪN CHƠI");
    attroff(COLOR_PAIR(4) | A_BOLD);
    
    mvprintw(2, 0, "Các lệnh có sẵn:");
    mvprintw(3, 0, "  ls          - Liệt kê files trong thư mục");
    mvprintw(4, 0, "  cd <dir>    - Chuyển thư mục");
    mvprintw(5, 0, "  open <file> - Mở file (có thể là kho báu hoặc bẫy)");
    mvprintw(6, 0, "  hack <file> - Hack quyền file (Admin only)");
    mvprintw(7, 0, "  scan        - Scan virus (Admin only)");
    mvprintw(8, 0, "  kill <pid>  - Kill virus (Admin only)");
    mvprintw(9, 0, "  quit        - Thoát game");
    
    mvprintw(11, 0, "Mục tiêu:");
    mvprintw(12, 0, "  - Tìm kho báu để tăng điểm");
    mvprintw(13, 0, "  - Tránh bẫy để không mất mạng");
    mvprintw(14, 0, "  - Kill virus để tăng điểm (Admin)");
    mvprintw(15, 0, "  - Hack file để tăng điểm (Admin)");
    
    mvprintw(ui_state->screen_height - 1, 0, "Nhấn bất kỳ phím nào để quay lại...");
    refresh();
}

void display_error(const char *message) {
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(g_ui_state.screen_height - 1, 0, "✗ %s", message);
    attroff(COLOR_PAIR(1) | A_BOLD);
    refresh();
    napms(2000);
}

void display_success(const char *message) {
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(g_ui_state.screen_height - 1, 0, "✓ %s", message);
    attroff(COLOR_PAIR(2) | A_BOLD);
    refresh();
    napms(1000);
}

void display_warning(const char *message) {
    attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(g_ui_state.screen_height - 1, 0, "⚠ %s", message);
    attroff(COLOR_PAIR(3) | A_BOLD);
    refresh();
    napms(1500);
}

void clear_screen(void) {
    clear();
}

void refresh_display(void) {
    refresh();
}

// Helper functions for file_manager integration
void format_file_size(off_t size, char *buffer, size_t buffer_size) {
    const char *units[] = {"B", "KB", "MB", "GB"};
    int unit_index = 0;
    double file_size = (double)size;
    
    while (file_size >= 1024.0 && unit_index < 3) {
        file_size /= 1024.0;
        unit_index++;
    }
    
    snprintf(buffer, buffer_size, "%.1f %s", file_size, units[unit_index]);
}

void get_permissions_string(mode_t permissions, char *buffer, size_t buffer_size) {
    char perm_str[10];
    int index = 0;
    
    // Owner permissions
    perm_str[index++] = (permissions & S_IRUSR) ? 'r' : '-';
    perm_str[index++] = (permissions & S_IWUSR) ? 'w' : '-';
    perm_str[index++] = (permissions & S_IXUSR) ? 'x' : '-';
    
    // Group permissions
    perm_str[index++] = (permissions & S_IRGRP) ? 'r' : '-';
    perm_str[index++] = (permissions & S_IWGRP) ? 'w' : '-';
    perm_str[index++] = (permissions & S_IXGRP) ? 'x' : '-';
    
    // Other permissions
    perm_str[index++] = (permissions & S_IROTH) ? 'r' : '-';
    perm_str[index++] = (permissions & S_IWOTH) ? 'w' : '-';
    perm_str[index++] = (permissions & S_IXOTH) ? 'x' : '-';
    
    perm_str[index] = '\0';
    strncpy(buffer, perm_str, buffer_size - 1);
    buffer[buffer_size - 1] = '\0';
}