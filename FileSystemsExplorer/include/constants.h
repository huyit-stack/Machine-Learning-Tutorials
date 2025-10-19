#ifndef CONSTANTS_H
#define CONSTANTS_H

#define PROJECT_NAME "FileSystemsExplorer"
#define PROJECT_VERSION "0.1.0"

// Gameplay constants
#define DEFAULT_LIVES 3
#define TREASURE_SCORE 10
#define TRAP_LIFE_PENALTY 1

// Limits
#define MAX_PLAYER_NAME 64
#define MAX_PATH_LEN 4096
#define FILE_PREVIEW_BYTES 512

// Relative data paths (run the binary from project root)
#define DATA_DIR "data"
#define TREASURES_DIR DATA_DIR "/treasures"
#define CONFIG_DIR DATA_DIR "/config"
#define RANKING_FILE DATA_DIR "/ranking.dat"
#define SAVE_FILE DATA_DIR "/game_save.dat"

#endif // CONSTANTS_H
