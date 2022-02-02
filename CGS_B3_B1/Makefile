CC=gcc
CFLAGS=-c
CCFLAGSDEBUG=-g
LFLAGS=

all: shell

shell : shell.o mymemory.o
	$(CC) $(LFLAGS) -o shell shell.o mymemory.o

shell.o : shell.c
	$(CC) $(CFLAGS) shell.c

mymemory : mymemory.o
	$(CC) $(LFLAGS) -o mymemory mymemory.o

mymemorydebug : mymemory.o
	$(CC) $(CFLAGSDEBUG) -o mymemory mymemory.o

mymemory-o : mymemory.c mymemory.h
	$(CC) -o mymemory.c