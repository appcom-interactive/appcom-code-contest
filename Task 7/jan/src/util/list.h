#ifndef J_LIST_H
#define J_LIST_H

typedef int (*findFunc)(void*, void*);
typedef void (*entryFreeFn)(void*);
typedef void (*listWalkFn)(void*);


typedef struct _listentry {
  struct _listentry* previous; 
  struct  _listentry* next;
  void * data;
} ListEntry;

typedef struct _list {
  ListEntry* head;
  ListEntry* tail;
  unsigned int length;
  entryFreeFn freeFn;
} List;

List * createLinkedList(entryFreeFn);
void pushListEntry(List *, void *);
void unshiftListEntry(List *, void *);
ListEntry* findListEntry(List*, findFunc, void*);
void removeListEntry(List*, findFunc, void*);
void walkList(List*, listWalkFn);
void freeList(List*);

#endif
