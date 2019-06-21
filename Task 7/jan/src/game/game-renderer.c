#include "game-renderer.h"
#include "../util/sdl-util.h"

void determineBlitRectBySurroundingBlocks(GameState* gameState, SDL_Rect* blitRect, uint8_t* surroundingBlocks) {
  // |?| |?|
  // | |=| |
  // |?|X|?|
  if (
    surroundingBlocks[SB_TOP_CENTER] == 0 &&
    surroundingBlocks[SB_LEFT] == 0 &&
    surroundingBlocks[SB_RIGHT] == 0 &&
    surroundingBlocks[SB_BOTTOM_CENTER] == 1
  ) {
    blitRect->x = 96;
    blitRect->y = 80;
    return;
  }

  // | |X| |
  // |X|=| |
  // |X|X| |
  if (
    surroundingBlocks[SB_TOP_LEFT] == 0 &&
    surroundingBlocks[SB_TOP_CENTER] == 1 &&
    surroundingBlocks[SB_TOP_RIGHT] == 0 &&
    surroundingBlocks[SB_RIGHT] == 0 &&
    surroundingBlocks[SB_BOTTOM_RIGHT] == 0 &&
    surroundingBlocks[SB_BOTTOM_CENTER] == 1 &&
    surroundingBlocks[SB_BOTTOM_LEFT] == 1 &&
    surroundingBlocks[SB_LEFT] == 1
  ) {
    blitRect->x = 512;
    blitRect->y = 144;
    return;
  }

  // | |X| |
  // |X|=| |
  // |?|?|?|
  if (
    surroundingBlocks[SB_TOP_LEFT] == 0 &&
    surroundingBlocks[SB_TOP_CENTER] == 1 &&
    surroundingBlocks[SB_TOP_RIGHT] == 0 &&
    surroundingBlocks[SB_RIGHT] == 0 &&
    surroundingBlocks[SB_LEFT] == 1
  ) {
    blitRect->x = 512;
    blitRect->y = 144;
    return;
  }

  // | |X| |
  // | |=|X|
  // | |X|X|
  if (
    surroundingBlocks[SB_TOP_LEFT] == 0 &&
    surroundingBlocks[SB_TOP_CENTER] == 1 &&
    surroundingBlocks[SB_TOP_RIGHT] == 0 &&
    surroundingBlocks[SB_RIGHT] == 1 &&
    surroundingBlocks[SB_BOTTOM_RIGHT] == 1 &&
    surroundingBlocks[SB_BOTTOM_CENTER] == 1 &&
    surroundingBlocks[SB_BOTTOM_LEFT] == 0 &&
    surroundingBlocks[SB_LEFT] == 0
  ) {
    blitRect->x = 480;
    blitRect->y = 144;
    return;
  }

  // | |X| |
  // | |=|X|
  // |?|?|?|
  if (
    surroundingBlocks[SB_TOP_LEFT] == 0 &&
    surroundingBlocks[SB_TOP_CENTER] == 1 &&
    surroundingBlocks[SB_TOP_RIGHT] == 0 &&
    surroundingBlocks[SB_RIGHT] == 1 &&
    surroundingBlocks[SB_LEFT] == 0
  ) {
    blitRect->x = 480;
    blitRect->y = 144;
    return;
  }

  // |?|X|?|
  // | |=|X|
  // |?|?|?|
  if (
    surroundingBlocks[SB_TOP_CENTER] == 1 &&
    surroundingBlocks[SB_RIGHT] == 1 &&
    surroundingBlocks[SB_LEFT] == 0
  ) {
    blitRect->x = 0;
    blitRect->y = 40;
    return;
  }

  // |?|X|?|
  // |X|=| |
  // |?|?|?|
  if (
    surroundingBlocks[SB_TOP_CENTER] == 1 &&
    surroundingBlocks[SB_LEFT] == 1 &&
    surroundingBlocks[SB_RIGHT] == 0
  ) {
    blitRect->x = 48;
    blitRect->y = 40;
    return;
  }

  // |?|X|?|
  // | |=| |
  // |?|X|?|
  if (
    surroundingBlocks[SB_TOP_CENTER] == 1 &&
    surroundingBlocks[SB_LEFT] == 0 &&
    surroundingBlocks[SB_BOTTOM_CENTER] == 1 &&
    surroundingBlocks[SB_RIGHT] == 0
  ) {
    blitRect->x = 544;
    blitRect->y = 122;
    return;
  }

  // | | |?|
  // | |=|?|
  // |?|X|?|
  if (
    surroundingBlocks[SB_TOP_CENTER] == 0 &&
    surroundingBlocks[SB_TOP_LEFT] == 0 &&
    surroundingBlocks[SB_LEFT] == 0
  ) {
    blitRect->x = 0;
    blitRect->y = 16;
    return;
  }

  // |?|X| |
  // |?|=|X|
  // |?|?|?|
  if (
    surroundingBlocks[SB_TOP_CENTER] == 1 &&
    surroundingBlocks[SB_RIGHT] == 1 &&
    surroundingBlocks[SB_TOP_RIGHT] == 0
  ) {
    blitRect->x = 176;
    blitRect->y = 256;
    return;
  }

  // | |X|?|
  // |X|=|?|
  // |?|?|?|
  if (
    surroundingBlocks[SB_TOP_CENTER] == 1 &&
    surroundingBlocks[SB_LEFT] == 1 &&
    surroundingBlocks[SB_TOP_LEFT] == 0
  ) {
    blitRect->x = 240;
    blitRect->y = 256;
    return;
  }

  // |?|?|?|
  // |X|=| |
  // |?|?|X|
  if (
    surroundingBlocks[SB_LEFT] == 1 &&
    surroundingBlocks[SB_RIGHT] == 0 &&
    surroundingBlocks[SB_BOTTOM_RIGHT] == 1
  ) {
    blitRect->x = 48;
    blitRect->y = 16;
    return;
  }

  // |?|?|?|
  // | |=|X|
  // |X|?|?|
  if (
    surroundingBlocks[SB_LEFT] == 0 &&
    surroundingBlocks[SB_RIGHT] == 1 &&
    surroundingBlocks[SB_BOTTOM_LEFT] == 1
  ) {
    blitRect->x = 0;
    blitRect->y = 16;
    return;
  }

  if (surroundingBlocks[SB_TOP_CENTER] != 1) {
    blitRect->x = gameState->defaultWorldTileOffsetX;
    blitRect->y = 192;
    gameState->defaultWorldTileOffsetX += 32;
    if (gameState->defaultWorldTileOffsetX == 128) {
      gameState->defaultWorldTileOffsetX = 0;
    }
    return;
  }
  blitRect->x = 24;
  blitRect->y = 40;
}

GameRenderer* makeGameRenderer(GameContext* ctx) {
  GameRenderer* renderer = (GameRenderer*)malloc(sizeof(GameRenderer));
  GameState* gameState = (GameState*)ctx->currentState->stateExt;

  renderer->winnerFont = TTF_OpenFont("./assets/fonts/Ubuntu-R.ttf", 150);
  renderer->worldSurface = createSurface(gameState->world->width * 32, gameState->world->height * 32);
  SDL_Surface* tileSetSurface = IMG_Load("./assets/gfx/char_tileset.png");
  renderer->charTilesTexture = SDL_CreateTextureFromSurface(ctx->renderer, tileSetSurface);
  SDL_FreeSurface(tileSetSurface);
  SDL_Surface* bgSurface = IMG_Load("./assets/gfx/plx-4.png");
  renderer->backgroundTexture = SDL_CreateTextureFromSurface(ctx->renderer, bgSurface);
  SDL_FreeSurface(bgSurface);
  renderer->worldTilesSurface = IMG_Load("./assets/gfx/world_tileset.png");
  renderer->worldTilesTexture = SDL_CreateTextureFromSurface(ctx->renderer, renderer->worldTilesSurface);

  World* world = gameState->world;
  SDL_Rect blitRect;
  blitRect.w = 32;
  blitRect.h = 32;
  SDL_Rect destRect;
  destRect.w = 32;
  destRect.h = 32;
  uint8_t* surroundingBlocks = (uint8_t*)malloc(8 * sizeof(uint8_t));
  int maxY = world->height - 1;
  int maxX = world->width - 1;
  for (int y = 0; y < world->height; y++) {
    for (int x = 0; x < world->width; x++) {
      uint8_t currentBlock = world->blocks[y * world->width + x];
      if (currentBlock != 1) continue;
      if (currentBlock == 1) {
        surroundingBlocks[SB_TOP_LEFT] = y == 0 || x == 0 ? 0 : world->blocks[(y - 1) * world->width + x - 1];           // TOP-LEFT
        surroundingBlocks[SB_TOP_CENTER] = y == 0 ? 0 : world->blocks[(y - 1) * world->width + x];                       // TOP-CENTER
        surroundingBlocks[SB_TOP_RIGHT] = y == 0 || x == maxX ? 0 : world->blocks[(y - 1) * world->width + x + 1];       // TOP-RIGHT
        surroundingBlocks[SB_RIGHT] = x == maxX ? 0 : world->blocks[y * world->width + x + 1];                           // RIGHT
        surroundingBlocks[SB_BOTTOM_RIGHT] = x == maxX || y == maxY ? 0 : world->blocks[(y + 1) * world->width + x + 1]; // BOTTOM-RIGHT
        surroundingBlocks[SB_BOTTOM_CENTER] = y == maxY ? 0 : world->blocks[(y + 1) * world->width + x];                 // BOTTOM
        surroundingBlocks[SB_BOTTOM_LEFT] = x == 0 || y == maxY ? 0 : world->blocks[(y + 1) * world->width + x - 1];     // BOTTOM-LEFT
        surroundingBlocks[SB_LEFT] = x == 0 ? 0 : world->blocks[y * world->width + x - 1];                               // LEFT
        for (int i = 0; i < 8; i++) {
          surroundingBlocks[i] = surroundingBlocks[i] == 3 || surroundingBlocks[i] == 2 ? 0 : surroundingBlocks[i];
        }
        determineBlitRectBySurroundingBlocks(gameState, &blitRect, surroundingBlocks);
      }
      destRect.x = x * 32;
      destRect.y = y * 32;
      SDL_BlitSurface(renderer->worldTilesSurface, &blitRect, renderer->worldSurface, &destRect);
    }
  }
  free(surroundingBlocks);
  renderer->worldTexture = SDL_CreateTextureFromSurface(ctx->renderer, renderer->worldSurface);
  renderer->gameTexture = SDL_CreateTexture(
    ctx->renderer,
    SDL_PIXELFORMAT_RGBA32,
    SDL_TEXTUREACCESS_TARGET,
    renderer->worldSurface->w,
    renderer->worldSurface->h
  );
  return renderer;
}

void determineArrowBlitRect(SDL_Rect* blitRect, GameState* gameState) {
  if (gameState->arrowTileOffsetX == 5) {
    gameState->arrowTileOffsetX = 0;
  }
  blitRect->x = gameState->arrowTileOffsetX * 32;
  blitRect->y = 224;
  Uint32 ticks = SDL_GetTicks();
  if (ticks - gameState->arrowTicks >= 100) {
    gameState->arrowTileOffsetX++;
    gameState->arrowTicks = ticks;
  }
}

void renderWinAnimation(GameRenderer* renderer, GameContext *ctx) {
  State* state = (State*)ctx->currentState;
  GameState* gameState = (GameState*)state->stateExt;
  Uint32 ticks = SDL_GetTicks();
  if (ticks - gameState->winAnimationTicks >= 5 && gameState->winRectHeight < 1080) {
    gameState->winRectHeight += 5;
    gameState->winAnimationTicks = ticks;
  }
  SDL_Rect rect;
  rect.x = 0;
  rect.y = 0;
  rect.w = 1920;
  rect.h = gameState->winRectHeight;
  SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 255);
  SDL_RenderFillRect(ctx->renderer, &rect);

  SDL_Color white = {255, 255, 255};
  SDL_Texture* textTexture = createTextTexture(ctx->renderer, "WINNER!", white, renderer->winnerFont, &rect.w, &rect.h);
  rect.x = 1920 / 2 - rect.w / 2;
  rect.y = gameState->winRectHeight / 2 - rect.h / 2;
  SDL_RenderCopy(ctx->renderer, textTexture, NULL, &rect);
  SDL_DestroyTexture(textTexture);
}

void renderGame(GameRenderer* renderer, GameContext* ctx) {
  GameState* gameState = (GameState*)ctx->currentState->stateExt;
  World* world = gameState->world;

  SDL_RenderCopy(ctx->renderer, renderer->backgroundTexture, NULL, NULL);

  // World -->
  SDL_SetTextureBlendMode(renderer->gameTexture, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(ctx->renderer, renderer->gameTexture);
  SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 0, 0);
  SDL_RenderClear(ctx->renderer);
  SDL_SetRenderDrawBlendMode(ctx->renderer, SDL_BLENDMODE_BLEND);
  SDL_RenderCopy(ctx->renderer, renderer->worldTexture, NULL, NULL);
  // <-- World

  // Char -->
  SDL_Rect tileSetSrc;
  tileSetSrc.x = gameState->charTileOffsetX * getCharTileWidthByAction(gameState->charAction);
  tileSetSrc.y = getCharTileYOffsetByAction(gameState->charAction);
  tileSetSrc.w = getCharTileWidthByAction(gameState->charAction);
  tileSetSrc.h = 34;
  SDL_Rect tileSetDest;
  tileSetDest.x = gameState->charPosX;
  tileSetDest.y = gameState->charPosY;
  tileSetDest.w = getCharTileWidthByAction(gameState->charAction);
  tileSetDest.h = 34;

  if (gameState->moveDir == 1)
    SDL_RenderCopy(ctx->renderer, renderer->charTilesTexture, &tileSetSrc, &tileSetDest);
  else
    SDL_RenderCopyEx(ctx->renderer, renderer->charTilesTexture, &tileSetSrc, &tileSetDest, 0, NULL, SDL_FLIP_HORIZONTAL);

  // <-- Char
  // GOAL -->
  tileSetSrc.w = 32;
  tileSetSrc.h = 32;
  determineArrowBlitRect(&tileSetSrc, gameState);
  tileSetDest.x = gameState->goalPosX;
  tileSetDest.y = gameState->goalPosY;
  tileSetDest.w = 32;
  tileSetDest.h = 32;
  SDL_RenderCopy(ctx->renderer, renderer->worldTilesTexture, &tileSetSrc, &tileSetDest);
  // <--GOAL

  if (gameState->debugMode) {
    SDL_Rect feetRect;
    SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 0, 255);
    determineBoundingRectangleByCharPos(gameState->charPosX, gameState->charPosY, gameState->charAction, &feetRect);
    SDL_RenderDrawRect(ctx->renderer, &feetRect);
    feetRect.x = gameState->goalPosX + 6;
    feetRect.y = gameState->goalPosY + 7;
    feetRect.w = 19;
    feetRect.h = 25;
    SDL_RenderDrawRect(ctx->renderer, &feetRect);
    determineFeetRectangleByCharPos(gameState, &feetRect);
    SDL_SetRenderDrawColor(ctx->renderer, 255, 0, 255, 255);
    SDL_RenderFillRect(ctx->renderer, &feetRect);
  }

  // switch back to default render target (window)
  SDL_SetRenderTarget(ctx->renderer, NULL);

  int scalingFactor = 128;
  while (world->width * scalingFactor > 1920 || world->height * scalingFactor > 1080) {
    scalingFactor -= 32;
  }

  SDL_Rect renderRect;
  renderRect.x = (1920 / 2) - ((world->width * scalingFactor) / 2);
  renderRect.y = (1080 / 2) - ((world->height * scalingFactor) / 2);
  renderRect.w = world->width * scalingFactor;
  renderRect.h = world->height * scalingFactor;
  SDL_RenderCopy(ctx->renderer, renderer->gameTexture, NULL, &renderRect);
  if (gameState->winFlag) {
    renderWinAnimation(renderer, ctx);
  }
}

void freeGameRenderer(GameRenderer* renderer) {
  SDL_DestroyTexture(renderer->backgroundTexture);
  SDL_DestroyTexture(renderer->charTilesTexture);
  SDL_DestroyTexture(renderer->gameTexture);
  SDL_DestroyTexture(renderer->worldTilesTexture);
  SDL_DestroyTexture(renderer->worldTexture);
  SDL_FreeSurface(renderer->worldTilesSurface);
  SDL_FreeSurface(renderer->worldSurface);
  TTF_CloseFont(renderer->winnerFont);
}
