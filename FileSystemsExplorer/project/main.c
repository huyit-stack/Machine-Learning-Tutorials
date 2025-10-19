#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>

#include "../include/constants.h"
#include "../include/error_codes.h"
#include "../include/game_structs.h"
#include "../include/utils.h"

#include "filesystem/file_manager.h"
#include "filesystem/directory_ops.h"
#include "filesystem/file_ops.h"

#include "traps/traps.h"
#include "traps/virus_manager.h"

#include "ui/ui.h"
#include "ui/console_ui.h"
#include "ui/command_parser.h"

#include "storage/storage.h"
#include "storage/game_state.h"
#include "storage/ranking_system.h"
#include "storage/save_load.h"

static void show_prompt(void) {
    printf("> ");
    fflush(stdout);
}

int main(int argc, char** argv) {
    (void)argc; (void)argv;

    GameState state;
    game_state_init(&state, "Player");

    // Ensure cwd in state
    char cwd[MAX_PATH_LEN];
    if (getcwd(cwd, sizeof(cwd))) {
        snprintf(state.currentPath, sizeof(state.currentPath), "%s", cwd);
    }

    storage_init();

    console_print_banner();
    ui_render(&state);
    puts("Type 'help' for commands. 'quit' to exit.");

    char line[1024];
    while (true) {
        show_prompt();
        if (!fgets(line, sizeof(line), stdin)) break;
        trim_newline(line);
        ErrorCode ec = cmd_parse_and_execute(&state, line);
        if (ec == EC_BUSY && strncmp(line, "quit", 4) == 0) {
            break;
        } else if (ec == EC_INVALID) {
            puts("Unknown or invalid command. Type 'help'.");
        } else if (ec != EC_OK) {
            printf("Error: %d\n", ec);
        }
        ui_render(&state);
        if (state.lives <= 0) {
            puts("Game over! Out of lives.");
            break;
        }
    }

    // Save ranking record on exit
    PlayerRecord rec;
    memset(&rec, 0, sizeof(rec));
    snprintf(rec.name, sizeof(rec.name), "%s", state.playerName);
    rec.score = state.score;
    rec.timestamp = time(NULL);
    ranking_add_record(&rec);

    storage_shutdown();

    return 0;
}
