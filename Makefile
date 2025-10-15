# Makefile cho File System Adventure Game
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
LDFLAGS = 
TARGET = filesystem_game
SOURCES = main.c file_manager.c trap_manager.c ui_manager_simple.c storage_manager.c
HEADERS = game_config.h file_manager.h trap_manager.h ui_manager.h storage_manager.h
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
	@echo "$(GREEN)Đang chạy File System Adventure Game...$(NC)"
	./$(TARGET)

# Test chương trình
test: $(TARGET)
	@echo "$(GREEN)Đang chạy test...$(NC)"
	@echo "Tạo test files..."
	@mkdir -p test_game
	@cd test_game && echo "TREASURE\nTest treasure content" > treasure.txt
	@cd test_game && echo "TRAP\nTest trap content" > trap.txt
	@cd test_game && echo "Hidden file content" > .hidden.txt
	@cd test_game && echo "Normal file content" > normal.txt
	@cd test_game && mkdir test_dir
	@echo "$(GREEN)✓ Test files đã được tạo trong thư mục test_game/$(NC)"

# Debug build
debug: CFLAGS += -DDEBUG -g3
debug: $(TARGET)

# Release build
release: CFLAGS += -DNDEBUG -O3
release: clean $(TARGET)

# Clean các file build
clean:
	@echo "$(YELLOW)Đang xóa các file build...$(NC)"
	rm -f $(OBJECTS) $(TARGET) *.dat *.csv
	rm -rf test_game
	@echo "$(GREEN)✓ Đã xóa sạch!$(NC)"

# Cài đặt (copy vào /usr/local/bin)
install: $(TARGET)
	@echo "$(GREEN)Đang cài đặt chương trình...$(NC)"
	sudo cp $(TARGET) /usr/local/bin/
	@echo "$(GREEN)✓ Cài đặt thành công!$(NC)"
	@echo "$(YELLOW)Bạn có thể chạy chương trình từ bất kỳ đâu bằng lệnh: filesystem_game$(NC)"

# Gỡ cài đặt
uninstall:
	@echo "$(YELLOW)Đang gỡ cài đặt chương trình...$(NC)"
	sudo rm -f /usr/local/bin/$(TARGET)
	@echo "$(GREEN)✓ Đã gỡ cài đặt!$(NC)"

# Tạo package
package: release
	@echo "$(GREEN)Đang tạo package...$(NC)"
	tar -czf filesystem_game.tar.gz $(TARGET) README.md *.h
	@echo "$(GREEN)✓ Package đã được tạo: filesystem_game.tar.gz$(NC)"

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

# Kiểm tra code coverage
coverage: CFLAGS += --coverage
coverage: LDFLAGS += --coverage
coverage: clean $(TARGET)
	@echo "$(GREEN)Đang chạy test coverage...$(NC)"
	./$(TARGET) &
	sleep 2
	pkill -f $(TARGET)
	gcov *.c
	@echo "$(GREEN)✓ Coverage report đã được tạo$(NC)"

# Hiển thị thông tin
info:
	@echo "$(GREEN)╔══════════════════════════════════════════════════════════════╗$(NC)"
	@echo "$(GREEN)║$(NC)                FILE SYSTEM ADVENTURE GAME                $(GREEN)║$(NC)"
	@echo "$(GREEN)╚══════════════════════════════════════════════════════════════╝$(NC)"
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
	@echo "  make coverage - Kiểm tra code coverage"
	@echo "  make info     - Hiển thị thông tin này"
	@echo ""
	@echo "Yêu cầu hệ thống:"
	@echo "  - Ubuntu Linux (hoặc các distro Linux khác)"
	@echo "  - GCC compiler"
	@echo "  - ncurses library"
	@echo "  - Make utility"

# Phụ thuộc
.PHONY: all run test debug release clean install uninstall package valgrind gdb check-memory coverage info

# Định nghĩa phụ thuộc
main.o: main.c game_config.h file_manager.h trap_manager.h ui_manager.h storage_manager.h
file_manager.o: file_manager.c file_manager.h game_config.h
trap_manager.o: trap_manager.c trap_manager.h game_config.h
ui_manager.o: ui_manager.c ui_manager.h game_config.h
storage_manager.o: storage_manager.c storage_manager.h game_config.h