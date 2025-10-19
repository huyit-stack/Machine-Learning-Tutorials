#ifndef RANKING_SYSTEM_H
#define RANKING_SYSTEM_H

#include "../../include/error_codes.h"
#include "../../include/game_structs.h"

ErrorCode ranking_add_record(const PlayerRecord* rec);
ErrorCode ranking_print_top(int topN);

#endif // RANKING_SYSTEM_H
