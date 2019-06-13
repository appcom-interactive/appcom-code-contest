#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include <SDL2/SDL.h>
#include "state.h"

typedef struct GameContext {
  SDL_Renderer * renderer;
  SDL_Window * window;
  Uint32 ticks;
  State * currentState;
  int running;
  int forceReRender;
} GameContext;

GameContext* createContext(SDL_Renderer*, SDL_Window*, State*);
void pushState(GameContext*, State*);

#endif
