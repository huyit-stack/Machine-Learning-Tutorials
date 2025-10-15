#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include "config.h"

// Cấu trúc User sử dụng config
typedef struct {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    char fullname[MAX_FULLNAME_LEN];
    char email[MAX_EMAIL_LEN];
} User;

// Hàm ẩn password khi nhập
void hidePassword(char *password) {
    struct termios old_termios, new_termios;
    int i = 0;
    char ch;
    
    // Lưu cài đặt terminal hiện tại
    tcgetattr(STDIN_FILENO, &old_termios);
    new_termios = old_termios;
    
    // Tắt echo
    new_termios.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
    
    printf("Password: ");
    fflush(stdout);
    
    while ((ch = getchar()) != '\n' && i < MAX_PASSWORD_LEN - 1) {
        if (ch == '\b' || ch == 127) { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b");
                fflush(stdout);
            }
        } else {
            password[i] = ch;
            i++;
            printf("*");
            fflush(stdout);
        }
    }
    password[i] = '\0';
    printf("\n");
    
    // Khôi phục cài đặt terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
}

// Hàm hiển thị banner
void showBanner() {
    system("clear");
    printf("\n");
    printf(CYAN "╔══════════════════════════════════════════════════════════════╗\n");
    printf("║" BOLD WHITE "                    %s                    " CYAN "║\n", PROGRAM_NAME);
    printf("║" WHITE "                    %s                " CYAN "║\n", WELCOME_MSG);
    printf("║" WHITE "                    Phiên bản %s                       " CYAN "║\n", VERSION);
    printf("╚══════════════════════════════════════════════════════════════╝\n" RESET);
    printf("\n");
}

// Hàm hiển thị menu chính
void showMainMenu() {
    printf(BOLD BLUE "┌─────────────────────────────────────────────────────────────┐\n");
    printf("│" WHITE "                         MENU CHÍNH                          " BLUE "│\n");
    printf("├─────────────────────────────────────────────────────────────┤\n");
    printf("│" GREEN "  1. " WHITE "Đăng nhập (Login)                                " BLUE "│\n");
    printf("│" GREEN "  2. " WHITE "Đăng ký (Register)                               " BLUE "│\n");
    printf("│" MAGENTA "  4. " WHITE "Thông tin chương trình (About)                  " BLUE "│\n");
    printf("│" RED "  3. " WHITE "Thoát (Exit)                                      " BLUE "│\n");
    printf("└─────────────────────────────────────────────────────────────┘\n" RESET);
    printf("\n");
}

// Hàm đọc file users
int readUsers(User users[], int *count) {
    FILE *file = fopen(USERS_FILE, "rb");
    if (file == NULL) {
        *count = 0;
        return 0;
    }
    
    *count = fread(users, sizeof(User), MAX_USERS, file);
    fclose(file);
    return 1;
}

// Hàm ghi file users
int writeUsers(User users[], int count) {
    FILE *file = fopen(USERS_FILE, "wb");
    if (file == NULL) {
        return 0;
    }
    
    fwrite(users, sizeof(User), count, file);
    fclose(file);
    return 1;
}

// Hàm tìm user theo username
int findUser(User users[], int count, char *username) {
    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

// Hàm đăng nhập
int login(User users[], int count) {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    int userIndex;
    
    printf(BOLD YELLOW "╔══════════════════════════════════════════════════════════════╗\n");
    printf("║" WHITE "                        ĐĂNG NHẬP                            " YELLOW "║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n" RESET);
    printf("\n");
    
    printf("Username: ");
    if (scanf("%49s", username) != 1) {
        printf(RED "✗ Lỗi nhập liệu!\n" RESET);
        return 0;
    }
    
    hidePassword(password);
    
    userIndex = findUser(users, count, username);
    if (userIndex != -1 && strcmp(users[userIndex].password, password) == 0) {
        printf("\n" GREEN "%s\n" RESET, LOGIN_SUCCESS);
        printf(CYAN "Chào mừng %s!\n" RESET, users[userIndex].fullname);
        printf("Email: %s\n", users[userIndex].email);
        
        // Hiển thị thông tin hệ thống
        printf("\n" BOLD BLUE "╔══════════════════════════════════════════════════════════════╗\n");
        printf("║" WHITE "                    THÔNG TIN HỆ THỐNG                    " BLUE "║\n");
        printf("╚══════════════════════════════════════════════════════════════╝\n" RESET);
        
        time_t now = time(0);
        printf("Thời gian đăng nhập: %s", ctime(&now));
        printf("Hệ điều hành: Ubuntu Linux\n");
        printf("Phiên bản chương trình: %s\n", VERSION);
        printf("Tổng số user trong hệ thống: %d\n", count);
        
        printf("\n" GREEN "Nhấn Enter để tiếp tục..." RESET);
        getchar();
        getchar();
        return 1;
    } else {
        printf("\n" RED "%s\n" RESET, LOGIN_FAILED);
        printf("Nhấn Enter để thử lại...");
        getchar();
        getchar();
        return 0;
    }
}

// Hàm đăng ký
int registerUser(User users[], int *count) {
    User newUser;
    
    printf(BOLD YELLOW "╔══════════════════════════════════════════════════════════════╗\n");
    printf("║" WHITE "                        ĐĂNG KÝ                              " YELLOW "║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n" RESET);
    printf("\n");
    
    printf("Username: ");
    if (scanf("%49s", newUser.username) != 1) {
        printf(RED "✗ Lỗi nhập liệu!\n" RESET);
        return 0;
    }
    
    // Kiểm tra username đã tồn tại chưa
    if (findUser(users, *count, newUser.username) != -1) {
        printf(RED "%s\n" RESET, USERNAME_EXISTS);
        printf("Nhấn Enter để thử lại...");
        getchar();
        getchar();
        return 0;
    }
    
    printf("Họ và tên: ");
    getchar(); // Xóa ký tự newline còn lại
    if (fgets(newUser.fullname, sizeof(newUser.fullname), stdin) == NULL) {
        printf(RED "✗ Lỗi nhập liệu!\n" RESET);
        return 0;
    }
    newUser.fullname[strcspn(newUser.fullname, "\n")] = 0; // Xóa newline
    
    printf("Email: ");
    if (scanf("%99s", newUser.email) != 1) {
        printf(RED "✗ Lỗi nhập liệu!\n" RESET);
        return 0;
    }
    
    hidePassword(newUser.password);
    
    // Thêm user mới vào mảng
    if (*count >= MAX_USERS) {
        printf(RED "✗ Hệ thống đã đầy! Không thể thêm user mới.\n" RESET);
        printf("Nhấn Enter để tiếp tục...");
        getchar();
        getchar();
        return 0;
    }
    
    users[*count] = newUser;
    (*count)++;
    
    // Lưu vào file
    if (writeUsers(users, *count)) {
        printf("\n" GREEN "%s\n" RESET, REGISTER_SUCCESS);
        printf("Bạn có thể đăng nhập ngay bây giờ.\n");
    } else {
        printf("\n" RED "%s\n" RESET, REGISTER_FAILED);
        (*count)--; // Hoàn tác việc tăng count
    }
    
    printf("Nhấn Enter để tiếp tục...");
    getchar();
    getchar();
    return 1;
}

// Hàm hiển thị thông tin chương trình
void showAbout() {
    printf(BOLD CYAN "╔══════════════════════════════════════════════════════════════╗\n");
    printf("║" WHITE "                        THÔNG TIN                            " CYAN "║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n" RESET);
    printf("\n");
    printf("Chương trình: %s\n", PROGRAM_NAME);
    printf("Ngôn ngữ: C\n");
    printf("Hệ điều hành: Ubuntu Linux\n");
    printf("Phiên bản: %s\n", VERSION);
    printf("Mô tả: Hệ thống đăng nhập đơn giản với giao diện đẹp\n");
    printf("Tác giả: Ubuntu C Developer\n");
    printf("Giấy phép: MIT License\n");
    printf("\n");
    printf("Tính năng:\n");
    printf("• Giao diện console đẹp mắt với màu sắc\n");
    printf("• Đăng nhập và đăng ký user\n");
    printf("• Lưu trữ dữ liệu trong file binary\n");
    printf("• Ẩn mật khẩu khi nhập\n");
    printf("• Hiển thị thông tin hệ thống\n");
    printf("\n");
    printf("Nhấn Enter để quay lại menu chính...");
    getchar();
    getchar();
}

// Hàm main
int main() {
    User users[MAX_USERS];
    int userCount = 0;
    int choice;
    
    // Đọc dữ liệu user từ file
    readUsers(users, &userCount);
    
    while (1) {
        showBanner();
        showMainMenu();
        
        printf("Chọn chức năng (1-4): ");
        if (scanf("%d", &choice) != 1) {
            printf(RED "✗ Lựa chọn không hợp lệ!\n" RESET);
            printf("Nhấn Enter để thử lại...");
            getchar();
            getchar();
            continue;
        }
        
        switch (choice) {
            case 1:
                if (login(users, userCount)) {
                    // Đăng nhập thành công, có thể thêm logic tiếp theo
                }
                break;
            case 2:
                registerUser(users, &userCount);
                break;
            case 4:
                showAbout();
                break;
            case 3:
                printf("\n" YELLOW "%s\n" RESET, GOODBYE_MSG);
                printf("Tạm biệt!\n");
                exit(0);
            default:
                printf(RED "✗ Lựa chọn không hợp lệ!\n" RESET);
                printf("Nhấn Enter để thử lại...");
                getchar();
                getchar();
                break;
        }
    }
    
    return 0;
}