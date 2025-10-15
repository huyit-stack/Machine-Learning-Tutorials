PROJECT := login-gtk-pam
BUILD_DIR := build
SRC := src/main.c src/pam_auth.c
OBJS := $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SRC))

CC := gcc
PKGS := gtk+-3.0
CSTD := -std=c11
CFLAGS := $(CSTD) -Wall -Wextra -Wpedantic -O2 $(shell pkg-config --cflags $(PKGS)) -Iinclude
LIBS := $(shell pkg-config --libs $(PKGS)) -lpam

.PHONY: all clean run

all: $(BUILD_DIR)/$(PROJECT)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/$(PROJECT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

run: all
	./$(BUILD_DIR)/$(PROJECT)

clean:
	rm -rf $(BUILD_DIR)
