CC = gcc
CFLAGS = -Wall -std=gnu99 -O4
LDFLAGS = -lSDL -ljson-c

ifeq (${DEBUG}, 1)
    CFLAGS += -g
endif

# all
all: map main
	$(CC) bin/map.o bin/bomberman.o -o bin/Bomberman $(CFLAGS) $(LDFLAGS)

#main
main: 
	$(CC) -c src/bomberman.c -o bin/bomberman.o $(CFLAGS)

#map
map:
	$(CC) -c src/map.c -o bin/map.o $(CFLAGS)

src/*.o: src/*.c
	*(CC) -c $< $(FLAGS)

#clean
clean:
	rm -rf bin/*.bak rm -rf bin/*.o

#mrproper
mrproper: clean
	rm -rf bin/
