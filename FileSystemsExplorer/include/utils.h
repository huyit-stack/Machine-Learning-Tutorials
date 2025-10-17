#ifndef UTILS_H
#define UTILS_H

#include "game_structs.h"
#include "constants.h"
#include "error_codes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

// Utility function declarations
void display_error(const char *message);
void display_success(const char *message);
void display_warning(const char *message);

void format_file_size(off_t size, char *buffer, size_t buffer_size);
void get_permissions_string(mode_t permissions, char *buffer, size_t buffer_size);
int is_hidden_file(const char *filename);

void clear_screen(void);
void refresh_display(void);

// Time utilities
char* format_time(time_t time_val);
time_t get_current_time(void);

// String utilities
int safe_strcpy(char *dest, const char *src, size_t dest_size);
int safe_strcat(char *dest, const char *src, size_t dest_size);
void trim_string(char *str);

// Validation utilities
int is_valid_filename(const char *filename);
int is_valid_path(const char *path);
int is_admin_user(void);

// Memory utilities
void* safe_malloc(size_t size);
void safe_free(void *ptr);

#endif // UTILS_H