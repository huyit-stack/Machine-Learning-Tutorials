#ifndef CONSTANTS_H
#define CONSTANTS_H

// Game constants
#define MAX_PATH_LEN 256
#define MAX_FILENAME_LEN 64
#define MAX_COMMAND_LEN 128
#define MAX_PLAYER_NAME 32
#define MAX_RECORDS 100
#define MAX_VIRUSES 10
#define MAX_HACK_ATTEMPTS 3

// File permissions
#define PERM_READ     (S_IRUSR | S_IRGRP | S_IROTH)
#define PERM_WRITE    (S_IWUSR | S_IWGRP | S_IWOTH)
#define PERM_EXEC     (S_IXUSR | S_IXGRP | S_IXOTH)

// XOR key for treasure decoding
#define TREASURE_XOR_KEY 0x42

// Game messages
#define MSG_WELCOME "Chào mừng đến với File System Adventure Game!"
#define MSG_GAME_OVER "Game Over! Bạn đã hết mạng sống!"
#define MSG_LEVEL_UP "Chúc mừng! Bạn đã lên cấp!"
#define MSG_TREASURE_FOUND "Tìm thấy kho báu! +100 điểm"
#define MSG_TRAP_HIT "Bẫy! -50 điểm và -1 mạng"
#define MSG_VIRUS_KILLED "Virus đã bị tiêu diệt! +200 điểm"
#define MSG_HACK_SUCCESS "Hack thành công! +50 điểm"

// Color codes for terminal
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"
#define COLOR_BOLD    "\033[1m"

#endif // CONSTANTS_H