#include <stdio.h>
#include "console_ui.h"
#include "../../include/utils.h"

static const char* ASCII_LOGO =
"==============================\n"
"  FileSystems Explorer Game\n"
"==============================\n";

ErrorCode console_print_banner(void) {
    fputs(ASCII_LOGO, stdout);
    return EC_OK;
}

ErrorCode console_prompt(char* buf, size_t buf_sz, const char* prompt) {
    if (!buf || buf_sz == 0) return EC_INVALID;
    fputs(prompt ? prompt : "> ", stdout);
    fflush(stdout);
    if (!fgets(buf, (int)buf_sz, stdin)) return EC_ERR;
    trim_newline(buf);
    return EC_OK;
}
