#!/bin/bash

# Test runner script cho File Systems Explorer
echo "=== RUNNING TESTS ==="

# Màu sắc
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
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

# Kiểm tra chương trình đã build chưa
if [ ! -f "filesystem_explorer" ]; then
    show_error "Chương trình chưa được build. Chạy: make"
    exit 1
fi

# Test 1: Basic functionality
echo "1. Test basic functionality..."
echo -e "1\nls\nopen data/treasures/treasure_1.txt\nquit" | timeout 10s ./filesystem_explorer
if [ $? -eq 0 ]; then
    show_message "Basic functionality test passed"
else
    show_warning "Basic functionality test failed or timed out"
fi

# Test 2: File operations
echo ""
echo "2. Test file operations..."
echo -e "1\nls\nopen data/treasures/trap_file.txt\nls\nquit" | timeout 10s ./filesystem_explorer
if [ $? -eq 0 ]; then
    show_message "File operations test passed"
else
    show_warning "File operations test failed or timed out"
fi

# Test 3: Help command
echo ""
echo "3. Test help command..."
echo -e "3\n" | timeout 5s ./filesystem_explorer
if [ $? -eq 0 ]; then
    show_message "Help command test passed"
else
    show_warning "Help command test failed or timed out"
fi

# Test 4: Ranking system
echo ""
echo "4. Test ranking system..."
echo -e "2\n" | timeout 5s ./filesystem_explorer
if [ $? -eq 0 ]; then
    show_message "Ranking system test passed"
else
    show_warning "Ranking system test failed or timed out"
fi

# Test 5: Admin features (if running as root)
if [ "$EUID" -eq 0 ]; then
    echo ""
    echo "5. Test admin features..."
    echo -e "1\ncreate_virus 1\nscan\nquit" | timeout 10s ./filesystem_explorer
    if [ $? -eq 0 ]; then
        show_message "Admin features test passed"
    else
        show_warning "Admin features test failed or timed out"
    fi
else
    show_warning "Skipping admin features test (not running as root)"
fi

echo ""
show_message "All tests completed!"