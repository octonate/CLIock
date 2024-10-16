CFLAGS = -Wall -pedantic
LFLAGS = -lm
CC=gcc

all: myfelch

myfelch: myfelch.c clock.c clock.h colors.h
	$(CC) $(CFLAGS) $(LFLAGS) myfelch.c -o myfelch

clean: 
	rm myfelch
