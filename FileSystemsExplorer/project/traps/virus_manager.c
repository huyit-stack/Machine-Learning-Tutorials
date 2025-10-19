#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include "virus_manager.h"

ErrorCode virus_start(GameState* state, const char* message) {
    if (!state) return EC_INVALID;
    if (state->virusRunning) return EC_BUSY;

    pid_t pid = fork();
    if (pid < 0) return EC_ERR;
    if (pid == 0) {
        // Child: simple loop that prints and sleeps; safe for demo
        const char* msg = message ? message : "[virus] running...";
        for (int i = 0; i < 10; ++i) {
            fprintf(stderr, "%s (%d)\n", msg, i + 1);
            struct timespec ts; ts.tv_sec = 0; ts.tv_nsec = 200L * 1000L * 1000L; // 200ms
            nanosleep(&ts, NULL);
        }
        _exit(0);
    }

    state->virusPid = pid;
    state->virusRunning = true;
    return EC_OK;
}

ErrorCode virus_stop(GameState* state) {
    if (!state) return EC_INVALID;
    if (!state->virusRunning) return EC_INVALID;
    if (kill(state->virusPid, SIGTERM) != 0) {
        return EC_ERR;
    }
    state->virusRunning = false;
    state->virusPid = 0;
    return EC_OK;
}

bool virus_is_running(const GameState* state) {
    return state && state->virusRunning;
}
