#ifndef GAME_STRUCTS_H
#define GAME_STRUCTS_H

#include <stdbool.h>
#include <sys/types.h>
#include <time.h>
#include "constants.h"

typedef struct GameState {
    char currentPath[MAX_PATH_LEN];
    char playerName[MAX_PLAYER_NAME];
    int score;
    int lives;
    int treasuresFound;
    int trapsTriggered;
    bool virusRunning;
    pid_t virusPid;
} GameState;

typedef struct PlayerRecord {
    char name[MAX_PLAYER_NAME];
    int score;
    time_t timestamp;
} PlayerRecord;

#endif // GAME_STRUCTS_H
