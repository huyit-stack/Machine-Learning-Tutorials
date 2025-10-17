#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

// Màu sắc cho terminal
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

// Cấu trúc User
typedef struct {
    char username[50];
    char password[50];
    char fullname[100];
    char email[100];
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
    
    while ((ch = getchar()) != '\n' && i < 49) {
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
    printf("║" BOLD WHITE "                    UBUNTU LOGIN SYSTEM                    " CYAN "║\n");
    printf("║" WHITE "                    Chào mừng đến với hệ thống                " CYAN "║\n");
    printf("║" WHITE "                    đăng nhập Ubuntu C                       " CYAN "║\n");
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
    printf("│" RED "  3. " WHITE "Thoát (Exit)                                      " BLUE "│\n");
    printf("└─────────────────────────────────────────────────────────────┘\n" RESET);
    printf("\n");
}

// Hàm đọc file users
int readUsers(User users[], int *count) {
    FILE *file = fopen("users.dat", "rb");
    if (file == NULL) {
        *count = 0;
        return 0;
    }
    
    *count = fread(users, sizeof(User), 100, file);
    fclose(file);
    return 1;
}

// Hàm ghi file users
int writeUsers(User users[], int count) {
    FILE *file = fopen("users.dat", "wb");
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
    char username[50];
    char password[50];
    int userIndex;
    
    printf(BOLD YELLOW "╔══════════════════════════════════════════════════════════════╗\n");
    printf("║" WHITE "                        ĐĂNG NHẬP                            " YELLOW "║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n" RESET);
    printf("\n");
    
    printf("Username: ");
    scanf("%s", username);
    
    hidePassword(password);
    
    userIndex = findUser(users, count, username);
    if (userIndex != -1 && strcmp(users[userIndex].password, password) == 0) {
        printf("\n" GREEN "✓ Đăng nhập thành công!\n" RESET);
        printf(CYAN "Chào mừng %s!\n" RESET, users[userIndex].fullname);
        printf("Email: %s\n", users[userIndex].email);
        
        // Hiển thị thông tin hệ thống
        printf("\n" BOLD BLUE "╔══════════════════════════════════════════════════════════════╗\n");
        printf("║" WHITE "                    THÔNG TIN HỆ THỐNG                    " BLUE "║\n");
        printf("╚══════════════════════════════════════════════════════════════╝\n" RESET);
        
        time_t now = time(0);
        printf("Thời gian đăng nhập: %s", ctime(&now));
        printf("Hệ điều hành: Ubuntu Linux\n");
        printf("Phiên bản chương trình: 1.0\n");
        
        printf("\n" GREEN "Nhấn Enter để tiếp tục..." RESET);
        getchar();
        getchar();
        return 1;
    } else {
        printf("\n" RED "✗ Tên đăng nhập hoặc mật khẩu không đúng!\n" RESET);
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
    scanf("%s", newUser.username);
    
    // Kiểm tra username đã tồn tại chưa
    if (findUser(users, *count, newUser.username) != -1) {
        printf(RED "✗ Username đã tồn tại!\n" RESET);
        printf("Nhấn Enter để thử lại...");
        getchar();
        getchar();
        return 0;
    }
    
    printf("Họ và tên: ");
    getchar(); // Xóa ký tự newline còn lại
    fgets(newUser.fullname, sizeof(newUser.fullname), stdin);
    newUser.fullname[strcspn(newUser.fullname, "\n")] = 0; // Xóa newline
    
    printf("Email: ");
    scanf("%s", newUser.email);
    
    hidePassword(newUser.password);
    
    // Thêm user mới vào mảng
    users[*count] = newUser;
    (*count)++;
    
    // Lưu vào file
    if (writeUsers(users, *count)) {
        printf("\n" GREEN "✓ Đăng ký thành công!\n" RESET);
        printf("Bạn có thể đăng nhập ngay bây giờ.\n");
    } else {
        printf("\n" RED "✗ Lỗi khi lưu thông tin!\n" RESET);
        (*count)--; // Hoàn tác việc tăng count
    }
    
    printf("Nhấn Enter để tiếp tục...");
    getchar();
    getchar();
    return 1;
}

// Hàm hiển thị thông tin tác giả
void showAbout() {
    printf(BOLD CYAN "╔══════════════════════════════════════════════════════════════╗\n");
    printf("║" WHITE "                        THÔNG TIN                            " CYAN "║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n" RESET);
    printf("\n");
    printf("Chương trình: Ubuntu Login System\n");
    printf("Ngôn ngữ: C\n");
    printf("Hệ điều hành: Ubuntu Linux\n");
    printf("Phiên bản: 1.0\n");
    printf("Mô tả: Hệ thống đăng nhập đơn giản với giao diện đẹp\n");
    printf("\n");
}

// Hàm main
int main() {
    User users[100];
    int userCount = 0;
    int choice;
    
    // Đọc dữ liệu user từ file
    readUsers(users, &userCount);
    
    while (1) {
        showBanner();
        showMainMenu();
        
        printf("Chọn chức năng (1-3): ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                if (login(users, userCount)) {
                    // Đăng nhập thành công, có thể thêm logic tiếp theo
                }
                break;
            case 2:
                registerUser(users, &userCount);
                break;
            case 3:
                printf("\n" YELLOW "Cảm ơn bạn đã sử dụng chương trình!\n" RESET);
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