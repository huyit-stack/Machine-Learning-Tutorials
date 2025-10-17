#!/bin/bash

# Build script cho File Systems Explorer
echo "=== BUILDING FILE SYSTEMS EXPLORER ==="

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

show_message "Dependencies OK"

# Clean trước khi build
echo ""
echo "2. Clean previous build..."
make clean

# Build chương trình
echo ""
echo "3. Biên dịch chương trình..."
if make; then
    show_message "Build thành công"
else
    show_error "Build thất bại"
    exit 1
fi

# Tạo thư mục data nếu chưa có
echo ""
echo "4. Tạo thư mục data..."
mkdir -p data/treasures data/config
show_message "Thư mục data đã được tạo"

# Tạo test files
echo ""
echo "5. Tạo test files..."
make test
show_message "Test files đã được tạo"

echo ""
show_message "Build hoàn thành!"
echo "Chạy chương trình: ./filesystem_explorer"