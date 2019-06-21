#include "main-menu.h"
#include "world-menu.h"

#define MAIN_MENU_START 0
#define MAIN_MENU_EXIT 1

void freeEntry(void* entry) {
  MenuEntry* e = (MenuEntry*)entry;
  free(e);
}

State* createMainMenuState() {
  State* state = createState();
  if (!TTF_WasInit() && TTF_Init() == -1) {
    printf("%s\n", TTF_GetError());
    return NULL;
  }
  SDL_Color white = {255,255,255};
  MainMenu * m = (MainMenu*)malloc(sizeof(MainMenu));
  m->color = white;
  m->headlineFont = TTF_OpenFont("./assets/fonts/Ubuntu-R.ttf", 150);
  m->entryFont = TTF_OpenFont("./assets/fonts/Ubuntu-R.ttf", 70);
  m->def = createMenuDef();
  pushListEntry(m->def->entries, (void*)createMenuEntry(MAIN_MENU_START, "Start", m->entryFont, 1));
#ifndef EMSCRIPTEN
  pushListEntry(m->def->entries, (void*)createMenuEntry(MAIN_MENU_EXIT, "Exit", m->entryFont, 0));
#endif
  state->onBeforeRender = &onBeforeMainMenuRender;
  state->onRender = &onMainMenuRender;
  state->onEvent = &onMainMenuEvent;
  state->onEnter = &onMainMenuEnter;
  state->onExit = &onMainMenuExit;
  state->stateExt = (void*)m;
  return state;
}

void onMainMenuEnter(GameContext* ctx) {
  MainMenu* menu = (MainMenu*)ctx->currentState->stateExt;
  SDL_Surface* backgroundSurface = IMG_Load("./assets/gfx/plx-4.png");
  menu->backgroundTexture = SDL_CreateTextureFromSurface(ctx->renderer, backgroundSurface);
  menu->menuHeadlineTexture = createTextTexture(ctx->renderer, "JUMPY", menu->color, menu->headlineFont, &menu->menuHeadlineWidth, &menu->menuHeadlineHeight);
  SDL_FreeSurface(backgroundSurface);
}

void onMainMenuEvent(GameContext* ctx, SDL_Event* event) {
  MainMenu* menu = (MainMenu*)ctx->currentState->stateExt;
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
    if (id == MAIN_MENU_START) {
      pushState(ctx, createWorldMenuState());
    } else if(id == MAIN_MENU_EXIT) {
      ctx->currentState->onExit(ctx);
      ctx->running = 0;
    }
  }
}

void onBeforeMainMenuRender(GameContext* ctx) {}

void onMainMenuRender(GameContext* ctx) {
  MainMenu* menu = (MainMenu*)ctx->currentState->stateExt;
  SDL_RenderCopy(ctx->renderer, menu->backgroundTexture, NULL, NULL);

  SDL_Rect rect;
  rect.x = 1920 / 2 - menu->menuHeadlineWidth / 2;
  rect.y = 20;
  rect.w = menu->menuHeadlineWidth;
  rect.h = menu->menuHeadlineHeight;

  SDL_RenderCopy(ctx->renderer, menu->menuHeadlineTexture, NULL, &rect);
  ctx->currentState->mustRender = 0;

  rect.y = 200;
  ListEntry * current = menu->def->entries->head;
  while (current != NULL) {
    MenuEntry * entry = (MenuEntry*)current->data;
    renderMenuEntry(ctx->renderer, entry, &rect);
    current = current->next;
  }
}

void onMainMenuExit(GameContext* ctx) {
  State* state = (State*)ctx->currentState;
  MainMenu* menu = (MainMenu*)state->stateExt;
  SDL_DestroyTexture(menu->backgroundTexture);
  SDL_DestroyTexture(menu->menuHeadlineTexture);
  TTF_CloseFont(menu->headlineFont);
  TTF_CloseFont(menu->entryFont);
  freeList(menu->def->entries);
  free(menu->def);
  free(menu);
}
