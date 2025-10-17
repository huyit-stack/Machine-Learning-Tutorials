#include "hack_system.h"
#include "../../include/utils.h"
#include <sys/stat.h>
#include <unistd.h>

int hack_file_permissions(const char *filepath, mode_t new_permissions, GameState_t *game_state) {
    // Check if user is admin
    if (!is_admin_user(game_state)) {
        display_error("Chỉ Admin mới có thể hack file permissions");
        return ERROR_ADMIN_REQUIRED;
    }
    
    // Check hack limit
    if (is_hack_limit_exceeded(game_state)) {
        display_error("Đã vượt quá giới hạn hack cho level này");
        return ERROR_LIMIT_EXCEEDED;
    }
    
    // Check if file exists
    struct stat file_stat;
    if (stat(filepath, &file_stat) == -1) {
        display_error(ERR_MSG_FILE_NOT_FOUND);
        return ERROR_FILE_NOT_FOUND;
    }
    
    // Attempt to change permissions
    if (chmod(filepath, new_permissions) == -1) {
        display_error("Không thể thay đổi quyền file");
        return ERROR_SYSTEM_CALL;
    }
    
    // Update game state
    update_hack_count(game_state);
    game_state->score += 50;
    
    display_success(MSG_HACK_SUCCESS);
    printf("Đã hack file: %s\n", filepath);
    
    return SUCCESS;
}

int is_admin_user(const GameState_t *game_state) {
    if (game_state == NULL) {
        return getuid() == 0; // Check if running as root
    }
    return game_state->role == ROLE_ADMIN;
}

int update_hack_count(GameState_t *game_state) {
    if (game_state == NULL) {
        return ERROR_INVALID_INPUT;
    }
    
    game_state->hack_count++;
    return SUCCESS;
}

int is_hack_limit_exceeded(const GameState_t *game_state) {
    if (game_state == NULL) {
        return 0;
    }
    
    return game_state->hack_count >= MAX_HACK_ATTEMPTS;
}

void reset_hack_count(GameState_t *game_state) {
    if (game_state != NULL) {
        game_state->hack_count = 0;
    }
}

int get_remaining_hack_attempts(const GameState_t *game_state) {
    if (game_state == NULL) {
        return 0;
    }
    
    return MAX_HACK_ATTEMPTS - game_state->hack_count;
}