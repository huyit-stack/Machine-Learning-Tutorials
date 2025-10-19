CC := gcc
CFLAGS := -Wall -Wextra -Werror -std=c11 -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE -Iinclude -Iproject
LDFLAGS := 
SRC := \
  project/main.c \
  project/filesystem/file_manager.c \
  project/filesystem/directory_ops.c \
  project/filesystem/file_ops.c \
  project/traps/traps.c \
  project/traps/hack_system.c \
  project/traps/virus_manager.c \
  project/ui/ui.c \
  project/ui/console_ui.c \
  project/ui/command_parser.c \
  project/storage/storage.c \
  project/storage/game_state.c \
  project/storage/ranking_system.c \
  project/storage/save_load.c

OBJ := $(SRC:.c=.o)
BIN := bin/fse

.PHONY: all clean run test

all: $(BIN)

$(BIN): $(OBJ)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

run: $(BIN)
	./$(BIN)

test:
	@echo "No unit tests implemented yet."

clean:
	rm -f $(OBJ) $(BIN)

