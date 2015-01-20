JSON_C_DIR=usr/local
CC = gcc
CFLAGS = -Wall -std=gnu99 -O4 -I$(JSON_C_DIR)/include/json-c
LDFLAGS = -lSDL -L$(JSON_C_DIR)/lib -ljson-c

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

#clean
clean:
	rm -rf bin/*.bak rm -rf bin/*.o

#mrproper
mrproper: clean
	rm -rf bin/
