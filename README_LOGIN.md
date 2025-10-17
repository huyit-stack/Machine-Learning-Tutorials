# Ubuntu Login System

Một chương trình giao diện đăng nhập đơn giản và đẹp mắt được viết bằng C cho Ubuntu Linux.

## Tính năng

- 🎨 Giao diện console đẹp mắt với màu sắc
- 🔐 Đăng nhập với username/password
- 📝 Đăng ký tài khoản mới
- 💾 Lưu trữ dữ liệu user trong file binary
- 🔒 Ẩn mật khẩu khi nhập
- 📊 Hiển thị thông tin hệ thống sau khi đăng nhập

## Yêu cầu hệ thống

- Ubuntu Linux (hoặc các distro Linux khác)
- GCC compiler
- Make utility

## Cài đặt và chạy

### Cách 1: Sử dụng Makefile (Khuyến nghị)

```bash
# Biên dịch chương trình
make

# Chạy chương trình
make run

# Cài đặt vào hệ thống (tùy chọn)
sudo make install

# Gỡ cài đặt
sudo make uninstall

# Xóa các file build
make clean
```

### Cách 2: Biên dịch thủ công

```bash
# Biên dịch
gcc -Wall -Wextra -std=c99 -O2 -o login login.c

# Chạy
./login
```

## Sử dụng

1. Chạy chương trình: `./login`
2. Chọn chức năng từ menu:
   - **1. Đăng nhập**: Đăng nhập với tài khoản đã có
   - **2. Đăng ký**: Tạo tài khoản mới
   - **3. Thoát**: Thoát chương trình

### Đăng ký tài khoản mới

1. Chọn "2. Đăng ký" từ menu chính
2. Nhập thông tin:
   - Username (tên đăng nhập)
   - Họ và tên
   - Email
   - Password (sẽ được ẩn khi nhập)

### Đăng nhập

1. Chọn "1. Đăng nhập" từ menu chính
2. Nhập username và password
3. Sau khi đăng nhập thành công, bạn sẽ thấy thông tin hệ thống

## Cấu trúc dữ liệu

Chương trình lưu trữ thông tin user trong file `users.dat` với cấu trúc:

```c
typedef struct {
    char username[50];
    char password[50];
    char fullname[100];
    char email[100];
} User;
```

## Tính năng bảo mật

- Mật khẩu được ẩn khi nhập (hiển thị dấu *)
- Dữ liệu được lưu trong file binary
- Kiểm tra username trùng lặp khi đăng ký

## Giao diện

Chương trình sử dụng ANSI color codes để tạo giao diện đẹp mắt:
- Màu xanh dương cho menu
- Màu xanh lá cho thông báo thành công
- Màu đỏ cho thông báo lỗi
- Màu vàng cho cảnh báo
- Màu cyan cho banner

## Tùy chỉnh

Bạn có thể dễ dàng tùy chỉnh:
- Màu sắc trong các macro định nghĩa màu
- Kích thước buffer trong cấu trúc User
- Thông báo và giao diện trong các hàm hiển thị

## Xử lý lỗi

Chương trình xử lý các lỗi phổ biến:
- File không tồn tại (tạo mới)
- Username trùng lặp
- Lỗi đọc/ghi file
- Nhập liệu không hợp lệ

## Phát triển

Để phát triển thêm tính năng:
1. Thêm các hàm mới vào file `login.c`
2. Cập nhật menu chính trong hàm `main()`
3. Biên dịch lại với `make`

## Giấy phép

Chương trình này được phát hành dưới giấy phép MIT.

## Tác giả

Được phát triển cho Ubuntu Linux sử dụng ngôn ngữ C.