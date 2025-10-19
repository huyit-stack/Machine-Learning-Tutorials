#ifndef ERROR_CODES_H
#define ERROR_CODES_H

typedef enum ErrorCode {
    EC_OK = 0,
    EC_ERR = -1,
    EC_NOT_FOUND = -2,
    EC_PERMISSION = -3,
    EC_INVALID = -4,
    EC_IO = -5,
    EC_BUSY = -6
} ErrorCode;

#endif // ERROR_CODES_H
