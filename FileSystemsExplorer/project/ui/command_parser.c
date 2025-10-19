#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "command_parser.h"
#include "../filesystem/file_manager.h"
#include "../filesystem/directory_ops.h"
#include "../filesystem/file_ops.h"
#include "../traps/traps.h"
#include "../traps/virus_manager.h"
#include "../../include/constants.h"

static const char* skip_ws(const char* s) {
    while (s && *s && isspace((unsigned char)*s)) ++s;
    return s;
}

ErrorCode cmd_parse_and_execute(GameState* state, const char* line) {
    if (!state || !line) return EC_INVALID;
    const char* s = skip_ws(line);
    if (*s == '\0') return EC_INVALID;

    if (strncmp(s, "ls", 2) == 0 && (s[2] == '\0' || isspace((unsigned char)s[2]))) {
        const char* arg = skip_ws(s + 2);
        return dir_list(*arg ? arg : ".");
    }

    if (strncmp(s, "cd", 2) == 0 && isspace((unsigned char)s[2])) {
        const char* path = skip_ws(s + 2);
        return fm_change_directory(state, *path ? path : ".");
    }

    if (strncmp(s, "open", 4) == 0 && isspace((unsigned char)s[4])) {
        const char* path = skip_ws(s + 4);
        bool isTreasure = false, isTrap = false;
        ErrorCode ec = fo_open_and_inspect(state, path, &isTreasure, &isTrap);
        if (ec == EC_OK) {
            if (isTreasure) printf("You found a treasure! +%d points\n", TREASURE_SCORE);
            if (isTrap) printf("Oh no! It's a trap. Lives -%d\n", TRAP_LIFE_PENALTY);
            bool trapTriggered = false;
            traps_handle(state, path, &trapTriggered);
        }
        return ec;
    }

    if (strncmp(s, "virus start", 11) == 0) {
        return virus_start(state, "[virus] demo process running...");
    }
    if (strncmp(s, "virus stop", 10) == 0) {
        return virus_stop(state);
    }

    if (strncmp(s, "pwd", 3) == 0) {
        char buf[4096];
        ErrorCode ec = fm_get_current_directory(state, buf, sizeof(buf));
        if (ec == EC_OK) {
            printf("%s\n", buf);
        }
        return ec;
    }

    if (strncmp(s, "help", 4) == 0) {
        printf("Commands: ls [path], cd <path>, open <file>, pwd, virus start, virus stop, help, quit\n");
        return EC_OK;
    }

    if (strncmp(s, "quit", 4) == 0) {
        return EC_BUSY; // signal to main to exit
    }

    return EC_INVALID;
}
