#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

#define ARRAY_LEN(a) (sizeof(a) / sizeof((a)[0]))

static inline char* trim_newline(char* s) {
    if (!s) return s;
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[--len] = '\0';
    }
    return s;
}

static inline void path_join(const char* a, const char* b, char* out, size_t out_sz) {
    if (!a || !*a) {
        snprintf(out, out_sz, "%s", b ? b : "");
        return;
    }
    if (!b || !*b) {
        snprintf(out, out_sz, "%s", a);
        return;
    }
    size_t alen = strlen(a);
    bool need_sep = alen > 0 && a[alen - 1] != '/';
    snprintf(out, out_sz, "%s%s%s", a, need_sep ? "/" : "", b);
}

static inline bool str_iequals(const char* a, const char* b) {
    if (!a || !b) return false;
    while (*a && *b) {
        char ca = (char)tolower((unsigned char)*a++);
        char cb = (char)tolower((unsigned char)*b++);
        if (ca != cb) return false;
    }
    return *a == '\0' && *b == '\0';
}

#endif // UTILS_H
