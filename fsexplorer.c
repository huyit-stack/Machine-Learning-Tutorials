// Enable POSIX prototypes on some libcs
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

static void print_error(const char *context) {
    fprintf(stderr, "Error (%s): %s\n", context, strerror(errno));
}

static char file_type_char(mode_t mode) {
    if (S_ISDIR(mode)) return 'd';
    if (S_ISLNK(mode)) return 'l';
    if (S_ISREG(mode)) return '-';
    if (S_ISCHR(mode)) return 'c';
    if (S_ISBLK(mode)) return 'b';
    if (S_ISFIFO(mode)) return 'p';
#ifdef S_ISSOCK
    if (S_ISSOCK(mode)) return 's';
#endif
    return '?';
}

static struct tm *portable_localtime_r(const time_t *timep, struct tm *out)
{
#if defined(__STDC_LIB_EXT1__) || defined(_POSIX_C_SOURCE)
    // Try to use localtime_r when available
    #if defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 199506L)
    return localtime_r(timep, out);
    #else
    // Fallback path: use localtime and copy result (not thread-safe)
    struct tm *tmp = localtime(timep);
    if (tmp) *out = *tmp;
    return tmp;
    #endif
#else
    struct tm *tmp = localtime(timep);
    if (tmp) *out = *tmp;
    return tmp;
#endif
}

static char *next_token(char **cursor)
{
    if (!cursor || !*cursor) return NULL;
    char *s = *cursor;
    while (*s && isspace((unsigned char)*s)) s++;
    if (*s == '\0') { *cursor = s; return NULL; }
    char *start = s;
    while (*s && !isspace((unsigned char)*s)) s++;
    if (*s) *s++ = '\0';
    *cursor = s;
    return start;
}

static void cmd_pwd(void) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        print_error("getcwd");
        return;
    }
    printf("%s\n", cwd);
}

static void cmd_cd(const char *path) {
    if (path == NULL) {
        fprintf(stderr, "Usage: cd <dir>\n");
        return;
    }
    if (chdir(path) != 0) {
        print_error("chdir");
    }
}

static void cmd_ls(const char *path) {
    const char *dirpath = (path && *path) ? path : ".";
    DIR *dir = opendir(dirpath);
    if (!dir) {
        print_error("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char fullpath[PATH_MAX];
        int n = snprintf(fullpath, sizeof(fullpath), "%s/%s", dirpath, entry->d_name);
        if (n < 0 || (size_t)n >= sizeof(fullpath)) {
            fprintf(stderr, "Path too long: %s/%s\n", dirpath, entry->d_name);
            continue;
        }

        struct stat st;
        if (lstat(fullpath, &st) != 0) {
            print_error("lstat");
            continue;
        }

        char tbuf[32];
        struct tm tm;
        if (portable_localtime_r(&st.st_mtime, &tm) != NULL) {
            strftime(tbuf, sizeof(tbuf), "%Y-%m-%d %H:%M", &tm);
        } else {
            strncpy(tbuf, "???????? ?????", sizeof(tbuf));
            tbuf[sizeof(tbuf)-1] = '\0';
        }

        printf("%c %10ld %s  %s\n",
               file_type_char(st.st_mode),
               (long)st.st_size,
               tbuf,
               entry->d_name);
    }

    closedir(dir);
}

static void cmd_cat(const char *path) {
    if (!path) {
        fprintf(stderr, "Usage: cat <file>\n");
        return;
    }
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        print_error("fopen");
        return;
    }
    char buf[8192];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), fp)) > 0) {
        if (fwrite(buf, 1, n, stdout) != n) {
            print_error("write");
            break;
        }
    }
    if (ferror(fp)) {
        print_error("fread");
    }
    fclose(fp);
}

static void cmd_statpath(const char *path) {
    if (!path) {
        fprintf(stderr, "Usage: stat <path>\n");
        return;
    }
    struct stat st;
    if (lstat(path, &st) != 0) {
        print_error("lstat");
        return;
    }
    char type = file_type_char(st.st_mode);
    char tbuf[64];
    struct tm tm;
    if (portable_localtime_r(&st.st_mtime, &tm) != NULL) {
        strftime(tbuf, sizeof(tbuf), "%Y-%m-%d %H:%M:%S", &tm);
    } else {
        strncpy(tbuf, "???????? ??????", sizeof(tbuf));
        tbuf[sizeof(tbuf)-1] = '\0';
    }

    printf("Path: %s\n", path);
    printf("Type: %c\n", type);
    printf("Size: %ld bytes\n", (long)st.st_size);
    printf("Mode: %o\n", (unsigned int)(st.st_mode & 07777));
    printf("Modified: %s\n", tbuf);
    printf("Links: %lu\n", (unsigned long)st.st_nlink);
}

static void print_help(void) {
    printf("Commands:\n");
    printf("  help                 Show this help\n");
    printf("  pwd                  Print current directory\n");
    printf("  ls [dir]             List directory contents\n");
    printf("  cd <dir>             Change directory\n");
    printf("  cat <file>           Print file contents\n");
    printf("  stat <path>          Show file/directory info\n");
    printf("  exit|quit|q          Exit the program\n");
}

int main(void) {
    setvbuf(stdout, NULL, _IOLBF, 0);

    char line[4096];
    for (;;) {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            strcpy(cwd, "?");
        }
        printf("fse %s> ", cwd);
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            putchar('\n');
            break; // EOF
        }

        // Trim newline
        line[strcspn(line, "\r\n")] = '\0';

        // Skip leading spaces
        char *p = line;
        while (*p && isspace((unsigned char)*p)) p++;
        if (*p == '\0') continue; // empty

        // Tokenize (portable, no strtok_r dependency)
        char *cursor = p;
        char *cmd = next_token(&cursor);
        char *arg1 = next_token(&cursor);

        if (!cmd) continue;

        if (strcmp(cmd, "help") == 0 || strcmp(cmd, "h") == 0) {
            print_help();
        } else if (strcmp(cmd, "pwd") == 0) {
            cmd_pwd();
        } else if (strcmp(cmd, "ls") == 0) {
            cmd_ls(arg1);
        } else if (strcmp(cmd, "cd") == 0) {
            cmd_cd(arg1);
        } else if (strcmp(cmd, "cat") == 0) {
            cmd_cat(arg1);
        } else if (strcmp(cmd, "stat") == 0) {
            cmd_statpath(arg1);
        } else if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0 || strcmp(cmd, "q") == 0) {
            break;
        } else {
            fprintf(stderr, "Unknown command: %s (type 'help')\n", cmd);
        }
    }

    return 0;
}
