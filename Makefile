#Version variables
VERSION = 1
PATCHLEVEL = 0
SUBLEVEL = 0

#Some variables
CC = gcc
CFLAGS = -c -Wall -O3

all: maggot

maggot: maggot.o
	$(CC) maggot.o -o maggot

maggot.o: src/maggot.c src/maggot.h
	$(CC) $(CFLAGS) src/maggot.c

clean:
	rm -rf *o maggot
