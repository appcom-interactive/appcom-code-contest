#include "game.h"
#include "stdint.h"
#include "../menu/world-menu.h"
#include "../util/sdl-util.h"

int getMaxCharTileCountByAction(CharActionEnum action) {
  switch (action) {
  case CHAR_ACTION_IDLEING:
    return 12;
  case CHAR_ACTION_RUNNING:
    return 8;
  default:
    return 1;
  }
}

int getCharTileYOffsetByAction(CharActionEnum action) {
  switch (action) {
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

int getCharTileWidthByAction(CharActionEnum action) {
  switch (action) {
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

State* createGameState(WorldEnum id) {
  State* state = createState();
  GameState* gameState = (GameState*)malloc(sizeof(GameState));
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

void onGameEnter(GameContext* ctx) {
  GameState* gameState = (GameState*)ctx->currentState->stateExt;
  gameState->gameRenderer = makeGameRenderer(ctx);
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
      uint8_t currentBlock = gameState->world->blocks[y * gameState->world->width + x];
      if (currentBlock == 2) {
        gameState->charPosX = x * 32;
        gameState->charPosY = y * 32;
      }
      if (currentBlock == 3) {
        gameState->goalPosX = x * 32;
        gameState->goalPosY = y * 32;
      }
    }
  }
}

void onGameEvent(GameContext* ctx, SDL_Event* event) {
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

void determineFeetRectangleByCharPos(GameState* gameState, SDL_Rect* feetRect) {
  feetRect->x = gameState->charPosX + 5;
  feetRect->y = gameState->charPosY + 34;
  feetRect->w = 9;
  feetRect->h = 1;
}

void determineBoundingRectangleByCharPos(int x, int y, int action, SDL_Rect* boundingRect) {
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

void onGameRender(GameContext* ctx) {
  State* state = (State*)ctx->currentState;
  GameState* gameState = (GameState*)state->stateExt;
  renderGame(gameState->gameRenderer, ctx);
}

void onGameExit(GameContext *ctx) {
  State *state = (State *)ctx->currentState;
  GameState *gameState = (GameState *)state->stateExt;
  freeGameRenderer(gameState->gameRenderer);
  free(gameState->gameRenderer);
  freeWorld(gameState->world);
  free(gameState);
}
