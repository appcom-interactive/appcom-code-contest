#include "game-context.h"
#include <stdlib.h>

GameContext* createContext(SDL_Renderer* r, SDL_Window* w, State* s) {
  GameContext* ctx = (GameContext*)malloc(sizeof(GameContext));
  ctx->renderer = r;
  ctx->window = w;
  ctx->currentState = s;
  ctx->ticks = SDL_GetTicks();
  ctx->forceReRender = 0;
  ctx->running = 1;
  return ctx;
}

void pushState(GameContext* ctx, State* s) {
  printf("pushState()\n");
  if (ctx->currentState == NULL) {
    ctx->currentState = s;
    if (ctx->currentState->onEnter != NULL) {
      printf("pushState->onEnter()\n");
      ctx->currentState->onEnter(ctx);
    }
    return;
  }
  if (ctx->currentState->onExit != NULL) {
    printf("pushState->onExit()\n");
    ctx->currentState->onExit(ctx);
  }
  free(ctx->currentState);
  ctx->currentState = s;
  if (ctx->currentState->onEnter != NULL) {
    printf("pushState->onEnter()\n");
    ctx->currentState->onEnter(ctx);
  }
}
