#ifndef J_MAP_H
#define J_MAP_H

#include "list.h"

typedef int (*mapCompareFn)(void*, void*);

typedef enum {
  MAP_TYPE_STRING,
  MAP_TYPE_ENUM,
  MAP_TYPE_CUSTOM
} MapType;

typedef struct Map {
  MapType type;
  List* entries;
  mapCompareFn cmpFn;
} Map;

typedef struct MapEntry {
  void* key;
  void* value;
} MapEntry;

Map* createMap(MapType, entryFreeFn, mapCompareFn);
int mapHas(Map*, void*);
void* mapGet(Map*, void*);
void mapPut(Map*, void*, void*);
void mapDelete(Map*, void*);
void freeMap(Map*);

#endif
