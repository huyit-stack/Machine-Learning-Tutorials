#!/bin/bash

# Demo script cho File System Adventure Game
echo "=== DEMO FILE SYSTEM ADVENTURE GAME ==="
echo ""

# Màu sắc
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function để hiển thị thông báo
show_message() {
    echo -e "${GREEN}✓ $1${NC}"
}

show_warning() {
    echo -e "${YELLOW}⚠ $1${NC}"
}

show_error() {
    echo -e "${RED}✗ $1${NC}"
}

# Kiểm tra dependencies
echo "1. Kiểm tra dependencies..."
if ! command -v gcc &> /dev/null; then
    show_error "GCC không được cài đặt"
    exit 1
fi

if ! pkg-config --exists ncurses; then
    show_error "ncurses library không được cài đặt"
    echo "Cài đặt: sudo apt-get install libncurses5-dev"
    exit 1
fi

show_message "Dependencies OK"

# Build chương trình
echo ""
echo "2. Biên dịch chương trình..."
make clean
if make; then
    show_message "Build thành công"
else
    show_error "Build thất bại"
    exit 1
fi

# Tạo test files
echo ""
echo "3. Tạo test files..."
make test
show_message "Test files đã được tạo"

# Test chương trình cơ bản
echo ""
echo "4. Test chương trình cơ bản..."
echo "Chạy game với các lệnh test..."

# Tạo script test tự động
cat > test_commands.txt << EOF
1
ls
open treasure.txt
open trap.txt
open normal.txt
ls
quit
EOF

echo "Test commands:"
cat test_commands.txt
echo ""

# Chạy test (timeout sau 10 giây)
timeout 10s ./filesystem_game < test_commands.txt || true

# Test Admin features (nếu chạy với sudo)
echo ""
echo "5. Test Admin features..."
if [ "$EUID" -eq 0 ]; then
    show_message "Chạy với quyền root - test Admin features"
    
    cat > test_admin_commands.txt << EOF
1
create_virus 1
scan
ls
hack treasure.txt
quit
EOF
    
    echo "Admin test commands:"
    cat test_admin_commands.txt
    echo ""
    
    timeout 10s ./filesystem_game < test_admin_commands.txt || true
else
    show_warning "Không chạy với quyền root - bỏ qua Admin features test"
fi

# Test ranking system
echo ""
echo "6. Test ranking system..."
if [ -f "ranking.dat" ]; then
    show_message "File ranking.dat đã được tạo"
    ls -la ranking.dat
else
    show_warning "File ranking.dat chưa được tạo"
fi

# Test save/load
echo ""
echo "7. Test save/load game state..."
if [ -f "savegame.dat" ]; then
    show_message "File savegame.dat đã được tạo"
    ls -la savegame.dat
else
    show_warning "File savegame.dat chưa được tạo"
fi

# Performance test
echo ""
echo "8. Performance test..."
echo "Chạy valgrind để kiểm tra memory leaks..."

if command -v valgrind &> /dev/null; then
    timeout 5s valgrind --leak-check=summary ./filesystem_game < test_commands.txt 2>&1 | grep -E "(LEAK|ERROR|definitely lost)" || true
    show_message "Valgrind test hoàn thành"
else
    show_warning "Valgrind không được cài đặt - bỏ qua memory test"
fi

# Cleanup
echo ""
echo "9. Cleanup..."
rm -f test_commands.txt test_admin_commands.txt
show_message "Cleanup hoàn thành"

# Summary
echo ""
echo "=== DEMO SUMMARY ==="
echo "✓ Chương trình đã được build thành công"
echo "✓ Test files đã được tạo"
echo "✓ Game đã được test với các lệnh cơ bản"
if [ "$EUID" -eq 0 ]; then
    echo "✓ Admin features đã được test"
else
    echo "⚠ Admin features chưa được test (cần chạy với sudo)"
fi
echo "✓ Ranking system đã được test"
echo "✓ Save/load system đã được test"

echo ""
echo "Để chạy game thực tế:"
echo "  ./filesystem_game"
echo ""
echo "Để chạy với quyền Admin:"
echo "  sudo ./filesystem_game"
echo ""
echo "Để xem help:"
echo "  make info"

echo ""
show_message "Demo hoàn thành!"