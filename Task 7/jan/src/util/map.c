#include "map.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

int enumCompareFn(void* data, void* cmp) {
  MapEntry* entry = (MapEntry*)data;
  int* key = (int*)entry->key;
  int* cmpI = (int*)cmp;
  return key == cmpI;
}

int stringCompareFn(void* data, void* cmp) {
  MapEntry* entry = (MapEntry*)data;
  return strcmp((const char*)entry->key, (const char*)cmp) == 0;
}

Map* createMap(MapType type, entryFreeFn freeFn, mapCompareFn compareFn) {
  Map* map = (Map*)malloc(sizeof(Map));
  map->type = type;
  switch (type) {
    case MAP_TYPE_ENUM:
      map->cmpFn = &enumCompareFn;
      break;
    case MAP_TYPE_STRING:
      map->cmpFn = &stringCompareFn;
      break;
    case MAP_TYPE_CUSTOM:
      if (compareFn == NULL) {
        free(map);
        return NULL;
      }
      map->cmpFn = compareFn;
      break;
    default:
      free(map);
      return NULL;
  }
  map->entries = createLinkedList(freeFn);
  return map;
}

int mapHas(Map* map, void* key) {
  return findListEntry(map->entries, map->cmpFn, key) != NULL;
}

void* mapGet(Map* map, void* key) {
  ListEntry* listEntry = findListEntry(map->entries, map->cmpFn, key);
  if (listEntry == NULL) {
    return listEntry;
  }
  MapEntry* mapEntry = (MapEntry*)listEntry->data;
  return mapEntry->value;
}

void mapPut(Map* map, void* key, void* value) {
  MapEntry* mapEntry = (MapEntry*)malloc(sizeof(MapEntry));
  mapEntry->key = key;
  mapEntry->value = value;
  ListEntry* listEntry = findListEntry(map->entries, map->cmpFn, key);
  if (listEntry != NULL) {
    listEntry->data = (void*)mapEntry;
    return;
  }
  pushListEntry(map->entries, (void*)mapEntry);
}

void mapDelete(Map* map, void* key) {
  removeListEntry(map->entries, map->cmpFn, key);
}

void freeMap(Map* map) {
  freeList(map->entries);
  free(map);
  map = NULL;
}
