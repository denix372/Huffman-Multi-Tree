CC=gcc
CFLAGS=-Wall -Wextra
SRC=tema2.c
BIN=tema2

all: build

build:
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

clean:
	rm -f $(BIN)
