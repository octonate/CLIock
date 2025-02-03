CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99
LFLAGS = -lm
BINDIR = /usr/bin
CC=gcc

all: cliock

cliock: cliock.c clock.c clock.h colors.h
	$(CC) $(CFLAGS) $(LFLAGS) cliock.c -o cliock

install: all
	install -D cliock $(BINDIR)/cliock

uninstall: 
	rm -f $(BINDIR)/cliock

clean: 
	rm cliock
