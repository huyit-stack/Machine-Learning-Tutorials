#include "../include/game_structs.h"
#include "../include/constants.h"
#include "../include/error_codes.h"
#include "../include/utils.h"

// Module headers
#include "filesystem/file_manager.h"
#include "traps/traps.h"
#include "ui/ui.h"
#include "ui/command_parser.h"
#include "storage/storage.h"
#include "storage/game_state.h"
#include "storage/ranking_system.h"
#include "storage/save_load.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

// Global variables
static GameState_t g_game_state;
static RankingSystem g_ranking;
static UIState g_ui_state;
static int g_running = 1;

// Function prototypes
void init_game(void);
void cleanup_game(void);
void handle_signal(int sig);
void game_loop(void);
void process_command(char *command);
void create_test_files(void);
void show_main_menu(void);
void show_ranking(void);
void show_help(void);
void start_new_game(void);
void load_game(void);
void save_game(void);

void init_game(void) {
    // Initialize game state
    init_game_state(&g_game_state);
    
    // Initialize ranking system
    init_ranking(&g_ranking);
    load_ranking(&g_ranking, "data/ranking.dat");
    
    // Initialize UI
    if (init_ui(&g_ui_state) == ERROR_SYSTEM_CALL) {
        fprintf(stderr, "Không thể khởi tạo UI\n");
        exit(1);
    }
    
    // Initialize modules
    init_traps_module();
    init_storage_module();
    
    // Set up signal handlers
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    
    // Create test files for the game
    create_test_files();
}

void cleanup_game(void) {
    // Save game state if playing
    if (g_game_state.state == GAME_STATE_PLAYING) {
        save_game();
    }
    
    // Save ranking
    save_ranking(&g_ranking, "data/ranking.dat");
    
    // Cleanup modules
    cleanup_traps_module();
    cleanup_storage_module();
    
    // Cleanup UI
    cleanup_ui();
}

void handle_signal(int sig) {
    (void)sig;
    g_running = 0;
    cleanup_game();
    exit(0);
}

void game_loop(void) {
    char command[MAX_COMMAND_LEN];
    
    while (g_running) {
        switch (g_game_state.state) {
            case GAME_STATE_MENU:
                show_main_menu();
                break;
                
            case GAME_STATE_PLAYING:
                // Display game interface
                FileInfo files[MAX_RECORDS];
                int file_count = 0;
                list_files(files, &file_count);
                display_game_interface(&g_ui_state, &g_game_state, files, file_count);
                
                // Get user input
                if (get_user_input(&g_ui_state) == SUCCESS) {
                    safe_strcpy(command, g_ui_state.input_buffer, sizeof(command));
                    g_ui_state.input_pos = 0;
                    memset(g_ui_state.input_buffer, 0, sizeof(g_ui_state.input_buffer));
                    
                    process_command(command);
                }
                break;
                
            case GAME_STATE_RANKING:
                show_ranking();
                break;
                
            case GAME_STATE_GAME_OVER:
                {
                    float difficulty_multiplier = get_difficulty_multiplier();
                    int final_score = calculate_final_score(&g_game_state, difficulty_multiplier);
                    display_game_over(&g_ui_state, &g_game_state, final_score);
                    
                    // Add to ranking
                    PlayerRecord record;
                    safe_strcpy(record.name, "Player", sizeof(record.name));
                    record.score = final_score;
                    record.treasures_found = g_game_state.treasures_found;
                    record.traps_hit = g_game_state.traps_hit;
                    record.viruses_killed = g_game_state.viruses_killed;
                    record.hack_count = g_game_state.hack_count;
                    record.levels_completed = g_game_state.levels_completed;
                    record.play_time = time(NULL) - g_game_state.start_time;
                    record.role = g_game_state.role;
                    
                    add_player_record(&g_ranking, &record);
                    
                    getchar(); // Wait for key press
                    g_game_state.state = GAME_STATE_MENU;
                }
                break;
        }
    }
}

void process_command(char *command) {
    char args[10][MAX_COMMAND_LEN];
    int arg_count = parse_command(command, args, 10);
    
    if (arg_count == 0) {
        return;
    }
    
    // Route commands to appropriate modules
    if (strcmp(args[0], "ls") == 0 || strcmp(args[0], "cd") == 0 || strcmp(args[0], "open") == 0) {
        // Filesystem commands
        if (strcmp(args[0], "ls") == 0) {
            FileInfo files[MAX_RECORDS];
            int file_count = 0;
                if (list_files(files, &file_count) == SUCCESS) {
                // Display files manually
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
            }
        }
        else if (strcmp(args[0], "cd") == 0) {
            if (arg_count > 1) {
                change_directory(args[1], &g_game_state);
            } else {
                display_error("Sử dụng: cd <thư mục>");
            }
        }
        else if (strcmp(args[0], "open") == 0) {
            if (arg_count > 1) {
                open_file(args[1], &g_game_state);
                
                // Check if game over
                if (is_game_over(&g_game_state)) {
                    g_game_state.state = GAME_STATE_GAME_OVER;
                }
            } else {
                display_error("Sử dụng: open <file>");
            }
        }
    }
    else if (strcmp(args[0], "hack") == 0 || strcmp(args[0], "scan") == 0 || 
             strcmp(args[0], "kill") == 0 || strcmp(args[0], "create_virus") == 0) {
        // Trap commands
        process_trap_command(command, &g_game_state);
    }
    else if (strcmp(args[0], "save") == 0 || strcmp(args[0], "load") == 0 || 
             strcmp(args[0], "ranking") == 0 || strcmp(args[0], "export") == 0) {
        // Storage commands
        process_storage_command(command, &g_game_state, &g_ranking);
    }
    else if (strcmp(args[0], "help") == 0) {
        show_help();
    }
    else if (strcmp(args[0], "quit") == 0) {
        g_running = 0;
    }
    else {
        display_error("Lệnh không hợp lệ. Gõ 'help' để xem danh sách lệnh.");
    }
}

void create_test_files(void) {
    // Create test treasure file
    FILE *fp = fopen("data/treasures/treasure_1.txt", "w");
    if (fp) {
        fprintf(fp, "TREASURE\nChúc mừng! Bạn đã tìm thấy kho báu!\n");
        fclose(fp);
    }
    
    // Create test trap file
    fp = fopen("data/treasures/trap_file.txt", "w");
    if (fp) {
        fprintf(fp, "TRAP\nCẩn thận! Đây là bẫy!\n");
        fclose(fp);
    }
    
    // Create hidden file
    fp = fopen("data/treasures/hidden_gem.enc", "w");
    if (fp) {
        fprintf(fp, "Đây là file ẩn\n");
        fclose(fp);
    }
    
    // Create normal file
    fp = fopen("data/normal.txt", "w");
    if (fp) {
        fprintf(fp, "Đây là file bình thường\n");
        fclose(fp);
    }
    
    // Create directory
    mkdir("data/test_dir", 0755);
}

void show_main_menu(void) {
    display_main_menu(&g_ui_state, &g_game_state);
    
    int ch = getchar();
    switch (ch) {
        case '1':
            start_new_game();
            break;
        case '2':
            show_ranking();
            break;
        case '3':
            show_help();
            break;
        case '4':
        case 'q':
        case 'Q':
            g_running = 0;
            break;
    }
}

void show_ranking(void) {
    display_ranking(&g_ui_state, &g_ranking);
    g_game_state.state = GAME_STATE_MENU;
}

void show_help(void) {
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
    g_game_state.state = GAME_STATE_MENU;
}

void start_new_game(void) {
    // Reset game state
    reset_game_state(&g_game_state);
    
    display_success("Game mới đã bắt đầu!");
}

void load_game(void) {
    if (load_game_state(&g_game_state, "data/game_save.dat") == SUCCESS) {
        display_success("Game đã được tải!");
        g_game_state.state = GAME_STATE_PLAYING;
    } else {
        display_error("Không thể tải game!");
    }
}

void save_game(void) {
    if (save_game_state(&g_game_state, "data/game_save.dat") == SUCCESS) {
        display_success("Game đã được lưu!");
    } else {
        display_error("Không thể lưu game!");
    }
}

int main(void) {
    printf("Đang khởi tạo File System Adventure Game...\n");
    
    init_game();
    
    printf("Game đã sẵn sàng!\n");
    sleep(1);
    
    game_loop();
    
    cleanup_game();
    
    printf("Cảm ơn bạn đã chơi!\n");
    return 0;
}