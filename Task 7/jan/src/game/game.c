#include "game.h"
#include "stdint.h"
#include "../menu/world-menu.h"
#include "../util/sdl-util.h"

int getMaxCharTileCountByAction(CharActionEnum action)
{
  switch (action)
  {
  case CHAR_ACTION_IDLEING:
    return 12;
  case CHAR_ACTION_RUNNING:
    return 8;
  default:
    return 1;
  }
}

int getCharTileYOffsetByAction(CharActionEnum action)
{
  switch (action)
  {
  case CHAR_ACTION_IDLEING:
    return 0;
  case CHAR_ACTION_RUNNING:
    return 34;
  case CHAR_ACTION_JUMPING:
    return 68;
  case CHAR_ACTION_FALLING:
    return 102;
  default:
    return 0;
  }
}

int getCharTileWidthByAction(CharActionEnum action)
{
  switch (action)
  {
  case CHAR_ACTION_IDLEING:
    return 19;
  case CHAR_ACTION_RUNNING:
    return 21;
  case CHAR_ACTION_JUMPING:
    return 19;
  case CHAR_ACTION_FALLING:
    return 19;
  default:
    return 0;
  }
}

State *createGameState(WorldEnum id)
{
  State *state = createState();
  GameState *gameState = (GameState *)malloc(sizeof(GameState));
  gameState->winnerFont = TTF_OpenFont("./assets/fonts/Ubuntu-R.ttf", 150);
  gameState->lastRender = SDL_GetTicks();
  gameState->spriteTicks = SDL_GetTicks();
  gameState->movementTicks = SDL_GetTicks();
  gameState->jumpTicks = SDL_GetTicks();
  gameState->arrowTicks = SDL_GetTicks();
  gameState->winAnimationTicks = SDL_GetTicks();
  gameState->arrowTileOffsetX = 0;
  gameState->world = loadWorld(id, WORLD_LOADING_STATUS_FULL);
  gameState->debugMode = 0;
  gameState->defaultWorldTileOffsetX = 0;
  gameState->gameTexture = NULL;
  gameState->worldTexture = NULL;
  gameState->backgroundTexture = NULL;
  gameState->charTilesTexture = NULL;
  gameState->worldTilesSurface = NULL;
  gameState->worldTilesTexture = NULL;
  gameState->worldSurface = NULL;
  gameState->winFlag = 0;
  gameState->winRectHeight = 0;
  state->onBeforeRender = &onBeforeGameRender;
  state->onRender = &onGameRender;
  state->onEvent = &onGameEvent;
  state->onEnter = &onGameEnter;
  state->onExit = &onGameExit;
  state->stateExt = (void *)gameState;
  state->mustRender = 1;
  return state;
}

void onGameEnter(GameContext *ctx)
{
  GameState *gameState = (GameState *)ctx->currentState->stateExt;
  gameState->worldSurface = createSurface(gameState->world->width * 32, gameState->world->height * 32);
  SDL_Surface *tileSetSurface = IMG_Load("./assets/gfx/char_tileset.png");
  gameState->charTilesTexture = SDL_CreateTextureFromSurface(ctx->renderer, tileSetSurface);
  SDL_FreeSurface(tileSetSurface);
  SDL_Surface *bgSurface = IMG_Load("./assets/gfx/plx-4.png");
  gameState->backgroundTexture = SDL_CreateTextureFromSurface(ctx->renderer, bgSurface);
  SDL_FreeSurface(bgSurface);
  gameState->worldTilesSurface = IMG_Load("./assets/gfx/world_tileset.png");
  gameState->worldTilesTexture = SDL_CreateTextureFromSurface(ctx->renderer, gameState->worldTilesSurface);
  gameState->charTileOffsetX = 0;
  gameState->charTileWidth = 19;
  gameState->jumpOriginY = 0;
  gameState->charAction = CHAR_ACTION_IDLEING;
  gameState->moveDir = -1;
  gameState->leap = 0;
  gameState->leftKeyDown = 0;
  gameState->rightKeyDown = 0;
  for (int y = 0; y < gameState->world->height; y++)
  {
    for (int x = 0; x < gameState->world->width; x++)
    {
      uint8_t currentBlock = gameState->world->blocks[y * gameState->world->width + x];
      if (currentBlock == 2)
      {
        gameState->charPosX = x * 32;
        gameState->charPosY = y * 32;
      }
      if (currentBlock == 3)
      {
        gameState->goalPosX = x * 32;
        gameState->goalPosY = y * 32;
      }
    }
  }

  World *world = gameState->world;
  SDL_Rect blitRect;
  blitRect.w = 32;
  blitRect.h = 32;
  SDL_Rect destRect;
  destRect.w = 32;
  destRect.h = 32;
  uint8_t *surroundingBlocks = (uint8_t *)malloc(8 * sizeof(uint8_t));
  int maxY = world->height - 1;
  int maxX = world->width - 1;
  for (int y = 0; y < world->height; y++)
  {
    for (int x = 0; x < world->width; x++)
    {
      uint8_t currentBlock = world->blocks[y * world->width + x];
      if (currentBlock != 1)
        continue;
      if (currentBlock == 1)
      {
        surroundingBlocks[SB_TOP_LEFT] = y == 0 || x == 0 ? 0 : world->blocks[(y - 1) * world->width + x - 1];           // TOP-LEFT
        surroundingBlocks[SB_TOP_CENTER] = y == 0 ? 0 : world->blocks[(y - 1) * world->width + x];                       // TOP-CENTER
        surroundingBlocks[SB_TOP_RIGHT] = y == 0 || x == maxX ? 0 : world->blocks[(y - 1) * world->width + x + 1];       // TOP-RIGHT
        surroundingBlocks[SB_RIGHT] = x == maxX ? 0 : world->blocks[y * world->width + x + 1];                           // RIGHT
        surroundingBlocks[SB_BOTTOM_RIGHT] = x == maxX || y == maxY ? 0 : world->blocks[(y + 1) * world->width + x + 1]; // BOTTOM-RIGHT
        surroundingBlocks[SB_BOTTOM_CENTER] = y == maxY ? 0 : world->blocks[(y + 1) * world->width + x];                 // BOTTOM
        surroundingBlocks[SB_BOTTOM_LEFT] = x == 0 || y == maxY ? 0 : world->blocks[(y + 1) * world->width + x - 1];     // BOTTOM-LEFT
        surroundingBlocks[SB_LEFT] = x == 0 ? 0 : world->blocks[y * world->width + x - 1];                               // LEFT
        for (int i = 0; i < 8; i++)
        {
          surroundingBlocks[i] = surroundingBlocks[i] == 3 || surroundingBlocks[i] == 2 ? 0 : surroundingBlocks[i];
        }
        determineBlitRectBySurroundingBlocks(gameState, &blitRect, surroundingBlocks);
      }
      destRect.x = x * 32;
      destRect.y = y * 32;
      SDL_BlitSurface(gameState->worldTilesSurface, &blitRect, gameState->worldSurface, &destRect);
    }
  }
  free(surroundingBlocks);
  gameState->worldTexture = SDL_CreateTextureFromSurface(ctx->renderer, gameState->worldSurface);
}

void onGameEvent(GameContext *ctx, SDL_Event *event)
{
  if (!(event->type == SDL_KEYDOWN || event->type == SDL_KEYUP))
    return;
  State *state = (State *)ctx->currentState;
  GameState *gameState = (GameState *)state->stateExt;
  SDL_Keycode key = event->key.keysym.sym;
  if (event->type == SDL_KEYUP && key == SDLK_ESCAPE)
  {
    pushState(ctx, createWorldMenuState());
  }
  if (gameState->winFlag)
  {
    if (event->type == SDL_KEYUP && key == SDLK_RETURN)
    {
      pushState(ctx, createWorldMenuState());
    }
    return; // No other events should be handled
  }
  int rightPressed = key == SDLK_d || key == SDLK_RIGHT;
  int leftPressed = key == SDLK_a || key == SDLK_LEFT;
  int upPressed = key == SDLK_w || key == SDLK_SPACE || key == SDLK_UP;
  if (event->type == SDL_KEYUP && (leftPressed || rightPressed))
  {
    if (leftPressed)
    {
      gameState->leftKeyDown = 0;
    }
    if (rightPressed)
    {
      gameState->rightKeyDown = 0;
    }
    if (gameState->charAction == CHAR_ACTION_RUNNING && !(gameState->leftKeyDown || gameState->rightKeyDown))
    {
      gameState->charAction = CHAR_ACTION_IDLEING;
      gameState->charTileOffsetX = 0;
      gameState->leap = 0;
      gameState->leftKeyDown = 0;
      gameState->rightKeyDown = 0;
    }
    if (gameState->charAction == CHAR_ACTION_JUMPING || gameState->charAction == CHAR_ACTION_FALLING)
    {
      gameState->leap = 0;
    }
  }
  if (event->type == SDL_KEYDOWN && upPressed && gameState->charAction != CHAR_ACTION_FALLING)
  {
    if (gameState->charAction != CHAR_ACTION_JUMPING)
    {
      gameState->charAction = CHAR_ACTION_JUMPING;
      gameState->jumpOriginY = gameState->charPosY;
      gameState->charTileOffsetX = 0;
    }
  }
  if (event->type == SDL_KEYDOWN && (leftPressed || rightPressed))
  {
    if (leftPressed)
    {
      gameState->leftKeyDown = 1;
    }
    if (rightPressed)
    {
      gameState->rightKeyDown = 1;
    }
    if (gameState->charAction == CHAR_ACTION_IDLEING)
    {
      gameState->charAction = CHAR_ACTION_RUNNING;
      gameState->charTileOffsetX = 0;
      gameState->moveDir = rightPressed ? 1 : -1;
      gameState->leap = 1;
    }
    if (gameState->charAction == CHAR_ACTION_JUMPING || gameState->charAction == CHAR_ACTION_FALLING)
    {
      gameState->moveDir = rightPressed ? 1 : -1;
      gameState->leap = 1;
    }
    if (gameState->charAction == CHAR_ACTION_RUNNING)
    {
      gameState->moveDir = rightPressed ? 1 : -1;
    }
  }
  if (event->type == SDL_KEYUP && key == SDLK_f)
  {
    gameState->debugMode = gameState->debugMode == 1 ? 0 : 1;
  }
}

void determineFeetRectangleByCharPos(GameState *gameState, SDL_Rect *feetRect)
{
  feetRect->x = gameState->charPosX + 5;
  feetRect->y = gameState->charPosY + 34;
  feetRect->w = 9;
  feetRect->h = 1;
}

void determineBoundingRectangleByCharPos(int x, int y, int action, SDL_Rect *boundingRect)
{
  boundingRect->x = x;
  boundingRect->y = y - 3;
  boundingRect->w = getCharTileWidthByAction(action);
  boundingRect->h = 34;
}

int isCollision(SDL_Rect *boundingRect, GameState *gameState)
{
  int yCoord = ((boundingRect->y + boundingRect->h) / 32) * gameState->world->width;
  int xCoord = gameState->moveDir == -1 ? (boundingRect->x / 32) : (boundingRect->x + boundingRect->w) / 32;
  return gameState->world->blocks[yCoord + xCoord] == 1 ? 1 : 0; // TODO: This is bullshit
}

void onBeforeGameRender(GameContext *ctx)
{
  State *state = (State *)ctx->currentState;
  GameState *gameState = (GameState *)state->stateExt;
  SDL_Rect boundingRect;
  SDL_Rect feetRect;

  // Sprite changement
  if (ctx->ticks - gameState->spriteTicks >= 90)
  {
    gameState->charTileOffsetX++;
    if (gameState->charTileOffsetX == getMaxCharTileCountByAction(gameState->charAction))
    {
      gameState->charTileOffsetX = 0;
    }
    state->mustRender = 1;
    gameState->spriteTicks = SDL_GetTicks();
  }

  // Running
  if ((gameState->charAction == CHAR_ACTION_RUNNING) && (ctx->ticks - gameState->movementTicks >= 5))
  {
    int newX = gameState->charPosX + (1 * gameState->moveDir);
    determineBoundingRectangleByCharPos(newX, gameState->charPosY, CHAR_ACTION_IDLEING, &boundingRect);
    if (!isCollision(&boundingRect, gameState))
    {
      gameState->charPosX = newX;
    }
    // Jumping off a cliff
    determineFeetRectangleByCharPos(gameState, &feetRect);
    int feetY = (feetRect.y / 32) * gameState->world->width;
    int feetXLeft = (feetRect.x / 32);
    int feetXRight = (feetRect.x + feetRect.w) / 32;
    uint8_t blockAtLeftFoot = gameState->world->blocks[feetY + feetXLeft];
    uint8_t blockAtRightFoot = gameState->world->blocks[feetY + feetXRight];
    if (blockAtLeftFoot != 1 && blockAtRightFoot != 1)
    {
      gameState->charAction = CHAR_ACTION_FALLING;
      gameState->charTileOffsetX = 0;
    }
    state->mustRender = 1;
    gameState->movementTicks = SDL_GetTicks();
  }

  // Jumping
  if ((gameState->charAction == CHAR_ACTION_JUMPING) && (ctx->ticks - gameState->jumpTicks >= 5))
  {
    int newY = gameState->charPosY - 1;
    int newX = gameState->charPosX;
    determineBoundingRectangleByCharPos(newX, newY, CHAR_ACTION_JUMPING, &boundingRect);
    if (isCollision(&boundingRect, gameState))
    {
      newY = gameState->charPosY;
    }
    else
    {
      gameState->charPosY = newY;
    }
    if (gameState->leap)
    {
      newX = gameState->charPosX + (1 * gameState->moveDir);
      determineBoundingRectangleByCharPos(newX, newY, CHAR_ACTION_JUMPING, &boundingRect);
      if (isCollision(&boundingRect, gameState))
      {
        newX = gameState->charPosX;
      }
      else
      {
        gameState->charPosX = newX;
      }
    }
    if (abs(gameState->charPosY - gameState->jumpOriginY) >= 36)
    {
      gameState->charAction = CHAR_ACTION_FALLING;
    }
    state->mustRender = 1;
    gameState->jumpTicks = SDL_GetTicks();
  }

  // Falling
  if ((gameState->charAction == CHAR_ACTION_FALLING) && (ctx->ticks - gameState->jumpTicks >= 5))
  {
    int newY = gameState->charPosY + 1;
    int newX = gameState->charPosX;
    determineBoundingRectangleByCharPos(newX, newY, CHAR_ACTION_FALLING, &boundingRect);
    if (isCollision(&boundingRect, gameState))
    {
      gameState->charAction = (gameState->leftKeyDown == 0 && gameState->rightKeyDown == 0) ? CHAR_ACTION_IDLEING : CHAR_ACTION_RUNNING;
      gameState->leap = !(gameState->leftKeyDown || gameState->rightKeyDown) ? 0 : 1;
      gameState->charPosY = boundingRect.y + 2;
    }
    else
    {
      gameState->charPosY = newY;
      if (gameState->leap)
      {
        newX = gameState->charPosX + (1 * gameState->moveDir);
        determineBoundingRectangleByCharPos(newX, newY, CHAR_ACTION_FALLING, &boundingRect);
        if (isCollision(&boundingRect, gameState))
        {
          newX = gameState->charPosX;
        }
        else
        {
          gameState->charPosX = newX;
        }
      }
    }
    state->mustRender = 1;
    gameState->jumpTicks = SDL_GetTicks();
  }
  determineBoundingRectangleByCharPos(gameState->charPosX, gameState->charPosY, CHAR_ACTION_IDLEING, &boundingRect);
  feetRect.x = gameState->goalPosX + 6;
  feetRect.y = gameState->goalPosY + 7;
  feetRect.w = 19;
  feetRect.h = 25;
  if (SDL_HasIntersection(&boundingRect, &feetRect))
  {
    gameState->winFlag = 1;
    gameState->charAction = CHAR_ACTION_IDLEING;
    gameState->charTileOffsetX = 0;
    gameState->leap = 0;
    gameState->leftKeyDown = 0;
    gameState->rightKeyDown = 0;
  }
}

void renderWinAnimation(GameContext *ctx)
{
  State *state = (State *)ctx->currentState;
  GameState *gameState = (GameState *)state->stateExt;
  Uint32 ticks = SDL_GetTicks();
  if (ticks - gameState->winAnimationTicks >= 5 && gameState->winRectHeight < 1080)
  {
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
  SDL_Texture *textTexture = createTextTexture(ctx->renderer, "WINNER!", white, gameState->winnerFont, &rect.w, &rect.h);
  rect.x = 1920 / 2 - rect.w / 2;
  rect.y = gameState->winRectHeight / 2 - rect.h / 2;
  SDL_RenderCopy(ctx->renderer, textTexture, NULL, &rect);
  SDL_DestroyTexture(textTexture);
}

void onGameRender(GameContext *ctx)
{
  State *state = (State *)ctx->currentState;
  GameState *gameState = (GameState *)state->stateExt;
  SDL_RenderCopy(ctx->renderer, gameState->backgroundTexture, NULL, NULL);
  renderWorld(ctx->renderer, gameState);
  if (gameState->winFlag)
  {
    renderWinAnimation(ctx);
  }
}

void onGameExit(GameContext *ctx)
{
  State *state = (State *)ctx->currentState;
  GameState *gameState = (GameState *)state->stateExt;
  SDL_DestroyTexture(gameState->backgroundTexture);
  SDL_DestroyTexture(gameState->charTilesTexture);
  SDL_DestroyTexture(gameState->gameTexture);
  SDL_DestroyTexture(gameState->worldTilesTexture);
  SDL_DestroyTexture(gameState->worldTexture);
  SDL_FreeSurface(gameState->worldTilesSurface);
  SDL_FreeSurface(gameState->worldSurface);
  TTF_CloseFont(gameState->winnerFont);
  freeWorld(gameState->world);
  free(gameState);
}

void determineBlitRectBySurroundingBlocks(GameState *gameState, SDL_Rect *blitRect, uint8_t *surroundingBlocks)
{
  // |?| |?|
  // | |=| |
  // |?|X|?|
  if (
      surroundingBlocks[SB_TOP_CENTER] == 0 &&
      surroundingBlocks[SB_LEFT] == 0 &&
      surroundingBlocks[SB_RIGHT] == 0 &&
      surroundingBlocks[SB_BOTTOM_CENTER] == 1)
  {
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
      surroundingBlocks[SB_LEFT] == 1)
  {
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
      surroundingBlocks[SB_LEFT] == 1)
  {
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
      surroundingBlocks[SB_LEFT] == 0)
  {
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
      surroundingBlocks[SB_LEFT] == 0)
  {
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
      surroundingBlocks[SB_LEFT] == 0)
  {
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
      surroundingBlocks[SB_RIGHT] == 0)
  {
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
      surroundingBlocks[SB_RIGHT] == 0)
  {
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
      surroundingBlocks[SB_LEFT] == 0)
  {
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
      surroundingBlocks[SB_TOP_RIGHT] == 0)
  {
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
      surroundingBlocks[SB_TOP_LEFT] == 0)
  {
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
      surroundingBlocks[SB_BOTTOM_RIGHT] == 1)
  {
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
      surroundingBlocks[SB_BOTTOM_LEFT] == 1)
  {
    blitRect->x = 0;
    blitRect->y = 16;
    return;
  }

  if (surroundingBlocks[SB_TOP_CENTER] != 1)
  {
    blitRect->x = gameState->defaultWorldTileOffsetX;
    blitRect->y = 192;
    gameState->defaultWorldTileOffsetX += 32;
    if (gameState->defaultWorldTileOffsetX == 128)
    {
      gameState->defaultWorldTileOffsetX = 0;
    }
    return;
  }
  blitRect->x = 24;
  blitRect->y = 40;
}

void determineArrowBlitRect(SDL_Rect *blitRect, GameState *gameState)
{
  if (gameState->arrowTileOffsetX == 5)
  {
    gameState->arrowTileOffsetX = 0;
  }
  blitRect->x = gameState->arrowTileOffsetX * 32;
  blitRect->y = 224;
  Uint32 ticks = SDL_GetTicks();
  if (ticks - gameState->arrowTicks >= 100)
  {
    gameState->arrowTileOffsetX++;
    gameState->arrowTicks = ticks;
  }
}

void renderWorld(SDL_Renderer *renderer, GameState *gameState)
{
  World *world = gameState->world;
  if (world->status != WORLD_LOADING_STATUS_FULL)
  {
    return;
  }
  if (gameState->gameTexture != NULL)
  {
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

  if (gameState->moveDir == 1)
    SDL_RenderCopy(renderer, gameState->charTilesTexture, &tileSetSrc, &tileSetDest);
  else
    SDL_RenderCopyEx(renderer, gameState->charTilesTexture, &tileSetSrc, &tileSetDest, 0, NULL, SDL_FLIP_HORIZONTAL);

  if (gameState->debugMode)
  {
    SDL_Rect feetRect;
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    determineBoundingRectangleByCharPos(gameState->charPosX, gameState->charPosY, gameState->charAction, &feetRect);
    SDL_RenderDrawRect(renderer, &feetRect);
    feetRect.x = gameState->goalPosX + 6;
    feetRect.y = gameState->goalPosY + 7;
    feetRect.w = 19;
    feetRect.h = 25;
    SDL_RenderDrawRect(renderer, &feetRect);
    determineFeetRectangleByCharPos(gameState, &feetRect);
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderFillRect(renderer, &feetRect);
  }
  // <-- char
  // GOAL -->
  tileSetSrc.w = 32;
  tileSetSrc.h = 32;
  determineArrowBlitRect(&tileSetSrc, gameState);
  tileSetDest.x = gameState->goalPosX;
  tileSetDest.y = gameState->goalPosY;
  tileSetDest.w = 32;
  tileSetDest.h = 32;
  SDL_RenderCopy(renderer, gameState->worldTilesTexture, &tileSetSrc, &tileSetDest);
  // <--GOAL

  // switch back to default render target (window)
  SDL_SetRenderTarget(renderer, NULL);

  int scalingFactor = 128;
  while (world->width * scalingFactor > 1920 || world->height * scalingFactor > 1080)
  {
    scalingFactor -= 32;
  }

  SDL_Rect renderRect;
  renderRect.x = (1920 / 2) - ((world->width * scalingFactor) / 2);
  renderRect.y = (1080 / 2) - ((world->height * scalingFactor) / 2);
  renderRect.w = world->width * scalingFactor;
  renderRect.h = world->height * scalingFactor;
  SDL_RenderCopy(renderer, gameState->gameTexture, NULL, &renderRect);
}