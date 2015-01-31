CC = gcc
CFLAGS = -W -Wall -std=gnu99 -O4
LDFLAGS = -ljson -lSDL -lSDL_image -lSDL_ttf
DEP = bomb.o player.o game.o map.o sprite.o IA.o astar.o

ifeq (${DEBUG}, 1)
    CFLAGS += -g -O0
endif

# all
all: $(DEP)
	$(CC) $(addprefix bin/, $(DEP)) src/bomberman.c -o bin/Bomberman $(CFLAGS) $(LDFLAGS)

%.o: src/%.c
	$(CC) -c $< -o bin/$@ $(CFLAGS)

#clean
clean:
	rm -rf bin/*.bak; rm -rf bin/*.o

#mrpropre
mrpropre: clean
	rm -rf bin/
