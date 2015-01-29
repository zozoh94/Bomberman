JSON_C_DIR=usr/local
<<<<<<< HEAD
CC = gcc
CFLAGS = -Wall -std=gnu99 -O4 -I$(JSON_C_DIR)/include/json-c -g
LDFLAGS = -lSDL -lSDL_image -lSDL_ttf -L$(JSON_C_DIR)/lib/ljson-c
=======
CC = gcc 
CFLAGS = -W -Wall -std=gnu99 -O4 -I$(JSON_C_DIR)/include/json-c
LDFLAGS = -lSDL -lSDL_image -L$(JSON_C_DIR)/lib -ljson-c
>>>>>>> 529454a6195966919d040931ad3be15c229d4a75
DEP = bomb.o player.o game.o map.o sprite.o


ifeq (${DEBUG}, 1)
    CFLAGS += -g
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
