CC = gcc
CFLAGS = -W -Wall -std=gnu99 -O4 -Iinclude/json-c
LDFLAGS = -ljson-c
DEP = bomb.o player.o game.o map.o sprite.o IA.o astar.o

ifeq (${WIN32}, 1)
    CFLAGS += -I/usr/include/SDL -D_GNU_SOURCE=1 -Dmain=SDL_main
    LDFLAGS += -L/usr/lib -lmingw32 -lSDLmain -lSDL -mwindows
else
    LDFLAGS += -Llib -lSDL -lSDL_image -lSDL_ttf
endif
ifeq (${WIN32_CROSS_PLATFORM}, 1)
    CC = x86_64-w64-mingw32-gcc
endif
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
