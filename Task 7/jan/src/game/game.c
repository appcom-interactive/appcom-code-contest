#include "game.h"
#include "stdint.h"
#include "../menu/world-menu.h"
#include "../util/sdl-util.h"

State* createGameState(WorldEnum id) {
  State* state = createState();
  GameState* gameState = (GameState*)malloc(sizeof(GameState));
  gameState->lastRender = SDL_GetTicks();
  gameState->spriteTicks = SDL_GetTicks();
  gameState->movementTicks = SDL_GetTicks();
  gameState->jumpTicks = SDL_GetTicks();
  gameState->world = loadWorld(id, WORLD_LOADING_STATUS_FULL);
  gameState->log = 1;
  gameState->gameTexture = NULL;
  gameState->worldTexture = NULL;
  gameState->backgroundTexture = NULL;
  gameState->charTilesTexture = NULL;
  gameState->worldTilesSurface = NULL;
  gameState->worldSurface = NULL;
  state->onBeforeRender = &onBeforeGameRender;
  state->onRender = &onGameRender;
  state->onEvent = &onGameEvent;
  state->onEnter = &onGameEnter;
  state->onExit = &onGameExit;
  state->stateExt = (void*)gameState;
  state->mustRender = 1;
  return state;
}

void onGameEnter(GameContext* ctx) {
  GameState* gameState = (GameState*)ctx->currentState->stateExt;
  gameState->worldSurface = createSurface(gameState->world->width * 32, gameState->world->height * 32);
  SDL_Surface* tileSetSurface = IMG_Load("./assets/gfx/char_tileset.png");
  gameState->charTilesTexture = SDL_CreateTextureFromSurface(ctx->renderer, tileSetSurface);
  SDL_FreeSurface(tileSetSurface);
  SDL_Surface* bgSurface = IMG_Load("./assets/gfx/plx-4.png");
  gameState->backgroundTexture = SDL_CreateTextureFromSurface(ctx->renderer, bgSurface);
  SDL_FreeSurface(bgSurface);
  gameState->worldTilesSurface = IMG_Load("./assets/gfx/world_tileset.png");
  gameState->charTileOffsetX = 0;
  gameState->charTileWidth = 19;
  gameState->jumpOriginY = 0;
  gameState->charAction = CHAR_ACTION_IDLEING;
  gameState->moveDir = -1;
  gameState->leap = 0;
  gameState->leftKeyDown = 0;
  gameState->rightKeyDown = 0;
  for (int y = 0; y < gameState->world->height; y++) {
    for (int x = 0; x < gameState->world->width; x++) {
      uint8_t currentBlock = gameState->world->blocks[y*gameState->world->width + x];
      if (currentBlock == 2) {
        gameState->charPosX = x * 32;
        gameState->charPosY = y * 32;
        printf("Char@%d|%d\n", x, y);
        return;
      }
    }
  }
}

int getMaxCharTileCountByAction(CharActionEnum action) {
  switch (action) {
    case CHAR_ACTION_IDLEING: return 12;
    case CHAR_ACTION_RUNNING: return 8;
    default: return 1;
  }
}

int getCharTileYOffsetByAction(CharActionEnum action) {
  switch (action) {
    case CHAR_ACTION_IDLEING: return 0;
    case CHAR_ACTION_RUNNING: return 34;
    case CHAR_ACTION_JUMPING: return 68;
    case CHAR_ACTION_FALLING: return 102;
    default: return 0;
  }
}

int getCharTileWidthByAction(CharActionEnum action) {
  switch (action) {
    case CHAR_ACTION_IDLEING: return 19;
    case CHAR_ACTION_RUNNING: return 21;
    case CHAR_ACTION_JUMPING: return 19;
    case CHAR_ACTION_FALLING: return 19;
    default: return 0;
  }
} 

void onGameEvent(GameContext* ctx, SDL_Event* event) {
  if (!(event->type == SDL_KEYDOWN || event->type == SDL_KEYUP)) return;
  State* state = (State*)ctx->currentState;
  GameState* gameState = (GameState*)state->stateExt;
  SDL_Keycode key = event->key.keysym.sym;
  if (event->type == SDL_KEYUP && (key == SDLK_RIGHT || key == SDLK_LEFT)) {
    if (key == SDLK_LEFT) {
      gameState->leftKeyDown = 0;
    }
    if (key == SDLK_RIGHT) {
      gameState->rightKeyDown = 0;
    }
    if (gameState->charAction == CHAR_ACTION_RUNNING && !(gameState->leftKeyDown || gameState->rightKeyDown)) {
      gameState->charAction = CHAR_ACTION_IDLEING;
      gameState->charTileOffsetX = 0;
      gameState->leap = 0;
      gameState->leftKeyDown = 0;
      gameState->rightKeyDown = 0;
    }
    if (gameState->charAction == CHAR_ACTION_JUMPING || gameState->charAction == CHAR_ACTION_FALLING) {
      gameState->leap = 0;
    }
  }
  if (event->type == SDL_KEYDOWN && key == SDLK_UP && gameState->charAction != CHAR_ACTION_FALLING) {
    if (gameState->charAction != CHAR_ACTION_JUMPING) {
      gameState->charAction = CHAR_ACTION_JUMPING;
      gameState->jumpOriginY = gameState->charPosY;
      gameState->charTileOffsetX = 0;
    }
  }
  if (event->type == SDL_KEYDOWN && (key == SDLK_RIGHT || key == SDLK_LEFT)) {
    if (key == SDLK_LEFT) {
      gameState->leftKeyDown = 1;
    }
    if (key == SDLK_RIGHT) {
      gameState->rightKeyDown = 1;
    }
    if (gameState->charAction == CHAR_ACTION_IDLEING) {
      gameState->charAction = CHAR_ACTION_RUNNING;
      gameState->charTileOffsetX = 0;
      gameState->moveDir = key == SDLK_RIGHT ? 1 : -1;
      gameState->leap = 1;
    }
    if (gameState->charAction == CHAR_ACTION_JUMPING || gameState->charAction == CHAR_ACTION_FALLING) {
      gameState->moveDir = key == SDLK_RIGHT ? 1 : -1;
      gameState->leap = 1;
    }
    if (gameState->charAction == CHAR_ACTION_RUNNING) {
      gameState->moveDir = key == SDLK_RIGHT ? 1 : -1;
    }
  }
  if (event->type == SDL_KEYUP && key == SDLK_ESCAPE) {
    pushState(ctx, createWorldMenuState());
  }
  if (event->type == SDL_KEYUP && key == SDLK_d) {
    getNormalizedCharPos(gameState);
  }
}

void determineFeetRectangleByCharPos(GameState* gameState, SDL_Rect* feetRect) {
  feetRect->x = gameState->charPosX + 5;
  feetRect->y = gameState->charPosY + 34;
  feetRect->w = 9;
  feetRect->h = 1;
}

void getNormalizedCharPos(GameState* gameState) {
  printf("NormXY: %d|%d\n", gameState->charPosX / 32, gameState->charPosY / 32);
  SDL_Rect feetRect;
  determineFeetRectangleByCharPos(gameState, &feetRect);
  printf("FeetXY: %d|%d\n", feetRect.x / 32, feetRect.y / 32);
}

void onBeforeGameRender(GameContext* ctx) {
  State* state = (State*)ctx->currentState;
  GameState* gameState = (GameState*)state->stateExt;
  SDL_Rect feetRect;

  // Sprite changement
  if (ctx->ticks - gameState->spriteTicks >= 90) {
    gameState->charTileOffsetX++;
    if (gameState->charTileOffsetX == getMaxCharTileCountByAction(gameState->charAction)) {
      gameState->charTileOffsetX = 0;
    }
    state->mustRender = 1;
    gameState->spriteTicks = SDL_GetTicks();
  }

  // Running
  if ((gameState->charAction == CHAR_ACTION_RUNNING) && (ctx->ticks - gameState->movementTicks >= 10)) {
    determineFeetRectangleByCharPos(gameState, &feetRect);
    int newX = gameState->charPosX + (2 * gameState->moveDir);
    int colX = gameState->moveDir == -1 ? newX - 1 : newX + 19;
    if (gameState->world->blocks[((gameState->charPosY / 32)) * gameState->world->width + (colX / 32)] != 1) {
      gameState->charPosX = newX;
    }
    uint8_t blockAtLeftFoot = gameState->world->blocks[((feetRect.y / 32)) * gameState->world->width + (feetRect.x / 32)];
    uint8_t blockAtRightFoot = gameState->world->blocks[((feetRect.y / 32)) * gameState->world->width + ((feetRect.x + feetRect.w) / 32)];
    if (blockAtLeftFoot != 1 && blockAtRightFoot != 1) {
      gameState->charAction = CHAR_ACTION_FALLING;
      gameState->charTileOffsetX = 0;
    }
    state->mustRender = 1;
    gameState->movementTicks = SDL_GetTicks();
  }

  // Jumping
  if ((gameState->charAction == CHAR_ACTION_JUMPING) && (ctx->ticks - gameState->jumpTicks >= 10)) {
    gameState->charPosY -= 2;
    if (gameState->leap) {
      int newX = gameState->charPosX + (2 * gameState->moveDir);
      int colX = gameState->moveDir == -1 ? newX - 1 : newX + 19;
      printf("BOING: %d|%d\n", gameState->charPosY / 32, colX / 32);
      if (gameState->world->blocks[((gameState->charPosY / 32)) * gameState->world->width + (colX / 32)] != 1) {
        gameState->charPosX = newX;
      }
    }
    if (abs(gameState->charPosY - gameState->jumpOriginY) >= 34) {
      gameState->charAction = CHAR_ACTION_FALLING;
    }
    state->mustRender = 1;
    gameState->jumpTicks = SDL_GetTicks();
  }

  // Falling
  if ((gameState->charAction == CHAR_ACTION_FALLING) && (ctx->ticks - gameState->jumpTicks >= 10)) {
    determineFeetRectangleByCharPos(gameState, &feetRect);
    if (gameState->world->blocks[((feetRect.y / 32)) * gameState->world->width + (feetRect.x / 32)] != 1) {
      gameState->charPosY += 2;
      if (gameState->leap) {
        int newX = gameState->charPosX + (2 * gameState->moveDir);
        int colX = gameState->moveDir == -1 ? newX - 1 : newX + 19;
        if (gameState->world->blocks[((gameState->charPosY / 32)) * gameState->world->width + (colX / 32)] != 1) {
          gameState->charPosX = newX;
        }
      }
    } else {
      gameState->charAction = (gameState->leftKeyDown == 0 && gameState->rightKeyDown == 0) ? CHAR_ACTION_IDLEING : CHAR_ACTION_RUNNING;
      printf("BOING: %d|%d|%d\n", gameState->leftKeyDown, gameState->rightKeyDown, gameState->charAction);
      gameState->leap = 0;
      gameState->charPosY = ((feetRect.y / 32) - 1) * 32;
    }
    state->mustRender = 1;
    gameState->jumpTicks = SDL_GetTicks();
  }
}

void onGameRender(GameContext* ctx) {
  State* state = (State*)ctx->currentState;
  GameState* gameState = (GameState*)state->stateExt;
  SDL_RenderCopy(ctx->renderer, gameState->backgroundTexture, NULL, NULL);
  renderWorld(ctx->renderer, gameState);
}

void onGameExit(GameContext* ctx) {
  State* state = (State*)ctx->currentState;
  GameState* gameState = (GameState*)state->stateExt;
  SDL_DestroyTexture(gameState->backgroundTexture);
  SDL_DestroyTexture(gameState->charTilesTexture);
  SDL_DestroyTexture(gameState->gameTexture);
  SDL_DestroyTexture(gameState->worldTexture);
  SDL_FreeSurface(gameState->worldTilesSurface);
  SDL_FreeSurface(gameState->worldSurface);
  freeWorld(gameState->world);
  free(gameState);
}

void determineBlitRectBySurroundingBlocks(SDL_Rect* blitRect, int8_t top, int8_t right, int8_t bottom, int8_t left) {
  if (top != 1) {
    blitRect->x = 0;
    blitRect->y = 192;
    return;
  }
  blitRect->x = 24;
  blitRect->y = 40;
}

void renderWorld(SDL_Renderer* renderer, GameState* gameState) {
  World* world = gameState->world;
  if (world->status != WORLD_LOADING_STATUS_FULL) {
    return;
  }
  //gameState->worldSurface = createSurface(gameState->world->width * 32, gameState->world->height * 32);
  if (gameState->log) {
    SDL_Rect blitRect;
    blitRect.w = 32;
    blitRect.h = 32;
    SDL_Rect destRect;
    destRect.w = 32;
    destRect.h = 32;
    int8_t top, left, right, bottom;
    for (int y = 0; y < world->height; y++) {
      for (int x = 0; x < world->width; x++) {
        uint8_t currentBlock = world->blocks[y*world->width + x];
        if (currentBlock != 1) continue;
        if (currentBlock == 1) {
          top = y == 0 ? -1 : world->blocks[(y-1)*world->width + x];
          left = x == 0 ? -1 : world->blocks[y * world->width + x - 1];
          right = x == world->width - 1 ? -1 : world->blocks[y * world->width + x + 1];
          bottom = y == world->height - 1 ? -1 : world->blocks[(y+1) * world->width + x];
          if (gameState->log) printf("%d|%d => %d,%d,%d,%d\n", x, y, top, right, bottom, left);
          determineBlitRectBySurroundingBlocks(&blitRect, top, right, bottom, left);
        }
        destRect.x = x * 32;
        destRect.y = y * 32;
        SDL_BlitSurface(gameState->worldTilesSurface, &blitRect, gameState->worldSurface, &destRect);
      }
    }

    gameState->worldTexture = SDL_CreateTextureFromSurface(renderer, gameState->worldSurface);
  }
  if (gameState->gameTexture != NULL) {
    SDL_DestroyTexture(gameState->gameTexture);
  }
  gameState->gameTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, gameState->worldSurface->w, gameState->worldSurface->h);
  SDL_SetTextureBlendMode(gameState->gameTexture, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(renderer, gameState->gameTexture);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_RenderCopy(renderer, gameState->worldTexture, NULL, NULL);
  // char-->
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
  if (gameState->log) printf("%d|%d =>\n", tileSetDest.x, tileSetDest.y);
  if (gameState->log) getNormalizedCharPos(gameState);
  
  if (gameState->moveDir == 1)
    SDL_RenderCopy(renderer, gameState->charTilesTexture, &tileSetSrc, &tileSetDest);
  else
    SDL_RenderCopyEx(renderer, gameState->charTilesTexture, &tileSetSrc, &tileSetDest, 0, NULL, SDL_FLIP_HORIZONTAL);

  SDL_Rect feetRect;
  determineFeetRectangleByCharPos(gameState, &feetRect);
  SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
  SDL_RenderFillRect(renderer, &feetRect);

  SDL_SetRenderTarget(renderer, NULL);
  // <-- char
  SDL_Rect renderRect;
  renderRect.x = (1920 / 2) - ((world->width * 128) / 2);
  renderRect.y = (1080 / 2) - ((world->height * 128) / 2);
  renderRect.w = world->width * 128;
  renderRect.h = world->height * 128;
  SDL_RenderCopy(renderer, gameState->gameTexture, NULL, &renderRect);
  gameState->log = 0;
}
