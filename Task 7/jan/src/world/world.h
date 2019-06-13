#ifndef _WORLD_H
#define _WORLD_H

#include <SDL2/SDL.h>
#include "stdint.h"
#include "stdio.h"

typedef enum {
  WORLD_1,
  WORLD_2,
  WORLD_3,
  WORLD_4
} WorldEnum;

typedef enum {
  WORLD_LOADING_STATUS_NONE,
  WORLD_LOADING_STATUS_PARTIAL,
  WORLD_LOADING_STATUS_FULL
} WorldLoadingStatus;

typedef struct World {
  WorldEnum id;
  const char* path;
  WorldLoadingStatus status;
  uint8_t width;
  uint8_t height;
  char * name;
  uint8_t * blocks;
} World;

World* loadWorld(WorldEnum, WorldLoadingStatus);
const char* getWorldFileByEnum(WorldEnum);

#endif
