#include "options-menu.h"
#include "main-menu.h"

void onOptionsMenuEnter(GameContext* ctx) {
  printf("OptionsMenu->onEnter()\n");
}

void onOptionsMenuRender(GameContext* ctx) {
  OptionsMenu* m = (OptionsMenu*)ctx->currentState->stateExt;
  SDL_Rect rect;
  rect.y = 200;
  ListEntry* current = (ListEntry*)m->def->entries->head;
  while (current != NULL) {
    MenuEntry * entry = (MenuEntry*)current->data;
    renderMenuEntry(ctx->renderer, entry, &rect);
    current = current->next;
  }
}

void onOptionsMenuEvent(GameContext* ctx, SDL_Event* event) {
  OptionsMenu* menu = (OptionsMenu*)ctx->currentState->stateExt;
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
  } else if (event->key.keysym.sym == SDLK_UP) {
    if (selectedEntry->previous != NULL) {
      next = selectedEntry->previous;
    } else {
      next = menu->def->entries->tail;
    }
    ((MenuEntry*)selectedEntry->data)->selected = 0;
    ((MenuEntry*)next->data)->selected = 1;
  } else if (event->key.keysym.sym == SDLK_RETURN) {
    if (((MenuEntry*)selectedEntry->data)->id == OPTIONS_MENU_BACK) {
      pushState(ctx, createMainMenuState());
    }
  }
  printf("OptionsMenu->onEvent()\n");
}

void onOptionsMenuExit(GameContext* ctx) {
  OptionsMenu* menu = (OptionsMenu*)ctx->currentState->stateExt;
  TTF_CloseFont(menu->entryFont);
  freeList(menu->def->entries);
  free(menu->def);
  free(menu);
}

State* createOptionsMenuState() {
  State* state = createState();
  OptionsMenu* m = (OptionsMenu*)malloc(sizeof(OptionsMenu));
  m->entryFont = TTF_OpenFont("./assets/fonts/UbuntuMono-R.ttf", 90);
  m->def = createMenuDef();
  pushListEntry(m->def->entries, createMenuEntry(OPTIONS_MENU_RESOLUTION, "Auflösung", m->entryFont, 1));
  pushListEntry(m->def->entries, createMenuEntry(OPTIONS_MENU_BACK, "Zurück", m->entryFont, 0));

  state->onEnter = &onOptionsMenuEnter;
  state->onEvent = &onOptionsMenuEvent;
  state->onRender = &onOptionsMenuRender;
  state->onExit = &onOptionsMenuExit;
  state->stateExt = (void*)m;
  return state;
}
