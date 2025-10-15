#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

// Game constants
#define MAX_PATH_LEN 256
#define MAX_FILENAME_LEN 64
#define MAX_COMMAND_LEN 128
#define MAX_PLAYER_NAME 32
#define MAX_RECORDS 100
#define MAX_VIRUSES 10
#define MAX_HACK_ATTEMPTS 3

// Game roles
typedef enum {
    ROLE_USER = 0,
    ROLE_ADMIN = 1
} PlayerRole;

// File types
typedef enum {
    FILE_TYPE_NORMAL = 0,
    FILE_TYPE_TREASURE = 1,
    FILE_TYPE_TRAP = 2,
    FILE_TYPE_HIDDEN = 3
} FileType;

// Game states
typedef enum {
    GAME_STATE_MENU = 0,
    GAME_STATE_PLAYING = 1,
    GAME_STATE_GAME_OVER = 2,
    GAME_STATE_RANKING = 3
} GameState;

// Player record structure
typedef struct {
    char name[MAX_PLAYER_NAME];
    int score;
    int treasures_found;
    int traps_hit;
    int levels_completed;
    int viruses_killed;
    int hack_count;
    time_t play_time;
    PlayerRole role;
} PlayerRecord;

// Game state structure
typedef struct {
    char current_dir[MAX_PATH_LEN];
    int treasures_found;
    int traps_hit;
    int levels_completed;
    int viruses_killed;
    int hack_count;
    int lives;
    int score;
    time_t start_time;
    time_t play_time;
    PlayerRole role;
    GameState state;
    int level;
} GameState_t;

// Ranking system
typedef struct {
    PlayerRecord records[MAX_RECORDS];
    int count;
} RankingSystem;

// Color codes for terminal
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"
#define COLOR_BOLD    "\033[1m"

// File permissions
#define PERM_READ     (S_IRUSR | S_IRGRP | S_IROTH)
#define PERM_WRITE    (S_IWUSR | S_IWGRP | S_IWOTH)
#define PERM_EXEC     (S_IXUSR | S_IXGRP | S_IXOTH)

// Game messages
#define MSG_WELCOME "Chào mừng đến với File System Adventure Game!"
#define MSG_GAME_OVER "Game Over! Bạn đã hết mạng sống!"
#define MSG_LEVEL_UP "Chúc mừng! Bạn đã lên cấp!"
#define MSG_TREASURE_FOUND "Tìm thấy kho báu! +100 điểm"
#define MSG_TRAP_HIT "Bẫy! -50 điểm và -1 mạng"
#define MSG_VIRUS_KILLED "Virus đã bị tiêu diệt! +200 điểm"
#define MSG_HACK_SUCCESS "Hack thành công! +50 điểm"

#endif // GAME_CONFIG_H