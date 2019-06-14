#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "../util/state.h"
#include "../util/game-context.h"
#include "../world/world.h"

typedef enum {
  CHAR_ACTION_IDLEING,
  CHAR_ACTION_RUNNING,
  CHAR_ACTION_JUMPING,
  CHAR_ACTION_FALLING
} CharActionEnum;

typedef struct GameState {
  SDL_Texture* backgroundTexture;
  SDL_Texture* charTilesTexture;
  SDL_Texture* gameTexture;
  SDL_Texture* worldTexture;
  SDL_Texture* worldTilesTexture;
  SDL_Surface* worldTilesSurface;
  SDL_Surface* worldSurface;
  World* world;
  int arrowTileOffsetX;
  int winRectHeight;
  int goalPosX;
  int goalPosY;
  int charTileOffsetX;
  int charPosX;
  int charPosY;
  int charTileWidth;
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
  TTF_Font* winnerFont;
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
void renderWorld(SDL_Renderer*, GameState*);
void getNormalizedCharPos(GameState*);
void determineBlitRectBySurroundingBlocks(SDL_Rect*, int8_t, int8_t, int8_t, int8_t);

#endif
