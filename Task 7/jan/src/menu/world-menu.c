#include "world-menu.h"
#include "main-menu.h"
#include "../game/game.h"

#define WORLD_MENU_BACK 99

int worldCmpFn(void* data, void* compareArg) {
  MapEntry* entry = (MapEntry*)data;
  int key = *((int*)entry->key);
  int cmpI = *((int*)compareArg);
  return key == cmpI;
}

void freeWorldMapEntry(void* data) {
  MapEntry* entry = (MapEntry*)data;
  freeWorld((World*)entry->value);
  free(entry);
}

State* createWorldMenuState() {
  State* state = createState();
  WorldMenu* menu = (WorldMenu*)malloc(sizeof(WorldMenu));
  menu->entryFont = TTF_OpenFont("./assets/fonts/Ubuntu-R.ttf", 70);
  menu->worlds = createMap(MAP_TYPE_ENUM, &freeWorldMapEntry, NULL);
  mapPut(menu->worlds, (void*)WORLD_1, (void*)loadWorld(WORLD_1, WORLD_LOADING_STATUS_PARTIAL));
  mapPut(menu->worlds, (void*)WORLD_2, (void*)loadWorld(WORLD_2, WORLD_LOADING_STATUS_PARTIAL));
  mapPut(menu->worlds, (void*)WORLD_3, (void*)loadWorld(WORLD_3, WORLD_LOADING_STATUS_PARTIAL));
  mapPut(menu->worlds, (void*)WORLD_4, (void*)loadWorld(WORLD_4, WORLD_LOADING_STATUS_PARTIAL));
  mapPut(menu->worlds, (void*)WORLD_5, (void*)loadWorld(WORLD_5, WORLD_LOADING_STATUS_PARTIAL));
  menu->def = createMenuDef();
  pushListEntry(menu->def->entries, createMenuEntry(WORLD_1, ((World*)mapGet(menu->worlds, (void*)WORLD_1))->name, menu->entryFont, 1));
  pushListEntry(menu->def->entries, createMenuEntry(WORLD_2, ((World*)mapGet(menu->worlds, (void*)WORLD_2))->name, menu->entryFont, 0));
  pushListEntry(menu->def->entries, createMenuEntry(WORLD_3, ((World*)mapGet(menu->worlds, (void*)WORLD_3))->name, menu->entryFont, 0));
  pushListEntry(menu->def->entries, createMenuEntry(WORLD_4, ((World*)mapGet(menu->worlds, (void*)WORLD_4))->name, menu->entryFont, 0));
  pushListEntry(menu->def->entries, createMenuEntry(WORLD_5, ((World*)mapGet(menu->worlds, (void*)WORLD_5))->name, menu->entryFont, 0));
  pushListEntry(menu->def->entries, createMenuEntry(WORLD_MENU_BACK, "Zurück", menu->entryFont, 0));
  state->onRender = &onWorldMenuRender;
  state->onEvent = &onWorldMenuEvent;
  state->onEnter = &onWorldMenuEnter;
  state->onExit = &onWorldMenuExit;
  state->stateExt = (void*)menu;
  state->mustRender = 1;
  return state;
}

void onWorldMenuEnter(GameContext* ctx) {
  WorldMenu* menu = (WorldMenu*)ctx->currentState->stateExt;
  SDL_Surface* backgroundSurface = IMG_Load("./assets/gfx/plx-4.png");
  menu->backgroundTexture = SDL_CreateTextureFromSurface(ctx->renderer, backgroundSurface);
  SDL_FreeSurface(backgroundSurface);
}

void onWorldMenuEvent(GameContext* ctx, SDL_Event* event) {
  WorldMenu* menu = (WorldMenu*)ctx->currentState->stateExt;
  if (event->type != SDL_KEYDOWN) return;
  ListEntry* selectedEntry = findListEntry(menu->def->entries, &findSelectedEntry, NULL);
  if (selectedEntry == NULL) return;
  ListEntry* next = NULL;
  if (event->key.keysym.sym == SDLK_DOWN) {
    if (selectedEntry->next != NULL) {
      next = selectedEntry->next;
    } else {
      next = menu->def->entries->head;
    }
    ((MenuEntry*)selectedEntry->data)->selected = 0;
    ((MenuEntry*)next->data)->selected = 1;
    ctx->currentState->mustRender = 1;
  } else if (event->key.keysym.sym == SDLK_UP) {
    if (selectedEntry->previous != NULL) {
      next = selectedEntry->previous;
    } else {
      next = menu->def->entries->tail;
    }
    ((MenuEntry*)selectedEntry->data)->selected = 0;
    ((MenuEntry*)next->data)->selected = 1;
    ctx->currentState->mustRender = 1;
  } else if (event->key.keysym.sym == SDLK_RETURN) {
    int id = ((MenuEntry*)selectedEntry->data)->id;
    if (id == WORLD_MENU_BACK) {
      pushState(ctx, createMainMenuState());
      return;
    }
    pushState(ctx, createGameState(id));
  }
}

void onWorldMenuRender(GameContext* ctx) {
  WorldMenu* menu = (WorldMenu*)ctx->currentState->stateExt;
  SDL_RenderCopy(ctx->renderer, menu->backgroundTexture, NULL, NULL);
  SDL_Rect rect;
  rect.y = 100;
  ListEntry* current = (ListEntry*)menu->def->entries->head;
  while (current != NULL) {
    MenuEntry* entry = (MenuEntry*)current->data;
    renderMenuEntry(ctx->renderer, entry, &rect);
    current = current->next;
  }
  ctx->currentState->mustRender = 0;
}

void onWorldMenuExit(GameContext* ctx) {
  State* state = (State*)ctx->currentState;
  WorldMenu* menu = (WorldMenu*)state->stateExt;
  freeList(menu->def->entries);
  free(menu->def);
  freeMap(menu->worlds);
  TTF_CloseFont(menu->entryFont);
  SDL_DestroyTexture(menu->backgroundTexture);
  free(menu);
}
