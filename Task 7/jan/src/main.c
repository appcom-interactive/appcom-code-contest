#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif
#include "world/world.h"
#include "util/util.h"
#include "menu/main-menu.h"

int mustReRender(GameContext* ctx) {
  return ctx->currentState->mustRender || ctx->forceReRender;
}

void main_loop(void* arg) {
  GameContext* context = (GameContext*)arg;
  SDL_Event event;
  context->ticks = SDL_GetTicks();
  while (SDL_PollEvent(&event)) {
    context->currentState->onEvent(context, &event);
  }
  if (context->currentState->onBeforeRender != NULL) {
    context->currentState->onBeforeRender(context);
  }
  if (mustReRender(context)) {
    SDL_SetRenderDrawColor(context->renderer, 174, 222, 203, 255);
    SDL_RenderClear(context->renderer);
    context->currentState->onRender(context);
    SDL_RenderPresent(context->renderer);
  }
}

int main(int argc, char** argv) {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
  IMG_Init(IMG_INIT_PNG);
  SDL_Window *window = SDL_CreateWindow(
    "Hello",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    1920,
    1080,
    #ifdef EMSCRIPTEN
    0
    #else
    SDL_WINDOW_FULLSCREEN
    #endif
  );

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  GameContext* context = createContext(renderer, window, NULL);
  pushState(context, createMainMenuState());
  #ifdef EMSCRIPTEN
    emscripten_set_main_loop_arg(main_loop, (void*)context, -1, 1);
  #else
    while(context->running) {
      main_loop((void*)context);
    }
  #endif
  free(context->currentState);
  free(context);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  TTF_Quit();
  IMG_Quit();
  return 0;
}
