#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>

// Cấu trúc để lưu thông tin user
typedef struct {
    char username[50];
    char password[50];
    int is_admin;
} User;

// Danh sách users (trong thực tế nên đọc từ file hoặc database)
User users[] = {
    {"admin", "admin123", 1},
    {"user1", "password1", 0},
    {"user2", "password2", 0},
    {"guest", "guest", 0}
};
int num_users = 4;

// Hàm để ẩn password khi nhập
void hidePassword(char *password) {
    struct termios old_termios, new_termios;
    int i = 0;
    char ch;
    
    // Lưu cài đặt terminal hiện tại
    tcgetattr(STDIN_FILENO, &old_termios);
    new_termios = old_termios;
    
    // Tắt echo
    new_termios.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
    
    // Nhập password
    while ((ch = getchar()) != '\n' && i < 49) {
        if (ch == 127 || ch == 8) { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password[i] = ch;
            i++;
            printf("*");
        }
    }
    password[i] = '\0';
    
    // Khôi phục cài đặt terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
    printf("\n");
}

// Hàm vẽ khung đẹp
void drawBox(int width, int height) {
    int i, j;
    
    // Vẽ góc trên trái
    printf("┌");
    for (i = 0; i < width - 2; i++) printf("─");
    printf("┐\n");
    
    // Vẽ các dòng giữa
    for (i = 0; i < height - 2; i++) {
        printf("│");
        for (j = 0; j < width - 2; j++) printf(" ");
        printf("│\n");
    }
    
    // Vẽ góc dưới phải
    printf("└");
    for (i = 0; i < width - 2; i++) printf("─");
    printf("┘\n");
}

// Hàm hiển thị tiêu đề
void showTitle() {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    HỆ THỐNG ĐĂNG NHẬP                        ║\n");
    printf("║                      UBUNTU LOGIN                            ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

// Hàm xác thực user
User* authenticateUser(const char* username, const char* password) {
    int i;
    for (i = 0; i < num_users; i++) {
        if (strcmp(users[i].username, username) == 0 && 
            strcmp(users[i].password, password) == 0) {
            return &users[i];
        }
    }
    return NULL;
}

// Hàm hiển thị menu chính
void showMainMenu(User* user) {
    int choice;
    
    while (1) {
        system("clear");
        showTitle();
        
        printf("╔══════════════════════════════════════════════════════════════╗\n");
        printf("║                    MENU CHÍNH                               ║\n");
        printf("╠══════════════════════════════════════════════════════════════╣\n");
        printf("║  Chào mừng: %-45s ║\n", user->username);
        printf("║  Quyền: %-50s ║\n", user->is_admin ? "Administrator" : "User");
        printf("╠══════════════════════════════════════════════════════════════╣\n");
        printf("║  1. Xem thông tin cá nhân                                  ║\n");
        printf("║  2. Đổi mật khẩu                                           ║\n");
        if (user->is_admin) {
            printf("║  3. Quản lý người dùng                                    ║\n");
            printf("║  4. Xem log hệ thống                                      ║\n");
        }
        printf("║  0. Đăng xuất                                               ║\n");
        printf("╚══════════════════════════════════════════════════════════════╝\n");
        printf("\nNhập lựa chọn: ");
        
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                showUserInfo(user);
                break;
            case 2:
                changePassword(user);
                break;
            case 3:
                if (user->is_admin) {
                    manageUsers();
                } else {
                    printf("Bạn không có quyền truy cập chức năng này!\n");
                    getchar(); getchar();
                }
                break;
            case 4:
                if (user->is_admin) {
                    showSystemLog();
                } else {
                    printf("Bạn không có quyền truy cập chức năng này!\n");
                    getchar(); getchar();
                }
                break;
            case 0:
                printf("Đang đăng xuất...\n");
                return;
            default:
                printf("Lựa chọn không hợp lệ!\n");
                getchar(); getchar();
        }
    }
}

// Hàm hiển thị thông tin user
void showUserInfo(User* user) {
    system("clear");
    showTitle();
    
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    THÔNG TIN CÁ NHÂN                        ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║  Username: %-45s ║\n", user->username);
    printf("║  Quyền: %-50s ║\n", user->is_admin ? "Administrator" : "User");
    printf("║  Trạng thái: Hoạt động                                        ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    
    printf("\nNhấn Enter để quay lại...");
    getchar(); getchar();
}

// Hàm đổi mật khẩu
void changePassword(User* user) {
    char old_password[50], new_password[50], confirm_password[50];
    
    system("clear");
    showTitle();
    
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    ĐỔI MẬT KHẨU                             ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    
    printf("\nNhập mật khẩu hiện tại: ");
    hidePassword(old_password);
    
    if (strcmp(old_password, user->password) != 0) {
        printf("Mật khẩu hiện tại không đúng!\n");
        printf("Nhấn Enter để quay lại...");
        getchar();
        return;
    }
    
    printf("Nhập mật khẩu mới: ");
    hidePassword(new_password);
    
    if (strlen(new_password) < 6) {
        printf("Mật khẩu phải có ít nhất 6 ký tự!\n");
        printf("Nhấn Enter để quay lại...");
        getchar();
        return;
    }
    
    printf("Xác nhận mật khẩu mới: ");
    hidePassword(confirm_password);
    
    if (strcmp(new_password, confirm_password) != 0) {
        printf("Mật khẩu xác nhận không khớp!\n");
        printf("Nhấn Enter để quay lại...");
        getchar();
        return;
    }
    
    strcpy(user->password, new_password);
    printf("Đổi mật khẩu thành công!\n");
    printf("Nhấn Enter để quay lại...");
    getchar();
}

// Hàm quản lý users (chỉ admin)
void manageUsers() {
    system("clear");
    showTitle();
    
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    QUẢN LÝ NGƯỜI DÙNG                        ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║  %-15s %-20s %-15s ║\n", "Username", "Password", "Role");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    
    int i;
    for (i = 0; i < num_users; i++) {
        printf("║  %-15s %-20s %-15s ║\n", 
               users[i].username, 
               "********", 
               users[i].is_admin ? "Admin" : "User");
    }
    
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    
    printf("\nNhấn Enter để quay lại...");
    getchar(); getchar();
}

// Hàm xem log hệ thống (chỉ admin)
void showSystemLog() {
    system("clear");
    showTitle();
    
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    LOG HỆ THỐNG                             ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║  [2024-01-01 10:00:00] Hệ thống khởi động                   ║\n");
    printf("║  [2024-01-01 10:05:00] User admin đăng nhập                 ║\n");
    printf("║  [2024-01-01 10:10:00] User user1 đăng nhập                 ║\n");
    printf("║  [2024-01-01 10:15:00] User admin đăng xuất                 ║\n");
    printf("║  [2024-01-01 10:20:00] User user2 đăng nhập                 ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    
    printf("\nNhấn Enter để quay lại...");
    getchar(); getchar();
}

// Hàm chính
int main() {
    char username[50], password[50];
    User* authenticated_user = NULL;
    int attempts = 0;
    const int max_attempts = 3;
    
    while (attempts < max_attempts) {
        system("clear");
        showTitle();
        
        printf("╔══════════════════════════════════════════════════════════════╗\n");
        printf("║                    ĐĂNG NHẬP HỆ THỐNG                      ║\n");
        printf("╚══════════════════════════════════════════════════════════════╝\n");
        
        if (attempts > 0) {
            printf("\n⚠️  Đăng nhập thất bại! Số lần thử còn lại: %d\n\n", max_attempts - attempts);
        }
        
        printf("Username: ");
        scanf("%s", username);
        
        printf("Password: ");
        hidePassword(password);
        
        authenticated_user = authenticateUser(username, password);
        
        if (authenticated_user != NULL) {
            printf("\n✅ Đăng nhập thành công!\n");
            sleep(1);
            showMainMenu(authenticated_user);
            break;
        } else {
            attempts++;
            if (attempts < max_attempts) {
                printf("\n❌ Tên đăng nhập hoặc mật khẩu không đúng!\n");
                printf("Nhấn Enter để thử lại...");
                getchar(); getchar();
            }
        }
    }
    
    if (attempts >= max_attempts) {
        system("clear");
        showTitle();
        printf("╔══════════════════════════════════════════════════════════════╗\n");
        printf("║                    ĐĂNG NHẬP THẤT BẠI                      ║\n");
        printf("║              Bạn đã vượt quá số lần thử cho phép!          ║\n");
        printf("║                    Hệ thống sẽ thoát...                     ║\n");
        printf("╚══════════════════════════════════════════════════════════════╝\n");
        printf("\n");
    }
    
    printf("Cảm ơn bạn đã sử dụng hệ thống!\n");
    return 0;
}