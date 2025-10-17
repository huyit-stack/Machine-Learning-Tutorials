#!/bin/bash

# Script demo cho Ubuntu Login System
echo "=== DEMO UBUNTU LOGIN SYSTEM ==="
echo ""

# Biên dịch chương trình
echo "1. Biên dịch chương trình..."
make clean
make
echo ""

# Test đăng ký user
echo "2. Test đăng ký user mới..."
echo -e "2\nadmin\nAdministrator\nadmin@example.com\nadmin123\n" | ./login
echo ""

# Test đăng nhập
echo "3. Test đăng nhập..."
echo -e "1\nadmin\nadmin123\n" | ./login
echo ""

# Test đăng nhập sai
echo "4. Test đăng nhập với mật khẩu sai..."
echo -e "1\nadmin\nwrongpassword\n" | ./login
echo ""

# Test đăng ký user trùng tên
echo "5. Test đăng ký user trùng tên..."
echo -e "2\nadmin\nAnother User\nuser@example.com\npassword123\n" | ./login
echo ""

# Test đăng ký user thứ 2
echo "6. Test đăng ký user thứ 2..."
echo -e "2\nuser1\nUser One\nuser1@example.com\nuser123\n" | ./login
echo ""

# Test đăng nhập user thứ 2
echo "7. Test đăng nhập user thứ 2..."
echo -e "1\nuser1\nuser123\n" | ./login
echo ""

echo "=== DEMO HOÀN THÀNH ==="
echo "Kiểm tra file users.dat đã được tạo:"
ls -la users.dat 2>/dev/null && echo "✓ File users.dat tồn tại" || echo "✗ File users.dat không tồn tại"
echo ""
echo "Kích thước file users.dat:"
wc -c users.dat 2>/dev/null || echo "File không tồn tại"