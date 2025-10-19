#ifndef STORAGE_H
#define STORAGE_H

#include <stddef.h>
#include "../../include/error_codes.h"

ErrorCode storage_init(void);
void storage_shutdown(void);

#endif // STORAGE_H
