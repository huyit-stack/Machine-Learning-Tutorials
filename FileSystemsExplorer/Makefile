# Makefile cho File Systems Explorer
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g -Iinclude
LDFLAGS = 
TARGET = filesystem_explorer
SOURCES = project/main.c include/utils.c \
          project/filesystem/file_manager.c project/filesystem/directory_ops.c project/filesystem/file_ops.c \
          project/traps/traps.c project/traps/hack_system.c project/traps/virus_manager.c \
          project/ui/ui.c project/ui/console_ui.c project/ui/command_parser.c \
          project/storage/storage.c project/storage/game_state.c project/storage/ranking_system.c project/storage/save_load.c
HEADERS = include/game_structs.h include/constants.h include/error_codes.h include/utils.h \
          project/filesystem/*.h project/traps/*.h project/ui/*.h project/storage/*.h
OBJECTS = $(SOURCES:.c=.o)

# Màu sắc cho terminal
GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
BLUE = \033[0;34m
NC = \033[0m # No Color

# Mục tiêu mặc định
all: $(TARGET)

# Build chương trình chính
$(TARGET): $(OBJECTS)
	@echo "$(GREEN)Đang liên kết chương trình...$(NC)"
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "$(GREEN)✓ Build thành công!$(NC)"
	@echo "$(YELLOW)Chạy chương trình: ./$(TARGET)$(NC)"

# Compile các file object
%.o: %.c $(HEADERS)
	@echo "$(BLUE)Đang biên dịch $<...$(NC)"
	$(CC) $(CFLAGS) -c $< -o $@

# Chạy chương trình
run: $(TARGET)
	@echo "$(GREEN)Đang chạy File Systems Explorer...$(NC)"
	./$(TARGET)

# Test chương trình
test: $(TARGET)
	@echo "$(GREEN)Đang chạy test...$(NC)"
	@echo "Tạo test files..."
	@mkdir -p data/treasures data/config
	@echo "TREASURE\nTest treasure content" > data/treasures/treasure_1.txt
	@echo "TRAP\nTest trap content" > data/treasures/trap_file.txt
	@echo "Hidden file content" > data/treasures/hidden_gem.enc
	@echo "Normal file content" > data/normal.txt
	@mkdir -p data/test_dir
	@echo "$(GREEN)✓ Test files đã được tạo$(NC)"

# Debug build
debug: CFLAGS += -DDEBUG -g3
debug: $(TARGET)

# Release build
release: CFLAGS += -DNDEBUG -O3
release: clean $(TARGET)

# Clean các file build
clean:
	@echo "$(YELLOW)Đang xóa các file build...$(NC)"
	rm -f $(OBJECTS) $(TARGET) data/*.dat data/*.csv
	rm -rf data/treasures data/config data/test_dir
	@echo "$(GREEN)✓ Đã xóa sạch!$(NC)"

# Cài đặt (copy vào /usr/local/bin)
install: $(TARGET)
	@echo "$(GREEN)Đang cài đặt chương trình...$(NC)"
	sudo cp $(TARGET) /usr/local/bin/
	@echo "$(GREEN)✓ Cài đặt thành công!$(NC)"
	@echo "$(YELLOW)Bạn có thể chạy chương trình từ bất kỳ đâu bằng lệnh: filesystem_explorer$(NC)"

# Gỡ cài đặt
uninstall:
	@echo "$(YELLOW)Đang gỡ cài đặt chương trình...$(NC)"
	sudo rm -f /usr/local/bin/$(TARGET)
	@echo "$(GREEN)✓ Đã gỡ cài đặt!$(NC)"

# Tạo package
package: release
	@echo "$(GREEN)Đang tạo package...$(NC)"
	tar -czf filesystem_explorer.tar.gz $(TARGET) README.md include/ project/ data/ docs/
	@echo "$(GREEN)✓ Package đã được tạo: filesystem_explorer.tar.gz$(NC)"

# Chạy với valgrind (debug memory)
valgrind: debug
	@echo "$(GREEN)Đang chạy với valgrind...$(NC)"
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Chạy với gdb (debug)
gdb: debug
	@echo "$(GREEN)Đang chạy với gdb...$(NC)"
	gdb ./$(TARGET)

# Kiểm tra memory leaks
check-memory: debug
	@echo "$(GREEN)Đang kiểm tra memory leaks...$(NC)"
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./$(TARGET)

# Hiển thị thông tin
info:
	@echo "$(GREEN)╔══════════════════════════════════════════════════════════════╗$(NC)"
	@echo "$(GREEN)║$(NC)                FILE SYSTEMS EXPLORER                     $(GREEN)║$(NC)"
	@echo "$(GREEN)╚══════════════════════════════════════════════════════════════╝$(NC)"
	@echo ""
	@echo "Cấu trúc dự án:"
	@echo "  project/filesystem/  - Module Filesystem (Sang Đăng)"
	@echo "  project/traps/       - Module Traps (Thuần)"
	@echo "  project/ui/          - Module UI (Quách Huy)"
	@echo "  project/storage/     - Module Storage (Dương Phạm)"
	@echo "  include/             - Global headers"
	@echo "  data/                - Game data files"
	@echo ""
	@echo "Các lệnh có sẵn:"
	@echo "  make          - Biên dịch chương trình"
	@echo "  make run      - Chạy chương trình"
	@echo "  make test     - Tạo test files"
	@echo "  make debug    - Build phiên bản debug"
	@echo "  make release  - Build phiên bản release"
	@echo "  make clean    - Xóa các file build"
	@echo "  make install  - Cài đặt vào hệ thống"
	@echo "  make uninstall- Gỡ cài đặt"
	@echo "  make package  - Tạo package tar.gz"
	@echo "  make valgrind - Chạy với valgrind"
	@echo "  make gdb      - Chạy với gdb"
	@echo "  make info     - Hiển thị thông tin này"

# Phụ thuộc
.PHONY: all run test debug release clean install uninstall package valgrind gdb check-memory info