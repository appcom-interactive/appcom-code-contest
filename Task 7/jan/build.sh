#!/bin/bash

emcc -Wall src/util/list.c -o obj/list.o
emcc -Wall src/util/map.c -o obj/map.o
emcc -Wall src/util/sdl-util.c -o obj/sdl-util.o -s USE_SDL=2 -s USE_SDL_TTF=2
emcc -Wall src/menu/menu.c -o obj/menu.o -s USE_SDL=2 -s USE_SDL_TTF=2
emcc -Wall src/state/game-context.c -o obj/game-context.o -s USE_SDL=2
emcc -Wall src/state/state.c -o obj/state.o -s USE_SDL=2
emcc -Wall src/menu/main-menu.c -o obj/main-menu.o -s USE_SDL=2 -s USE_SDL_TTF=2
emcc -Wall src/menu/world-menu.c -o obj/world-menu.o -s USE_SDL=2 -s USE_SDL_TTF=2
emcc -Wall src/world/world.c -o obj/world.o -s USE_SDL=2
emcc -Wall src/game/game-renderer.c -o obj/game-renderer.o -s USE_SDL=2 -s USE_SDL_TTF=2
emcc -Wall src/game/game.c -o obj/game.o -s USE_SDL=2 -s USE_SDL_TTF=2
emcc -Wall src/main.c \
  obj/list.o \
  obj/map.o \
  obj/sdl-util.o \
  obj/state.o \
  obj/game-context.o \
  obj/menu.o \
  obj/main-menu.o \
  obj/world-menu.o \
  obj/world.o \
  obj/game-renderer.o \
  obj/game.o \
  -o game.html \
  -s USE_SDL=2 \
  -s USE_SDL_TTF=2 \
  -s USE_SDL_IMAGE=2 \
  -s SDL2_IMAGE_FORMATS='["png"]' \
  -s TOTAL_MEMORY=33554432 \
  --shell-file template.html \
  --preload-file ./assets@/assets
mv game.* build/
serve ./build
