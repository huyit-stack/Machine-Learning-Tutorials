# Makefile cho Ubuntu Login System
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
TARGET = login
SOURCE = login.c

# Màu sắc cho terminal
GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
NC = \033[0m # No Color

# Mục tiêu mặc định
all: $(TARGET)

# Build chương trình
$(TARGET): $(SOURCE)
	@echo "$(GREEN)Đang biên dịch chương trình...$(NC)"
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)
	@echo "$(GREEN)✓ Biên dịch thành công!$(NC)"
	@echo "$(YELLOW)Chạy chương trình: ./$(TARGET)$(NC)"

# Chạy chương trình
run: $(TARGET)
	@echo "$(GREEN)Đang chạy chương trình...$(NC)"
	./$(TARGET)

# Clean các file build
clean:
	@echo "$(YELLOW)Đang xóa các file build...$(NC)"
	rm -f $(TARGET) users.dat
	@echo "$(GREEN)✓ Đã xóa sạch!$(NC)"

# Cài đặt (copy vào /usr/local/bin)
install: $(TARGET)
	@echo "$(GREEN)Đang cài đặt chương trình...$(NC)"
	sudo cp $(TARGET) /usr/local/bin/
	@echo "$(GREEN)✓ Cài đặt thành công!$(NC)"
	@echo "$(YELLOW)Bạn có thể chạy chương trình từ bất kỳ đâu bằng lệnh: login$(NC)"

# Gỡ cài đặt
uninstall:
	@echo "$(YELLOW)Đang gỡ cài đặt chương trình...$(NC)"
	sudo rm -f /usr/local/bin/$(TARGET)
	@echo "$(GREEN)✓ Đã gỡ cài đặt!$(NC)"

# Hiển thị thông tin
info:
	@echo "$(GREEN)╔══════════════════════════════════════════════════════════════╗$(NC)"
	@echo "$(GREEN)║$(NC)                    UBUNTU LOGIN SYSTEM                    $(GREEN)║$(NC)"
	@echo "$(GREEN)╚══════════════════════════════════════════════════════════════╝$(NC)"
	@echo ""
	@echo "Các lệnh có sẵn:"
	@echo "  make          - Biên dịch chương trình"
	@echo "  make run      - Chạy chương trình"
	@echo "  make clean    - Xóa các file build"
	@echo "  make install  - Cài đặt vào hệ thống"
	@echo "  make uninstall- Gỡ cài đặt"
	@echo "  make info     - Hiển thị thông tin này"

# Phụ thuộc
.PHONY: all run clean install uninstall info