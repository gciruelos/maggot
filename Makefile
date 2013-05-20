#Version variables
VERSION = 1
PATCHLEVEL = 0
SUBLEVEL = 0

#Some variables
CC = gcc
CFLAGS = -c -Wall -O3

all: maggot

gonsnake: maggot.o
	$(CC) maggot.o -o maggot

gonsnake.o: gonsnake.c
	$(CC) $(CFLAGS) maggot.c
clean:
	rm -rf *o maggot
