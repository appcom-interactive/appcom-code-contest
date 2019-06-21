#ifndef WORLD_MENU_H
#define WORLD_MENU_H

#include <SDL2/SDL_ttf.h>
#include "menu.h"
#include "../state/state.h"
#include "../util/util.h"
#include "../state/game-context.h"
#include "../world/world.h"

typedef struct WorldMenu {
  TTF_Font* entryFont;
  Map* worlds;
  MenuDefinition* def;
  SDL_Texture* backgroundTexture;
} WorldMenu;

State* createWorldMenuState();
void onWorldMenuRender(GameContext*);
void onWorldMenuEvent(GameContext*, SDL_Event*);
void onWorldMenuEnter(GameContext*);
void onWorldMenuExit(GameContext*);
void freeWorld(World *);

#endif