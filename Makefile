CC=cc

all:
	$(CC) `pkg-config --cflags gtk4` main.c -o ./build/take-break -ggdb `pkg-config --libs gtk4`
clear:
	rm -rf ./build/take-break core*
