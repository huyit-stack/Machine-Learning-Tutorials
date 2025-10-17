#include "game_state.h"
#include "../../include/utils.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>

int calculate_final_score(const GameState_t *game_state, float difficulty_multiplier) {
    if (game_state == NULL) {
        return 0;
    }
    
    int base_score = game_state->score;
    int treasure_bonus = game_state->treasures_found * 100;
    int virus_bonus = game_state->viruses_killed * 200;
    int hack_bonus = game_state->hack_count * 50;
    int trap_penalty = game_state->traps_hit * 50;
    int level_bonus = game_state->levels_completed * 500;
    
    // Time bonus (faster completion = higher bonus)
    time_t play_time = time(NULL) - game_state->start_time;
    int time_bonus = (play_time < 300) ? 1000 : (play_time < 600) ? 500 : 0;
    
    int final_score = (base_score + treasure_bonus + virus_bonus + hack_bonus + 
                      level_bonus + time_bonus - trap_penalty) * difficulty_multiplier;
    
    return (final_score > 0) ? final_score : 0;
}

void update_game_stats(GameState_t *game_state, const char *action, int points) {
    if (game_state == NULL || action == NULL) {
        return;
    }
    
    game_state->score += points;
    
    if (strcmp(action, "treasure") == 0) {
        game_state->treasures_found++;
    } else if (strcmp(action, "trap") == 0) {
        game_state->traps_hit++;
        game_state->lives--;
    } else if (strcmp(action, "virus") == 0) {
        game_state->viruses_killed++;
    } else if (strcmp(action, "hack") == 0) {
        game_state->hack_count++;
    } else if (strcmp(action, "level") == 0) {
        game_state->levels_completed++;
    }
}

void init_game_state(GameState_t *game_state) {
    if (game_state == NULL) {
        return;
    }
    
    memset(game_state, 0, sizeof(GameState_t));
    game_state->lives = 3;
    game_state->level = 1;
    game_state->role = (getuid() == 0) ? ROLE_ADMIN : ROLE_USER;
    game_state->state = GAME_STATE_MENU;
    game_state->start_time = time(NULL);
    
    // Get current directory
    if (getcwd(game_state->current_dir, sizeof(game_state->current_dir)) == NULL) {
        safe_strcpy(game_state->current_dir, "/tmp", sizeof(game_state->current_dir));
    }
}

void reset_game_state(GameState_t *game_state) {
    if (game_state == NULL) {
        return;
    }
    
    game_state->treasures_found = 0;
    game_state->traps_hit = 0;
    game_state->viruses_killed = 0;
    game_state->hack_count = 0;
    game_state->levels_completed = 0;
    game_state->score = 0;
    game_state->lives = 3;
    game_state->level = 1;
    game_state->start_time = time(NULL);
    game_state->state = GAME_STATE_PLAYING;
}

int is_game_over(const GameState_t *game_state) {
    if (game_state == NULL) {
        return 1;
    }
    
    return game_state->lives <= 0;
}

float get_difficulty_multiplier(void) {
    double loadavg[3];
    
    if (getloadavg(loadavg, 3) == -1) {
        return 1.0f; // Default multiplier if can't get load average
    }
    
    // Use 1-minute load average
    double load = loadavg[0];
    
    // Adjust multiplier based on system load
    if (load > 2.0) {
        return 1.5f; // High load = higher difficulty
    } else if (load > 1.0) {
        return 1.2f; // Medium load = slightly higher difficulty
    } else {
        return 1.0f; // Low load = normal difficulty
    }
}

int calculate_level(int score) {
    if (score < 1000) return 1;
    if (score < 5000) return 2;
    if (score < 10000) return 3;
    if (score < 25000) return 4;
    if (score < 50000) return 5;
    return 6; // Max level
}

int check_level_up(GameState_t *game_state) {
    if (game_state == NULL) {
        return 0;
    }
    
    int new_level = calculate_level(game_state->score);
    if (new_level > game_state->level) {
        game_state->level = new_level;
        game_state->levels_completed++;
        return 1; // Level up
    }
    
    return 0; // No level up
}