#ifndef ERROR_CODES_H
#define ERROR_CODES_H

// Error codes
#define SUCCESS 0
#define ERROR_FILE_NOT_FOUND -1
#define ERROR_PERMISSION_DENIED -2
#define ERROR_INVALID_INPUT -3
#define ERROR_MEMORY_ALLOCATION -4
#define ERROR_SYSTEM_CALL -5
#define ERROR_INVALID_STATE -6
#define ERROR_LIMIT_EXCEEDED -7
#define ERROR_ADMIN_REQUIRED -8

// Error messages
#define ERR_MSG_FILE_NOT_FOUND "File không tồn tại"
#define ERR_MSG_PERMISSION_DENIED "Không có quyền truy cập"
#define ERR_MSG_INVALID_INPUT "Dữ liệu đầu vào không hợp lệ"
#define ERR_MSG_MEMORY_ALLOCATION "Lỗi cấp phát bộ nhớ"
#define ERR_MSG_SYSTEM_CALL "Lỗi system call"
#define ERR_MSG_INVALID_STATE "Trạng thái không hợp lệ"
#define ERR_MSG_LIMIT_EXCEEDED "Vượt quá giới hạn cho phép"
#define ERR_MSG_ADMIN_REQUIRED "Cần quyền Admin"

#endif // ERROR_CODES_H