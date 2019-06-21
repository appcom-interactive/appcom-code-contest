#!/bin/bash

LINKER=$(pkg-config --cflags --libs SDL2_image SDL2_ttf)
if [ -d obj_standalone ]; then
  rm -rf obj_standalone/*.o
fi
mkdir -p obj_standalone
gcc -g -c -Wall src/util/list.c -o obj_standalone/list.o ${LINKER}
gcc -g -c -Wall src/util/map.c -o obj_standalone/map.o ${LINKER}
gcc -g -c -Wall src/util/sdl-util.c -o obj_standalone/sdl-util.o ${LINKER}
gcc -g -c -Wall src/menu/menu.c -o obj_standalone/menu.o ${LINKER}
gcc -g -c -Wall src/state/game-context.c -o obj_standalone/game-context.o ${LINKER}
gcc -g -c -Wall src/state/state.c -o obj_standalone/state.o ${LINKER}
gcc -g -c -Wall src/menu/main-menu.c -o obj_standalone/main-menu.o ${LINKER}
gcc -g -c -Wall src/menu/world-menu.c -o obj_standalone/world-menu.o ${LINKER}
gcc -g -c -Wall src/world/world.c -o obj_standalone/world.o ${LINKER}
gcc -g -c -Wall src/game/game-renderer.c -o obj_standalone/game-renderer.o ${LINKER}
gcc -g -c -Wall src/game/game.c -o obj_standalone/game.o ${LINKER}
gcc -g -Wall src/main.c \
  obj_standalone/list.o \
  obj_standalone/map.o \
  obj_standalone/sdl-util.o \
  obj_standalone/state.o \
  obj_standalone/game-context.o \
  obj_standalone/menu.o \
  obj_standalone/main-menu.o \
  obj_standalone/world-menu.o \
  obj_standalone/world.o \
  obj_standalone/game-renderer.o \
  obj_standalone/game.o \
  -o game \
  ${LINKER}
