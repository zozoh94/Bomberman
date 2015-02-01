CFLAGS = -W -Wall -std=gnu99 -O4
DEP = audio.o bomb.o player.o game.o map.o sprite.o IA.o astar.o

UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
CC = gcc
else
CC = mingw32-gcc
CFLAGS += -Iinclude/
LDFLAGS = -Llib/ -s -lmingw32 -lSDLmain -lSDL.dll -luser32 -lgdi32 -lwinmm -ldxguid -mwindows
endif

ifeq (${DEBUG}, 1)
	CFLAGS += -g -O0
endif

LDFLAGS += -ljson -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer

# all
all: $(DEP)
	$(CC) $(addprefix bin/, $(DEP)) src/bomberman.c -o bin/Bomberman $(CFLAGS) $(LDFLAGS)

%.o: src/%.c
	$(CC) -c $< -o bin/$@ $(CFLAGS)

#clean
clean:
	rm -rf bin/*.bak; rm -rf bin/*.o

#mrpropre
mrproper: clean
	rm -f bin/Bomberman
	rm -f bin/*.exe
