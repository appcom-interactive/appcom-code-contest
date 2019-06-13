#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <SDL2/SDL.h>

typedef struct GameContext GameContext;

typedef void (*stateRenderFn)(GameContext*);
typedef void (*stateBeforeRenderFn)(GameContext*);
typedef void (*stateEventFn)(GameContext*, SDL_Event*);
typedef void (*stateExitFn)(GameContext*);
typedef void (*stateEnterFn)(GameContext*);

typedef struct _game_state {
  stateBeforeRenderFn onBeforeRender;
  stateRenderFn onRender;
  stateEventFn onEvent;
  stateExitFn onExit;
  stateEnterFn onEnter;
  int mustRender;
  void* stateExt;
} State;

State * createState();

#endif
