#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>

#define MAX_NAME_LEN 100
#define MAX_USERNAME_LEN 50
#define MAX_PASSWORD_LEN 100
#define MAX_EMAIL_LEN 100
#define MAX_PHONE_LEN 20

typedef struct Player {
    char fullName[MAX_NAME_LEN + 1];
    int age;
    char username[MAX_USERNAME_LEN + 1];
    char password[MAX_PASSWORD_LEN + 1];
    char email[MAX_EMAIL_LEN + 1];
    char phone[MAX_PHONE_LEN + 1];
} Player;

static void clearScreen(void) {
    /* ANSI escape: clear screen and move cursor to home */
    printf("\033[2J\033[H");
}

static void trimTrailingNewline(char *s) {
    if (s == NULL) return;
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') {
        s[len - 1] = '\0';
    }
}

static int countNonSpace(const char *s) {
    int count = 0;
    for (const char *p = s; p && *p; ++p) {
        if (!isspace((unsigned char)*p)) count++;
    }
    return count;
}

static int isDigits(const char *s) {
    if (s == NULL || *s == '\0') return 0;
    for (const char *p = s; *p; ++p) {
        if (!isdigit((unsigned char)*p)) return 0;
    }
    return 1;
}

static int isValidUsername(const char *s) {
    size_t len = strlen(s);
    if (len < 3 || len > MAX_USERNAME_LEN) return 0;
    for (size_t i = 0; i < len; ++i) {
        char c = s[i];
        if (!(isalnum((unsigned char)c) || c == '_' || c == '.')) {
            return 0;
        }
    }
    return 1;
}

static int isValidEmail(const char *s) {
    /* Simple email check: has one '@', at least one '.' after '@', not starting/ending with '.' and '@' isn't first/last */
    const char *at = strchr(s, '@');
    if (!at) return 0;
    if (at == s) return 0; /* cannot start with '@' */
    if (*(at + 1) == '\0') return 0; /* cannot end with '@' */
    const char *dot = strrchr(at + 1, '.');
    if (!dot) return 0;
    if (dot <= at + 1) return 0;
    if (*(dot + 1) == '\0') return 0; /* cannot end with '.' */
    return 1;
}

static int isValidPhone(const char *s) {
    /* Allow optional leading '+', then 9-15 digits */
    if (s == NULL || *s == '\0') return 0;
    size_t i = 0;
    if (s[0] == '+') i = 1;
    size_t digits = 0;
    for (; s[i]; ++i) {
        if (!isdigit((unsigned char)s[i])) return 0;
        digits++;
    }
    return digits >= 9 && digits <= 15;
}

static int readLine(const char *prompt, char *buffer, size_t size) {
    if (prompt) {
        printf("%s", prompt);
        fflush(stdout);
    }
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }
    trimTrailingNewline(buffer);
    return 1;
}

static int readPasswordHidden(const char *prompt, char *buffer, size_t size) {
    struct termios oldt;
    struct termios newt;

    if (tcgetattr(STDIN_FILENO, &oldt) != 0) {
        return 0;
    }
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) != 0) {
        return 0;
    }

    if (prompt) {
        printf("%s", prompt);
        fflush(stdout);
    }

    char *res = fgets(buffer, (int)size, stdin);

    /* Restore terminal */
    (void)tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    printf("\n");
    if (res == NULL) return 0;
    trimTrailingNewline(buffer);
    return 1;
}

static void drawTitle(void) {
    printf("+------------------------------------------+\n");
    printf("|         ĐĂNG KÝ THÔNG TIN NGƯỜI CHƠI      |\n");
    printf("+------------------------------------------+\n\n");
}

static void promptFullName(Player *player) {
    for (;;) {
        if (!readLine("Họ và tên: ", player->fullName, sizeof(player->fullName))) {
            printf("\nĐọc dữ liệu thất bại. Thử lại.\n");
            continue;
        }
        if (countNonSpace(player->fullName) < 2) {
            printf("- Tên không được để trống và phải có ít nhất 2 ký tự.\n\n");
            continue;
        }
        break;
    }
}

static void promptAge(Player *player) {
    char buf[32];
    for (;;) {
        if (!readLine("Tuổi (1-120): ", buf, sizeof(buf))) {
            printf("\nĐọc dữ liệu thất bại. Thử lại.\n");
            continue;
        }
        if (!isDigits(buf)) {
            printf("- Tuổi phải là số nguyên hợp lệ.\n\n");
            continue;
        }
        long value = strtol(buf, NULL, 10);
        if (value < 1 || value > 120) {
            printf("- Tuổi phải nằm trong khoảng 1 đến 120.\n\n");
            continue;
        }
        player->age = (int)value;
        break;
    }
}

static void promptUsername(Player *player) {
    for (;;) {
        if (!readLine("Tên đăng nhập (3-50, a-z A-Z 0-9 _ .): ", player->username, sizeof(player->username))) {
            printf("\nĐọc dữ liệu thất bại. Thử lại.\n");
            continue;
        }
        if (!isValidUsername(player->username)) {
            printf("- Tên đăng nhập không hợp lệ.\n\n");
            continue;
        }
        break;
    }
}

static void promptPassword(Player *player) {
    char pw1[MAX_PASSWORD_LEN + 1];
    char pw2[MAX_PASSWORD_LEN + 1];
    for (;;) {
        if (!readPasswordHidden("Mật khẩu (>= 6 ký tự): ", pw1, sizeof(pw1))) {
            printf("\nĐọc dữ liệu thất bại. Thử lại.\n");
            continue;
        }
        if (strlen(pw1) < 6) {
            printf("- Mật khẩu quá ngắn.\n\n");
            continue;
        }
        if (!readPasswordHidden("Nhập lại mật khẩu: ", pw2, sizeof(pw2))) {
            printf("\nĐọc dữ liệu thất bại. Thử lại.\n");
            continue;
        }
        if (strcmp(pw1, pw2) != 0) {
            printf("- Mật khẩu không khớp.\n\n");
            continue;
        }
        strncpy(player->password, pw1, sizeof(player->password) - 1);
        player->password[sizeof(player->password) - 1] = '\0';
        break;
    }
}

static void promptEmail(Player *player) {
    for (;;) {
        if (!readLine("Email: ", player->email, sizeof(player->email))) {
            printf("\nĐọc dữ liệu thất bại. Thử lại.\n");
            continue;
        }
        if (!isValidEmail(player->email)) {
            printf("- Email không hợp lệ. Ví dụ hợp lệ: ten@domain.com\n\n");
            continue;
        }
        break;
    }
}

static void promptPhone(Player *player) {
    for (;;) {
        if (!readLine("Số điện thoại (+84 hoặc 0, chỉ chứa số): ", player->phone, sizeof(player->phone))) {
            printf("\nĐọc dữ liệu thất bại. Thử lại.\n");
            continue;
        }
        if (!isValidPhone(player->phone)) {
            printf("- Số điện thoại không hợp lệ (9-15 chữ số, có thể bắt đầu bằng '+').\n\n");
            continue;
        }
        break;
    }
}

static int confirmYesNo(const char *prompt) {
    char buf[8];
    for (;;) {
        if (!readLine(prompt, buf, sizeof(buf))) {
            printf("\nĐọc dữ liệu thất bại. Thử lại.\n");
            continue;
        }
        if (buf[0] == 'y' || buf[0] == 'Y') return 1;
        if (buf[0] == 'n' || buf[0] == 'N') return 0;
        printf("- Vui lòng nhập 'y' hoặc 'n'.\n");
    }
}

static void showSummary(const Player *player) {
    printf("\n+------------------------------------------+\n");
    printf("|            THÔNG TIN ĐÃ NHẬP            |\n");
    printf("+------------------------------------------+\n");
    printf("Họ và tên     : %s\n", player->fullName);
    printf("Tuổi          : %d\n", player->age);
    printf("Tên đăng nhập : %s\n", player->username);
    printf("Email         : %s\n", player->email);
    printf("Số điện thoại : %s\n", player->phone);
    printf("(Mật khẩu đã được thiết lập)\n");
}

int main(void) {
    clearScreen();
    drawTitle();

    printf("Nhập thông tin theo hướng dẫn bên dưới (Ctrl+C để hủy).\n\n");

    Player player;
    memset(&player, 0, sizeof(player));

    promptFullName(&player);
    promptAge(&player);
    promptUsername(&player);
    promptPassword(&player);
    promptEmail(&player);
    promptPhone(&player);

    clearScreen();
    drawTitle();
    showSummary(&player);

    if (confirmYesNo("\nXác nhận đăng ký? (y/n): ")) {
        printf("\n✅ Đăng ký thành công!\n");
    } else {
        printf("\n❌ Đăng ký đã bị hủy.\n");
    }

    printf("\nNhấn Enter để thoát...");
    fflush(stdout);
    char tmp[4];
    (void)fgets(tmp, sizeof(tmp), stdin);

    return 0;
}
