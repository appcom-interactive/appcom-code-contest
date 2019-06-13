#include "world.h"
#include "stdlib.h"
#include "../util/sdl-util.h"

void freeWorld(World* world) {
  printf("freeWorld()\n");
  if (world->status == WORLD_LOADING_STATUS_FULL) {
    free(world->blocks);
  }
  free(world->name);
  free(world);
}

const char* getWorldFileByEnum(WorldEnum worldId) {
  switch (worldId) {
    case WORLD_1: return "./assets/worlds/world_1.wld";
    case WORLD_2: return "./assets/worlds/world_2.wld";
    case WORLD_3: return "./assets/worlds/world_3.wld";
    case WORLD_4: return "./assets/worlds/world_4.wld";
  }
  return "";
}

World* loadWorld(WorldEnum id, WorldLoadingStatus status) {
  if (status > WORLD_LOADING_STATUS_FULL) {
    return NULL;
  }
  World * w = (World*)malloc(sizeof(World));
  w->id = id;
  w->path = getWorldFileByEnum(id);
  if (status == WORLD_LOADING_STATUS_NONE) {
    return w;
  }

  FILE * worldFile = fopen(w->path, "rb");
  if (!worldFile) {
    printf("File '%s' opening failed\n", w->path);
    return NULL;
  }
  uint8_t nameLength; 
  fread(&w->width, sizeof(uint8_t), 1, worldFile);
  fread(&w->height, sizeof(uint8_t), 1, worldFile);
  fread(&nameLength, sizeof(uint8_t), 1, worldFile);
  w->name = (char*)malloc(nameLength * sizeof(char) + 1);
  fread(w->name, sizeof(char), nameLength, worldFile);
  w->name[nameLength] = '\0';
  if (status == WORLD_LOADING_STATUS_PARTIAL) {
    fclose(worldFile);
    w->status = status;
    return w;
  }
  uint32_t blockCount = w->height * w->width;
  w->blocks = (uint8_t*)malloc(blockCount * sizeof(uint8_t));
  fread(w->blocks, sizeof(uint8_t), blockCount, worldFile);
  fclose(worldFile);
  w->status = status;
  return w;
}
