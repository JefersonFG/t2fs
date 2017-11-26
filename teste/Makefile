CC = gcc
CFLAGS = -m32 -Wall

LIB_DIR = ../lib
LIB = ../lib/libt2fs.a

MAIN_SRC = main.c

all: main

main: $(MAIN_SRC) $(LIB)
	$(CC) $(CFLAGS) -o main $(MAIN_SRC) -L$(LIB_DIR) -lt2fs

clean:
	-rm -f main
