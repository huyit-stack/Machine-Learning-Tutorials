#ifndef CONFIG_H
#define CONFIG_H

// Cấu hình chương trình Ubuntu Login System

// Kích thước buffer
#define MAX_USERNAME_LEN 50
#define MAX_PASSWORD_LEN 50
#define MAX_FULLNAME_LEN 100
#define MAX_EMAIL_LEN 100
#define MAX_USERS 100

// Tên file lưu trữ
#define USERS_FILE "users.dat"

// Phiên bản chương trình
#define VERSION "1.0"
#define PROGRAM_NAME "Ubuntu Login System"

// Màu sắc terminal (có thể tùy chỉnh)
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

// Thông báo hệ thống
#define WELCOME_MSG "Chào mừng đến với hệ thống đăng nhập Ubuntu C"
#define LOGIN_SUCCESS "✓ Đăng nhập thành công!"
#define LOGIN_FAILED "✗ Tên đăng nhập hoặc mật khẩu không đúng!"
#define REGISTER_SUCCESS "✓ Đăng ký thành công!"
#define REGISTER_FAILED "✗ Lỗi khi lưu thông tin!"
#define USERNAME_EXISTS "✗ Username đã tồn tại!"
#define GOODBYE_MSG "Cảm ơn bạn đã sử dụng chương trình!"

#endif // CONFIG_H