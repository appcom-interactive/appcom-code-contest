#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "game-renderer.h"
#include "../state/state.h"
#include "../state/game-context.h"
#include "../world/world.h"

typedef struct GameRenderer GameRenderer;

typedef enum {
  CHAR_ACTION_IDLEING,
  CHAR_ACTION_RUNNING,
  CHAR_ACTION_JUMPING,
  CHAR_ACTION_FALLING
} CharActionEnum;

typedef enum {
  SB_TOP_LEFT,
  SB_TOP_CENTER,
  SB_TOP_RIGHT,
  SB_RIGHT,
  SB_BOTTOM_RIGHT,
  SB_BOTTOM_CENTER,
  SB_BOTTOM_LEFT,
  SB_LEFT
} SurroundingBlock;

typedef struct GameState {
  World* world;
  int arrowTileOffsetX;
  int winRectHeight;
  int goalPosX;
  int goalPosY;
  int charTileOffsetX;
  int charPosX;
  int charPosY;
  int charTileWidth;
  int defaultWorldTileOffsetX;
  int jumpOriginY;
  CharActionEnum charAction;
  Uint32 lastRender;
  Uint32 winAnimationTicks;
  Uint32 jumpTicks;
  Uint32 movementTicks;
  Uint32 spriteTicks;
  Uint32 arrowTicks;
  int moveDir;
  int leap;
  int debugMode;
  int leftKeyDown;
  int rightKeyDown;
  int winFlag;
  GameRenderer* gameRenderer;
} GameState;

// State functions
State* createGameState(WorldEnum);
void onBeforeGameRender(GameContext*);
void onGameRender(GameContext*);
void onGameEvent(GameContext*, SDL_Event*);
void onGameEnter(GameContext*);
void onGameExit(GameContext*);

// helper functions
int getMaxCharTileCountByAction(CharActionEnum);
int getCharTileYOffsetByAction(CharActionEnum);
int getCharTileWidthByAction(CharActionEnum);
void getNormalizedCharPos(GameState*);
void determineBoundingRectangleByCharPos(int, int, int, SDL_Rect*);
void determineFeetRectangleByCharPos(GameState*, SDL_Rect*);

#endif
