#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <stdbool.h>
#include "../../include/error_codes.h"
#include "../../include/game_structs.h"

// Returns EC_OK if handled, EC_INVALID if unknown command
ErrorCode cmd_parse_and_execute(GameState* state, const char* line);

#endif // COMMAND_PARSER_H
