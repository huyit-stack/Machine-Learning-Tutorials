#ifndef GAME_STRUCTS_H
#define GAME_STRUCTS_H

#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

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

// File information structure
typedef struct {
    char name[64];
    char path[256];
    off_t size;
    mode_t permissions;
    FileType type;
    int is_directory;
    int is_hidden;
} FileInfo;

// Player record structure
typedef struct {
    char name[32];
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
    char current_dir[256];
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
    PlayerRecord records[100];
    int count;
} RankingSystem;

// UI state structure
typedef struct {
    int screen_height;
    int screen_width;
    int current_menu;
    int selected_item;
    char input_buffer[128];
    int input_pos;
} UIState;

// Virus process structure
typedef struct {
    int pid;
    char name[32];
    int level;
    time_t created_time;
} VirusProcess;

#endif // GAME_STRUCTS_H