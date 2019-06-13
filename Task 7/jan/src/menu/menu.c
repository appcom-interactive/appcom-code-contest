#include "menu.h"
#include "stdlib.h"

void freeMenuEntry(void* entry) {
  printf("freeMenuEntry()\n");
  MenuEntry* e = (MenuEntry*)entry;
  free(e);
}

MenuDefinition* createMenuDef() {
  MenuDefinition* d = (MenuDefinition*)malloc(sizeof(MenuDefinition));
  d->entries = createLinkedList(&freeMenuEntry);
  return d; 
}

MenuEntry* createMenuEntry(int id, const char* name, TTF_Font* font, int selected) {
  MenuEntry* e = (MenuEntry*)malloc(sizeof(MenuEntry));
  e->id = id;
  e->name = name;
  e->selected = selected;
  e->font = font;
  return e;
}

int findSelectedEntry(void* data, void* arg) {
  return ((MenuEntry*)data)->selected;
}

void renderMenuEntry(SDL_Renderer* renderer, MenuEntry* entry, SDL_Rect* rect) {
  SDL_Color white = {255,255,255};
  SDL_Color highlight = {255,255,0};
  SDL_Surface* menuEntryTpl = IMG_Load("./assets/gfx/button_template.png");
  SDL_Texture* t = renderTextToSurface(
    renderer,
    menuEntryTpl,
    entry->name,
    entry->selected ? highlight : white,
    entry->font
  );
  SDL_FreeSurface(menuEntryTpl);
  if (!t) return;
  rect->x = 1920 / 2 - menuEntryTpl->w / 2;
  rect->y = rect->y + menuEntryTpl->h + 20;
  rect->w = menuEntryTpl->w;
  rect->h = menuEntryTpl->h;
  SDL_RenderCopy(renderer, t, NULL, rect);
  SDL_DestroyTexture(t);
}
