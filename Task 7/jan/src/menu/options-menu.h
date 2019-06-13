#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H

#include <SDL2/SDL_ttf.h>
#include "menu.h"
#include "../util/state.h"
#include "../util/util.h"
#include "../util/game-context.h"

typedef struct OptionsMenu {
  TTF_Font* entryFont;
  MenuDefinition* def;
} OptionsMenu;

State* createOptionsMenuState();
void onOptionsMenuRender(GameContext*);
void onOptionsMenuEvent(GameContext*, SDL_Event*);
void onOptionsMenuEnter(GameContext*);
void onOptionsMenuExit(GameContext*);

#endif