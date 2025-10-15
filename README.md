# File System Adventure Game

Một game console đầy thú vị được phát triển bằng C cho Ubuntu Linux, mô phỏng cuộc phiêu lưu trong hệ thống file với các tính năng hack, diệt virus và tìm kho báu.

## 🎮 Tính năng chính

### Module Filesystem (Thành viên 1)
- **Duyệt hệ thống files**: Sử dụng `opendir()`, `readdir()`, `stat()` để liệt kê file/thư mục
- **Di chuyển thư mục**: Sử dụng `chdir()` với kiểm tra quyền truy cập
- **Mở file**: Đọc nội dung file với `open()`, `read()`
- **Phát hiện kho báu/bẫy**: Tích hợp giải mã XOR cho kho báu
- **Màu sắc phân loại**: File ẩn, thư mục, kho báu, bẫy có màu sắc riêng

### Module Traps (Thành viên 2)
- **Hack quyền file**: Sử dụng `chmod()` (giới hạn 3 lần/level, Admin only)
- **Quản lý virus**: Tạo và kill virus process với `fork()`, `pipe()`, `kill()`
- **Scan virus**: Hiển thị PID và thông tin virus
- **Điều chỉnh độ khó**: Dựa trên `/proc/meminfo` và system load

### Module UI (Thành viên 3)
- **Giao diện ncurses**: Menu đẹp mắt với màu sắc và ASCII art
- **Xử lý lệnh**: Parse các lệnh `ls`, `cd`, `open`, `hack`, `kill`, `scan`, `quit`
- **Hiển thị thống kê**: Điểm số, mạng sống, kho báu, bẫy, virus
- **Phân quyền**: Giới hạn lệnh theo role (User/Admin)

### Module Storage/Ranking (Thành viên 4)
- **Tính toán điểm**: Dựa trên treasures, traps, levels, thời gian, độ khó
- **Báo cáo kết quả**: ASCII table và CSV export
- **Lưu/khôi phục**: Game state và ranking system
- **Xếp hạng**: Top 100 players với filter theo role

## 🚀 Cài đặt và chạy

### Yêu cầu hệ thống
```bash
# Ubuntu/Debian
sudo apt-get install build-essential libncurses5-dev

# CentOS/RHEL
sudo yum install gcc ncurses-devel

# Arch Linux
sudo pacman -S gcc ncurses
```

### Biên dịch và chạy
```bash
# Biên dịch
make

# Chạy game
make run

# Tạo test files
make test

# Build debug version
make debug

# Build release version
make release
```

### Cài đặt vào hệ thống
```bash
# Cài đặt
sudo make install

# Chạy từ bất kỳ đâu
filesystem_game

# Gỡ cài đặt
sudo make uninstall
```

## 🎯 Cách chơi

### Lệnh cơ bản
- `ls` - Liệt kê files trong thư mục
- `cd <dir>` - Chuyển thư mục
- `open <file>` - Mở file (có thể là kho báu hoặc bẫy)
- `quit` - Thoát game

### Lệnh Admin (chỉ khi chạy với quyền root)
- `hack <file>` - Hack quyền file (+50 điểm, giới hạn 3 lần/level)
- `scan` - Scan virus trong hệ thống
- `kill <pid>` - Kill virus (+200 điểm)
- `create_virus <level>` - Tạo virus test

### Mục tiêu
- 🏆 Tìm kho báu để tăng điểm (+100 điểm)
- ⚠️ Tránh bẫy để không mất mạng (-50 điểm, -1 mạng)
- 🦠 Kill virus để tăng điểm (+200 điểm, Admin only)
- 🔓 Hack file để tăng điểm (+50 điểm, Admin only)
- 📈 Lên level để mở khóa tính năng mới

## 🏗️ Cấu trúc dự án

```
filesystem_game/
├── main.c                 # Main game loop
├── game_config.h          # Cấu hình và constants
├── file_manager.h/.c      # Module Filesystem
├── trap_manager.h/.c      # Module Traps  
├── ui_manager.h/.c        # Module UI
├── storage_manager.h/.c   # Module Storage/Ranking
├── Makefile              # Build system
└── README.md             # Tài liệu này
```

## 🔧 Phát triển

### Debug
```bash
# Chạy với gdb
make gdb

# Chạy với valgrind
make valgrind

# Kiểm tra memory leaks
make check-memory

# Code coverage
make coverage
```

### Test
```bash
# Tạo test files
make test

# Chạy test tự động
echo -e "1\nls\nopen treasure.txt\nquit" | ./filesystem_game
```

## 📊 Tính năng kỹ thuật

### System Calls sử dụng
- **File operations**: `open()`, `read()`, `write()`, `close()`, `stat()`, `chmod()`
- **Directory operations**: `opendir()`, `readdir()`, `closedir()`, `chdir()`
- **Process management**: `fork()`, `exec()`, `wait()`, `kill()`, `signal()`
- **Memory management**: `malloc()`, `free()`, `memset()`, `memcpy()`

### Libraries
- **ncurses**: Giao diện console đẹp mắt
- **Standard C**: `stdio.h`, `stdlib.h`, `string.h`, `time.h`
- **POSIX**: `unistd.h`, `sys/stat.h`, `sys/wait.h`, `signal.h`

### Data Structures
- **GameState_t**: Trạng thái game hiện tại
- **PlayerRecord**: Thông tin người chơi
- **RankingSystem**: Hệ thống xếp hạng
- **FileInfo**: Thông tin file/thư mục
- **VirusProcess**: Thông tin virus process

## 🎨 Giao diện

### Màu sắc
- 🔵 **Xanh dương**: Thư mục
- 🟢 **Xanh lá**: Kho báu, thành công
- 🔴 **Đỏ**: Bẫy, lỗi
- 🟡 **Vàng**: File ẩn, cảnh báo
- ⚪ **Trắng**: File bình thường

### ASCII Art
- Welcome screen với khung đẹp mắt
- Game over screen với hiệu ứng
- Level up animation
- Progress bars và thống kê

## 🔒 Bảo mật

### Phân quyền
- **User**: Chỉ có thể chơi game cơ bản
- **Admin**: Có thể hack file và kill virus

### Giới hạn
- Hack file: Tối đa 3 lần/level
- Virus: Chỉ Admin mới có thể tạo/kill
- Memory: Giới hạn buffer để tránh overflow

## 📈 Performance

### Tối ưu hóa
- Sử dụng binary file format cho ranking
- Lazy loading cho file listing
- Efficient memory management
- Minimal system calls

### Monitoring
- Real-time system load monitoring
- Memory usage tracking
- Performance metrics

## 🐛 Troubleshooting

### Lỗi thường gặp
```bash
# Lỗi ncurses
sudo apt-get install libncurses5-dev

# Lỗi permission
sudo chmod +x filesystem_game

# Lỗi memory
make clean && make debug
```

### Debug mode
```bash
# Chạy với debug info
make debug
gdb ./filesystem_game

# Kiểm tra memory leaks
valgrind --leak-check=full ./filesystem_game
```

## 🤝 Đóng góp

### Cách đóng góp
1. Fork repository
2. Tạo feature branch
3. Commit changes
4. Push to branch
5. Tạo Pull Request

### Coding standards
- Sử dụng C99 standard
- Comment code bằng tiếng Việt
- Format code theo GNU style
- Test trước khi commit

## 📄 License

MIT License - Xem file LICENSE để biết thêm chi tiết.

## 👥 Tác giả

**Team File System Adventure Game**
- **Thành viên 1**: Module Filesystem
- **Thành viên 2**: Module Traps  
- **Thành viên 3**: Module UI
- **Thành viên 4**: Module Storage/Ranking

## 🎉 Chúc mừng!

Bạn đã hoàn thành việc phát triển File System Adventure Game! Đây là một dự án phức tạp với nhiều module tích hợp, sử dụng các system calls Linux và tạo ra một trải nghiệm game thú vị.

**Tổng thời gian phát triển ước tính**: 60-80 giờ
**Độ phức tạp**: Cao
**Tính năng**: Đầy đủ theo yêu cầu

Chúc bạn chơi game vui vẻ! 🎮