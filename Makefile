# Makefile cho chương trình Login System
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
TARGET = login_system
SOURCE = main.c

# Màu sắc cho terminal
GREEN = \033[0;32m
YELLOW = \033[1;33m
RED = \033[0;31m
NC = \033[0m # No Color

# Target mặc định
all: $(TARGET)

# Compile chương trình
$(TARGET): $(SOURCE)
	@echo "$(YELLOW)Đang compile chương trình...$(NC)"
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)
	@echo "$(GREEN)✅ Compile thành công!$(NC)"
	@echo "$(GREEN)Chạy chương trình bằng: ./$(TARGET)$(NC)"

# Chạy chương trình
run: $(TARGET)
	@echo "$(YELLOW)Đang chạy chương trình...$(NC)"
	./$(TARGET)

# Clean các file đã compile
clean:
	@echo "$(YELLOW)Đang xóa các file đã compile...$(NC)"
	rm -f $(TARGET)
	@echo "$(GREEN)✅ Đã xóa thành công!$(NC)"

# Debug version
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

# Install (copy to /usr/local/bin)
install: $(TARGET)
	@echo "$(YELLOW)Đang cài đặt chương trình...$(NC)"
	sudo cp $(TARGET) /usr/local/bin/
	@echo "$(GREEN)✅ Cài đặt thành công!$(NC)"
	@echo "$(GREEN)Bạn có thể chạy chương trình từ bất kỳ đâu bằng lệnh: $(TARGET)$(NC)"

# Uninstall
uninstall:
	@echo "$(YELLOW)Đang gỡ cài đặt chương trình...$(NC)"
	sudo rm -f /usr/local/bin/$(TARGET)
	@echo "$(GREEN)✅ Gỡ cài đặt thành công!$(NC)"

# Help
help:
	@echo "$(GREEN)=== HƯỚNG DẪN SỬ DỤNG MAKEFILE ===$(NC)"
	@echo "$(YELLOW)make$(NC)           - Compile chương trình"
	@echo "$(YELLOW)make run$(NC)       - Compile và chạy chương trình"
	@echo "$(YELLOW)make clean$(NC)     - Xóa file đã compile"
	@echo "$(YELLOW)make debug$(NC)     - Compile phiên bản debug"
	@echo "$(YELLOW)make install$(NC)   - Cài đặt chương trình vào hệ thống"
	@echo "$(YELLOW)make uninstall$(NC) - Gỡ cài đặt chương trình"
	@echo "$(YELLOW)make help$(NC)      - Hiển thị hướng dẫn này"

# Phony targets
.PHONY: all run clean debug install uninstall help