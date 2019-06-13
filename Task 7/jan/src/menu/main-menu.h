#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include "menu.h"
#include "../util/util.h"
#include "../util/state.h"
#include "../util/game-context.h"

typedef struct MainMenu {
  TTF_Font* headlineFont;
  TTF_Font* entryFont;
  MenuDefinition* def;
  SDL_Color color;
  SDL_Texture* backgroundTexture;
  int menuHeadlineWidth;
  int menuHeadlineHeight;
  SDL_Texture* menuHeadlineTexture;
} MainMenu;

State* createMainMenuState();
void onBeforeMainMenuRender(GameContext*);
void onMainMenuRender(GameContext*);
void onMainMenuEvent(GameContext*, SDL_Event*);
void onMainMenuEnter(GameContext*);
void onMainMenuExit(GameContext*);

#endif
