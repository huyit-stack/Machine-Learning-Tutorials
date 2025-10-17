#include "game_config.h"
#include "file_manager.h"
#include "trap_manager.h"
#include "ui_manager.h"
#include "storage_manager.h"
#include <signal.h>
#include <sys/wait.h>

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
    memset(&g_game_state, 0, sizeof(g_game_state));
    g_game_state.lives = 3;
    g_game_state.level = 1;
    g_game_state.role = (getuid() == 0) ? ROLE_ADMIN : ROLE_USER;
    g_game_state.state = GAME_STATE_MENU;
    g_game_state.start_time = time(NULL);
    
    // Get current directory
    if (getcwd(g_game_state.current_dir, sizeof(g_game_state.current_dir)) == NULL) {
        strcpy(g_game_state.current_dir, "/tmp");
    }
    
    // Initialize ranking system
    init_ranking(&g_ranking);
    load_ranking(&g_ranking, "ranking.dat");
    
    // Initialize UI
    if (init_ui(&g_ui_state) == -1) {
        fprintf(stderr, "Không thể khởi tạo UI\n");
        exit(1);
    }
    
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
    save_ranking(&g_ranking, "ranking.dat");
    
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
                if (get_user_input(&g_ui_state) == 0) {
                    strcpy(command, g_ui_state.input_buffer);
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
                    strcpy(record.name, "Player");
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
    
    if (strcmp(args[0], "ls") == 0) {
        FileInfo files[MAX_RECORDS];
        int file_count = 0;
        if (list_files(files, &file_count) == 0) {
            display_file_list(files, file_count, 0, 20);
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
            if (g_game_state.lives <= 0) {
                g_game_state.state = GAME_STATE_GAME_OVER;
            }
        } else {
            display_error("Sử dụng: open <file>");
        }
    }
    else if (strcmp(args[0], "hack") == 0) {
        if (arg_count > 1) {
            // Set permissions to 777 for demo
            hack_file_permissions(args[1], 0777, &g_game_state);
        } else {
            display_error("Sử dụng: hack <file>");
        }
    }
    else if (strcmp(args[0], "scan") == 0) {
        VirusProcess viruses[MAX_VIRUSES];
        int virus_count = 0;
        scan_viruses(viruses, &virus_count);
        
        if (virus_count > 0) {
            char warning_msg[256];
            snprintf(warning_msg, sizeof(warning_msg), "Tìm thấy %d virus:", virus_count);
            display_warning(warning_msg);
            for (int i = 0; i < virus_count; i++) {
                printf("  PID: %d, Tên: %s, Level: %d\n", 
                       viruses[i].pid, viruses[i].name, viruses[i].level);
            }
        }
    }
    else if (strcmp(args[0], "kill") == 0) {
        if (arg_count > 1) {
            pid_t pid = atoi(args[1]);
            kill_virus(pid, &g_game_state);
        } else {
            display_error("Sử dụng: kill <pid>");
        }
    }
    else if (strcmp(args[0], "create_virus") == 0) {
        if (arg_count > 1) {
            int level = atoi(args[1]);
            pid_t pid = create_test_virus("TestVirus", level);
            if (pid > 0) {
                char success_msg[256];
                snprintf(success_msg, sizeof(success_msg), "Đã tạo virus test với PID: %d", pid);
                display_success(success_msg);
            }
        } else {
            display_error("Sử dụng: create_virus <level>");
        }
    }
    else if (strcmp(args[0], "quit") == 0) {
        g_running = 0;
    }
    else if (strcmp(args[0], "help") == 0) {
        show_help();
    }
    else {
        display_error("Lệnh không hợp lệ. Gõ 'help' để xem danh sách lệnh.");
    }
}

void create_test_files(void) {
    // Create test treasure file
    FILE *fp = fopen("treasure.txt", "w");
    if (fp) {
        fprintf(fp, "TREASURE\nChúc mừng! Bạn đã tìm thấy kho báu!\n");
        fclose(fp);
    }
    
    // Create test trap file
    fp = fopen("trap.txt", "w");
    if (fp) {
        fprintf(fp, "TRAP\nCẩn thận! Đây là bẫy!\n");
        fclose(fp);
    }
    
    // Create hidden file
    fp = fopen(".hidden.txt", "w");
    if (fp) {
        fprintf(fp, "Đây là file ẩn\n");
        fclose(fp);
    }
    
    // Create normal file
    fp = fopen("normal.txt", "w");
    if (fp) {
        fprintf(fp, "Đây là file bình thường\n");
        fclose(fp);
    }
    
    // Create directory
    mkdir("test_dir", 0755);
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
        case 'w':
        case 'W':
            if (g_ui_state.selected_item > 0) {
                g_ui_state.selected_item--;
            }
            break;
        case 's':
        case 'S':
            if (g_ui_state.selected_item < 3) {
                g_ui_state.selected_item++;
            }
            break;
        case '\n':
        case '\r':
            switch (g_ui_state.selected_item) {
                case 0: start_new_game(); break;
                case 1: show_ranking(); break;
                case 2: show_help(); break;
                case 3: g_running = 0; break;
            }
            break;
    }
}

void show_ranking(void) {
    display_ranking(&g_ui_state, &g_ranking);
    getchar();
    g_game_state.state = GAME_STATE_MENU;
}

void show_help(void) {
    display_help(&g_ui_state);
    getchar();
    g_game_state.state = GAME_STATE_MENU;
}

void start_new_game(void) {
    // Reset game state
    g_game_state.treasures_found = 0;
    g_game_state.traps_hit = 0;
    g_game_state.viruses_killed = 0;
    g_game_state.hack_count = 0;
    g_game_state.levels_completed = 0;
    g_game_state.score = 0;
    g_game_state.lives = 3;
    g_game_state.level = 1;
    g_game_state.start_time = time(NULL);
    g_game_state.state = GAME_STATE_PLAYING;
    
    // Reset hack count for new level
    reset_hack_count(&g_game_state);
    
    display_success("Game mới đã bắt đầu!");
}

void load_game(void) {
    if (load_game_state(&g_game_state, "savegame.dat") == 0) {
        display_success("Game đã được tải!");
        g_game_state.state = GAME_STATE_PLAYING;
    } else {
        display_error("Không thể tải game!");
    }
}

void save_game(void) {
    if (save_game_state(&g_game_state, "savegame.dat") == 0) {
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