#include "traps.h"
#include "hack_system.h"
#include "virus_manager.h"
#include "../../include/utils.h"
#include <string.h>

// Global virus processes array
static VirusProcess virus_processes[MAX_VIRUSES];
static int virus_count = 0;

int init_traps_module(void) {
    virus_count = 0;
    memset(virus_processes, 0, sizeof(virus_processes));
    return SUCCESS;
}

void cleanup_traps_module(void) {
    cleanup_virus_processes();
}

int process_trap_command(const char *command, GameState_t *game_state) {
    char args[10][MAX_COMMAND_LEN];
    int arg_count = 0;
    char command_copy[MAX_COMMAND_LEN];
    
    // Parse command
    safe_strcpy(command_copy, command, sizeof(command_copy));
    char *token = strtok(command_copy, " \t\n");
    
    while (token != NULL && arg_count < 10) {
        safe_strcpy(args[arg_count], token, MAX_COMMAND_LEN);
        arg_count++;
        token = strtok(NULL, " \t\n");
    }
    
    if (arg_count == 0) {
        return ERROR_INVALID_INPUT;
    }
    
    // Process different trap commands
    if (strcmp(args[0], "hack") == 0) {
        if (arg_count > 1) {
            // Set permissions to 777 for demo
            return hack_file_permissions(args[1], 0777, game_state);
        } else {
            display_error("Sử dụng: hack <file>");
            return ERROR_INVALID_INPUT;
        }
    }
    else if (strcmp(args[0], "scan") == 0) {
        VirusProcess viruses[MAX_VIRUSES];
        int virus_count = 0;
        int result = scan_viruses(viruses, &virus_count);
        
        if (result == SUCCESS && virus_count > 0) {
            char warning_msg[256];
            snprintf(warning_msg, sizeof(warning_msg), "Tìm thấy %d virus:", virus_count);
            display_warning(warning_msg);
            for (int i = 0; i < virus_count; i++) {
                printf("  PID: %d, Tên: %s, Level: %d\n", 
                       viruses[i].pid, viruses[i].name, viruses[i].level);
            }
        }
        return result;
    }
    else if (strcmp(args[0], "kill") == 0) {
        if (arg_count > 1) {
            int pid = atoi(args[1]);
            return kill_virus(pid, game_state);
        } else {
            display_error("Sử dụng: kill <pid>");
            return ERROR_INVALID_INPUT;
        }
    }
    else if (strcmp(args[0], "create_virus") == 0) {
        if (arg_count > 1) {
            int level = atoi(args[1]);
            int pid = create_test_virus("TestVirus", level);
            if (pid > 0) {
                char success_msg[256];
                snprintf(success_msg, sizeof(success_msg), "Đã tạo virus test với PID: %d", pid);
                display_success(success_msg);
                return SUCCESS;
            }
            return ERROR_SYSTEM_CALL;
        } else {
            display_error("Sử dụng: create_virus <level>");
            return ERROR_INVALID_INPUT;
        }
    }
    else {
        display_error("Lệnh trap không hợp lệ");
        return ERROR_INVALID_INPUT;
    }
}