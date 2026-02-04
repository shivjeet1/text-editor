NAME := jadu

SRC_DIR := code/src
INC_DIR := code/headers

CC := gcc
CFLAGS := -I$(INC_DIR) -Wall -Wextra -Werror 

PREFIX ?= /usr/local
BINDIR := $(PREFIX)/bin

SRCS := $(SRC_DIR)/*.c

all :
	$(CC) $(SRCS) $(CFLAGS) -o $(NAME)

install : $(NAME)
	mkdir -p $(BINDIR)
	install -m 0755 $(NAME) $(BINDIR)/$(NAME)

uninstall :
	rm -f $(BINDIR)/$(NAME)

clean :
	rm -f $(NAME)

rebuild : clean all
	
.PHONY : all install uninstall clean rebuild

