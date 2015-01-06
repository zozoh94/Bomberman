CC = gcc
FLAGS = -Wall -std=gnu99 -lSDL

# all
all: main
	$(CC) bin/bomberman.o -o bin/Bomberman $(FLAGS)

#main
main:
	$(CC) -c src/bomberman.c -o bin/bomberman.o $(FLAGS)

#clean
clean:
	rm -rf bin/*.bak rm -rf bin/*.o

#mrproper
mrproper: clean
	rm -rf bin/
