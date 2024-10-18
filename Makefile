CFLAGS = -Wall -pedantic
LFLAGS = -lm
CC=gcc

all: cliock

cliock: cliock.c clock.c clock.h colors.h
	$(CC) $(CFLAGS) $(LFLAGS) cliock.c -o cliock

clean: 
	rm cliock
