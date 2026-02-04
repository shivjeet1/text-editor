NAME := jadu

SRC_DIR := code/src
INC_DIR := code/headers
BUILD_DIR := build

CC := gcc
CFLAGS := -Wall -Wextra -std=c11 -I$(INC_DIR)
LDFLAGS :=

PREFIX ?= /usr/local
BINDIR := $(PREFIX)/bin

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)


all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

install: $(NAME)
	install -m 0755 $(NAME) $(BINDIR)/$(NAME)

uninstall:
	rm -f $(BINDIR)/$(NAME)

clean:
	rm -rf $(BUILD_DIR) $(NAME)

rebuild: clean all

.PHONY: all install uninstall clean rebuild
