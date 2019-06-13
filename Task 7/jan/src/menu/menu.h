#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "../util/util.h"

typedef enum {
  MAIN_MENU_START,
  MAIN_MENU_OPTIONS,
  MAIN_MENU_EXIT,

  OPTIONS_MENU_RESOLUTION,
  OPTIONS_MENU_CLEAR_SAVES,
  OPTIONS_MENU_BACK,

  WORLD_MENU_BACK
} MenuItemID;

typedef struct MenuEntry {
  int id;
  const char * name;
  int selected;
  TTF_Font* font;
} MenuEntry;

typedef struct MenuDefinition {
  List* entries;
} MenuDefinition;

MenuDefinition* createMenuDef();
MenuEntry* createMenuEntry(int, const char*, TTF_Font*, int);
int findSelectedEntry(void*, void*);
void renderMenuEntry(SDL_Renderer*, MenuEntry*, SDL_Rect*);

#endif
