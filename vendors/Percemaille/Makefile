CC = gcc
CFLAGS = -Wall -ansi -pedantic
MAIN = Percemaille.c

all : main library

main : 
	$(CC) $(CFLAGS) -c $(MAIN) -o Percemaille.o

library : Percemaille.o
	ar rcs percemaille.lib Percemaille.o
	ranlib percemaille.lib
