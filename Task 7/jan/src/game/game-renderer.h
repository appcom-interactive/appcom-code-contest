#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include <SDL2/SDL.h>
#include "game.h"
#include "../state/game-context.h"

typedef struct GameState GameState;

typedef struct GameRenderer {
  SDL_Texture* backgroundTexture;
  SDL_Texture* charTilesTexture;
  SDL_Texture* gameTexture;
  SDL_Texture* worldTexture;
  SDL_Texture* worldTilesTexture;
  SDL_Surface* worldTilesSurface;
  SDL_Surface* worldSurface;
  TTF_Font* winnerFont;
} GameRenderer;

GameRenderer* makeGameRenderer(GameContext*);
void renderGame(GameRenderer*, GameContext*);
void freeGameRenderer(GameRenderer*);

void determineBlitRectBySurroundingBlocks(GameState* gameState, SDL_Rect* blitRect, uint8_t* surroundingBlocks);
#endif
