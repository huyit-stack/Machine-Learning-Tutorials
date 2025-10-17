# File Systems Explorer

Một game console đầy thú vị được phát triển bằng C cho Ubuntu Linux, mô phỏng cuộc phiêu lưu trong hệ thống file với các tính năng hack, diệt virus và tìm kho báu.

## 🏗️ Cấu trúc dự án

```
FileSystemsExplorer/
├── project/                          # Source code modules
│   ├── filesystem/               # Sang Đăng: Duyệt/Di chuyển/Mở file
│   │   ├── file_manager.c        # Core filesystem operations
│   │   ├── file_manager.h        # Header cho file_manager
│   │   ├── directory_ops.c       # Duyệt hệ thống files (ls, opendir)
│   │   ├── directory_ops.h       # Header cho directory
│   │   ├── file_ops.c            # Mở file, kiểm tra kho báu/bẫy
│   │   └── file_ops.h            # Header cho file operations
│   │
│   ├── traps/                    # Thuần: Hack/Virus
│   │   ├── traps.c               # Core trap management
│   │   ├── traps.h               # Header cho traps
│   │   ├── hack_system.c         # Hack quyền file (chmod)
│   │   ├── hack_system.h         # Header hack
│   │   ├── virus_manager.c       # Quản lý virus process (fork, kill)
│   │   └── virus_manager.h       # Header virus
│   │
│   ├── ui/                       # Quách Huy: Giao diện/Xử lý lệnh
│   │   ├── ui.c                  # Core UI (ncurses)
│   │   ├── ui.h                  # Header UI
│   │   ├── console_ui.c          # Giao diện console, ASCII art
│   │   ├── console_ui.h          # Header console
│   │   ├── command_parser.c      # Xử lý lệnh người dùng
│   │   └── command_parser.h      # Header parser
│   │
│   ├── storage/                  # Dương Phạm: Tính toán/Lưu trữ/Ranking
│   │   ├── storage.c             # Core storage operations
│   │   ├── storage.h             # Header storage
│   │   ├── game_state.c          # Tính toán trạng thái, điểm số
│   │   ├── game_state.h          # Header game state
│   │   ├── ranking_system.c      # Hệ thống ranking
│   │   ├── ranking_system.h      # Header ranking
│   │   ├── save_load.c           # Lưu/khôi phục trạng thái
│   │   └── save_load.h           # Header save/load
│   │
│   └── main.c                    # Entry point chính
│
├── include/                      # Global headers
│   ├── game_structs.h            # Structs (GameState, PlayerRecord)
│   ├── constants.h               # Constants (điểm, lives, etc.)
│   ├── error_codes.h             # Error codes và messages
│   └── utils.h                   # Utility functions chung
│
├── data/                         # Game data files
│   ├── ranking.dat               # Binary ranking file (auto)
│   ├── game_save.dat             # Game state save (auto)
│   ├── treasures/                # Test treasure files
│   │   ├── treasure_1.txt
│   │   ├── trap_file.txt
│   │   └── hidden_gem.enc
│   └── config/                   # Config files
│       └── game_config.ini
│
├── Makefile                      # Build system
└── README.md                     # Tài liệu này
```

## 🎮 Tính năng chính

### Module Filesystem (Sang Đăng)
- **Duyệt hệ thống files**: Sử dụng `opendir()`, `readdir()`, `stat()` để liệt kê file/thư mục
- **Di chuyển thư mục**: Sử dụng `chdir()` với kiểm tra quyền truy cập
- **Mở file**: Đọc nội dung file với `open()`, `read()`
- **Phát hiện kho báu/bẫy**: Tích hợp giải mã XOR cho kho báu
- **Màu sắc phân loại**: File ẩn, thư mục, kho báu, bẫy có màu sắc riêng

### Module Traps (Thuần)
- **Hack quyền file**: Sử dụng `chmod()` (giới hạn 3 lần/level, Admin only)
- **Quản lý virus**: Tạo và kill virus process với `fork()`, `pipe()`, `kill()`
- **Scan virus**: Hiển thị PID và thông tin virus
- **Điều chỉnh độ khó**: Dựa trên `/proc/meminfo` và system load

### Module UI (Quách Huy)
- **Giao diện console**: Menu đẹp mắt với màu sắc và ASCII art
- **Xử lý lệnh**: Parse các lệnh `ls`, `cd`, `open`, `hack`, `kill`, `scan`, `quit`
- **Hiển thị thống kê**: Điểm số, mạng sống, kho báu, bẫy, virus
- **Phân quyền**: Giới hạn lệnh theo role (User/Admin)

### Module Storage (Dương Phạm)
- **Tính toán điểm**: Dựa trên treasures, traps, levels, thời gian, độ khó
- **Báo cáo kết quả**: ASCII table và CSV export
- **Lưu/khôi phục**: Game state và ranking system
- **Xếp hạng**: Top 100 players với filter theo role

## 🚀 Cài đặt và chạy

### Yêu cầu hệ thống
```bash
# Ubuntu/Debian
sudo apt-get install build-essential

# CentOS/RHEL
sudo yum install gcc

# Arch Linux
sudo pacman -S gcc
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
filesystem_explorer

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

## 🔧 Phát triển

### Debug
```bash
# Chạy với gdb
make gdb

# Chạy với valgrind
make valgrind

# Kiểm tra memory leaks
make check-memory
```

### Test
```bash
# Tạo test files
make test

# Chạy test tự động
echo -e "1\nls\nopen data/treasures/treasure_1.txt\nquit" | ./filesystem_explorer
```

## 📊 Tính năng kỹ thuật

### System Calls sử dụng
- **File operations**: `open()`, `read()`, `write()`, `close()`, `stat()`, `chmod()`
- **Directory operations**: `opendir()`, `readdir()`, `closedir()`, `chdir()`
- **Process management**: `fork()`, `exec()`, `wait()`, `kill()`, `signal()`
- **Memory management**: `malloc()`, `free()`, `memset()`, `memcpy()`

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
# Lỗi compilation
make clean && make

# Lỗi permission
sudo chmod +x filesystem_explorer

# Lỗi memory
make clean && make debug
```

### Debug mode
```bash
# Chạy với debug info
make debug
gdb ./filesystem_explorer

# Kiểm tra memory leaks
valgrind --leak-check=full ./filesystem_explorer
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

**Team File Systems Explorer**
- **Sang Đăng**: Module Filesystem
- **Thuần**: Module Traps  
- **Quách Huy**: Module UI
- **Dương Phạm**: Module Storage

## 🎉 Chúc mừng!

Bạn đã hoàn thành việc phát triển File Systems Explorer! Đây là một dự án phức tạp với nhiều module tích hợp, sử dụng các system calls Linux và tạo ra một trải nghiệm game thú vị.

**Tổng thời gian phát triển ước tính**: 60-80 giờ
**Độ phức tạp**: Cao
**Tính năng**: Đầy đủ theo yêu cầu

Chúc bạn chơi game vui vẻ! 🎮