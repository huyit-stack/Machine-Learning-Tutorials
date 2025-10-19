#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include "../../include/error_codes.h"
#include "../../include/game_structs.h"

ErrorCode console_print_banner(void);
ErrorCode console_prompt(char* buf, size_t buf_sz, const char* prompt);

#endif // CONSOLE_UI_H
