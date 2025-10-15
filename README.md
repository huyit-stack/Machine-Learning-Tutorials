# Hệ Thống Đăng Nhập Ubuntu - C Language

Một chương trình đăng nhập đơn giản được viết bằng ngôn ngữ C, có giao diện console đẹp mắt và các tính năng cơ bản.

## 🚀 Tính Năng

- **Giao diện đăng nhập đẹp mắt** với khung viền Unicode
- **Ẩn mật khẩu** khi nhập (hiển thị dấu *)
- **Xác thực người dùng** với username/password
- **Phân quyền** Admin và User thông thường
- **Menu chính** với các chức năng:
  - Xem thông tin cá nhân
  - Đổi mật khẩu
  - Quản lý người dùng (chỉ Admin)
  - Xem log hệ thống (chỉ Admin)
- **Giới hạn số lần đăng nhập** (tối đa 3 lần)

## 📋 Yêu Cầu Hệ Thống

- Ubuntu Linux (hoặc các distro Linux khác)
- GCC compiler
- Terminal hỗ trợ Unicode

## 🛠️ Cài Đặt và Chạy

### Cách 1: Sử dụng Makefile (Khuyến nghị)

```bash
# Compile chương trình
make

# Chạy chương trình
make run

# Hoặc chạy trực tiếp
./login_system
```

### Cách 2: Compile thủ công

```bash
gcc -Wall -Wextra -std=c99 -O2 -o login_system main.c
./login_system
```

## 👥 Tài Khoản Mặc Định

Chương trình có sẵn các tài khoản sau:

| Username | Password | Quyền |
|----------|----------|-------|
| admin    | admin123 | Administrator |
| user1    | password1| User |
| user2    | password2| User |
| guest    | guest    | User |

## 🎯 Cách Sử Dụng

1. **Chạy chương trình**:
   ```bash
   ./login_system
   ```

2. **Đăng nhập**:
   - Nhập username
   - Nhập password (sẽ được ẩn khi gõ)

3. **Sử dụng menu**:
   - Chọn các chức năng từ menu chính
   - Admin có thêm quyền quản lý users và xem log

## 🔧 Các Lệnh Makefile

```bash
make          # Compile chương trình
make run      # Compile và chạy
make clean    # Xóa file đã compile
make debug    # Compile phiên bản debug
make install  # Cài đặt vào hệ thống
make uninstall# Gỡ cài đặt
make help     # Xem hướng dẫn
```

## 📁 Cấu Trúc File

```
.
├── main.c      # File source code chính
├── Makefile    # File build configuration
└── README.md   # File hướng dẫn này
```

## 🔒 Bảo Mật

- Mật khẩu được ẩn khi nhập
- Giới hạn số lần đăng nhập thất bại
- Phân quyền rõ ràng giữa Admin và User

## 🎨 Giao Diện

Chương trình sử dụng các ký tự Unicode để tạo giao diện đẹp mắt:
- Khung viền với ký tự `┌─┐│└┘`
- Màu sắc terminal (nếu hỗ trợ)
- Layout rõ ràng, dễ đọc

## 🐛 Debug

Để compile phiên bản debug:
```bash
make debug
```

## 📝 Lưu Ý

- Chương trình này chỉ mang tính chất demo
- Trong thực tế, nên lưu dữ liệu user vào file hoặc database
- Nên mã hóa mật khẩu trước khi lưu trữ
- Cần thêm các biện pháp bảo mật khác

## 🤝 Đóng Góp

Mọi đóng góp đều được chào đón! Hãy tạo issue hoặc pull request.

## 📄 License

MIT License - Xem file LICENSE để biết thêm chi tiết.

---

**Tác giả**: Ubuntu C Login System  
**Phiên bản**: 1.0.0  
**Ngày tạo**: 2024