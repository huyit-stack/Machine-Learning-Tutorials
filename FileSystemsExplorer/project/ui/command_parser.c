#include "command_parser.h"
#include "../../include/utils.h"
#include <string.h>

int parse_command(const char *command, char args[][MAX_COMMAND_LEN], int max_args) {
    char command_copy[MAX_COMMAND_LEN];
    safe_strcpy(command_copy, command, sizeof(command_copy));
    
    int arg_count = 0;
    char *token = strtok(command_copy, " \t\n");
    
    while (token != NULL && arg_count < max_args) {
        safe_strcpy(args[arg_count], token, MAX_COMMAND_LEN);
        arg_count++;
        token = strtok(NULL, " \t\n");
    }
    
    return arg_count;
}

int process_filesystem_command(char args[][MAX_COMMAND_LEN], int arg_count, GameState_t *game_state) {
    if (arg_count == 0) {
        return ERROR_INVALID_INPUT;
    }
    
    if (strcmp(args[0], "ls") == 0) {
        // This would call the filesystem module
        display_success("Lệnh ls được xử lý bởi filesystem module");
        return SUCCESS;
    }
    else if (strcmp(args[0], "cd") == 0) {
        if (arg_count > 1) {
            display_success("Lệnh cd được xử lý bởi filesystem module");
            return SUCCESS;
        } else {
            display_error("Sử dụng: cd <thư mục>");
            return ERROR_INVALID_INPUT;
        }
    }
    else if (strcmp(args[0], "open") == 0) {
        if (arg_count > 1) {
            display_success("Lệnh open được xử lý bởi filesystem module");
            return SUCCESS;
        } else {
            display_error("Sử dụng: open <file>");
            return ERROR_INVALID_INPUT;
        }
    }
    else {
        display_error("Lệnh filesystem không hợp lệ");
        return ERROR_INVALID_INPUT;
    }
}

int process_trap_command(char args[][MAX_COMMAND_LEN], int arg_count, GameState_t *game_state) {
    if (arg_count == 0) {
        return ERROR_INVALID_INPUT;
    }
    
    if (strcmp(args[0], "hack") == 0) {
        if (arg_count > 1) {
            display_success("Lệnh hack được xử lý bởi traps module");
            return SUCCESS;
        } else {
            display_error("Sử dụng: hack <file>");
            return ERROR_INVALID_INPUT;
        }
    }
    else if (strcmp(args[0], "scan") == 0) {
        display_success("Lệnh scan được xử lý bởi traps module");
        return SUCCESS;
    }
    else if (strcmp(args[0], "kill") == 0) {
        if (arg_count > 1) {
            display_success("Lệnh kill được xử lý bởi traps module");
            return SUCCESS;
        } else {
            display_error("Sử dụng: kill <pid>");
            return ERROR_INVALID_INPUT;
        }
    }
    else {
        display_error("Lệnh trap không hợp lệ");
        return ERROR_INVALID_INPUT;
    }
}

int process_ui_command(char args[][MAX_COMMAND_LEN], int arg_count, GameState_t *game_state) {
    if (arg_count == 0) {
        return ERROR_INVALID_INPUT;
    }
    
    if (strcmp(args[0], "help") == 0) {
        display_success("Lệnh help được xử lý bởi UI module");
        return SUCCESS;
    }
    else if (strcmp(args[0], "quit") == 0) {
        display_success("Lệnh quit được xử lý bởi UI module");
        return SUCCESS;
    }
    else {
        display_error("Lệnh UI không hợp lệ");
        return ERROR_INVALID_INPUT;
    }
}

int is_valid_command(const char *command) {
    const char *valid_commands[] = {
        "ls", "cd", "open", "hack", "scan", "kill", "help", "quit"
    };
    
    for (int i = 0; i < 8; i++) {
        if (strcmp(command, valid_commands[i]) == 0) {
            return 1;
        }
    }
    
    return 0;
}

const char* get_command_help(const char *command) {
    if (strcmp(command, "ls") == 0) {
        return "Liệt kê files trong thư mục";
    } else if (strcmp(command, "cd") == 0) {
        return "Chuyển thư mục";
    } else if (strcmp(command, "open") == 0) {
        return "Mở file (có thể là kho báu hoặc bẫy)";
    } else if (strcmp(command, "hack") == 0) {
        return "Hack quyền file (Admin only)";
    } else if (strcmp(command, "scan") == 0) {
        return "Scan virus (Admin only)";
    } else if (strcmp(command, "kill") == 0) {
        return "Kill virus (Admin only)";
    } else if (strcmp(command, "help") == 0) {
        return "Hiển thị hướng dẫn";
    } else if (strcmp(command, "quit") == 0) {
        return "Thoát game";
    }
    
    return NULL;
}

void list_commands(void) {
    printf("Các lệnh có sẵn:\n");
    printf("  ls          - Liệt kê files trong thư mục\n");
    printf("  cd <dir>    - Chuyển thư mục\n");
    printf("  open <file> - Mở file (có thể là kho báu hoặc bẫy)\n");
    printf("  hack <file> - Hack quyền file (Admin only)\n");
    printf("  scan        - Scan virus (Admin only)\n");
    printf("  kill <pid>  - Kill virus (Admin only)\n");
    printf("  help        - Hiển thị hướng dẫn\n");
    printf("  quit        - Thoát game\n");
}