#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "ranking_system.h"
#include "../../include/constants.h"

ErrorCode ranking_add_record(const PlayerRecord* rec) {
    if (!rec) return EC_INVALID;
    FILE* f = fopen(RANKING_FILE, "ab");
    if (!f) return EC_ERR;
    fwrite(rec, sizeof(*rec), 1, f);
    fclose(f);
    return EC_OK;
}

static int cmp_records_desc(const void* a, const void* b) {
    const PlayerRecord* ra = (const PlayerRecord*)a;
    const PlayerRecord* rb = (const PlayerRecord*)b;
    return rb->score - ra->score;
}

ErrorCode ranking_print_top(int topN) {
    FILE* f = fopen(RANKING_FILE, "rb");
    if (!f) {
        puts("No ranking records yet.");
        return EC_OK;
    }
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    long n = sz / (long)sizeof(PlayerRecord);
    if (n <= 0) {
        fclose(f);
        puts("No ranking records yet.");
        return EC_OK;
    }
    rewind(f);
    PlayerRecord* arr = (PlayerRecord*)malloc((size_t)n * sizeof(PlayerRecord));
    if (!arr) { fclose(f); return EC_ERR; }
    fread(arr, sizeof(PlayerRecord), (size_t)n, f);
    fclose(f);
    qsort(arr, (size_t)n, sizeof(PlayerRecord), cmp_records_desc);

    if (topN <= 0 || topN > n) topN = (int)n;
    for (int i = 0; i < topN; ++i) {
        char ts[32];
        struct tm* tm = localtime(&arr[i].timestamp);
        if (tm) strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M", tm); else snprintf(ts, sizeof(ts), "-");
        printf("%2d. %-20s %6d  %s\n", i + 1, arr[i].name, arr[i].score, ts);
    }
    free(arr);
    return EC_OK;
}
