#include "list.h"
#include "stdlib.h"
#include "stdio.h"

List* createLinkedList(entryFreeFn freeFn) {
  List * l = (List *)malloc(sizeof(List));
  l->head = NULL;
  l->tail = NULL;
  l->freeFn = freeFn;
  l->length = 0;
  return l;
}

void pushListEntry(List* l, void* data) {
  ListEntry * le = (ListEntry *)malloc(sizeof(ListEntry));
  le->next = NULL;
  le->data = data;
  if (l->length == 0) {
    le->previous = NULL;
    l->head = le;
    l->tail = le;
  } else {
    le->previous = l->tail;
    l->tail->next = le;
    l->tail = le;
  }
  l->length++;
}

void unshiftListEntry(List* l, void* data) {
  ListEntry * le = (ListEntry *)malloc(sizeof(ListEntry));
  le->previous = NULL;
  le->data = data;
  if (l->length == 0) {
    le->next = NULL;
    l->head = le;
    l->tail = le;
  } else {
    le->next = l->head;
    l->head->previous = le;
    l->head = le;
  }
  l->length++;
}

ListEntry* findListEntry(List* l, findFunc f, void* funArg) {
  ListEntry* current = l->head;
  while (current != NULL) {
    if (f(current->data, funArg)) {
      return current;
    }
    current = current->next;
  }
  return NULL;
}

void _remove(List* l, ListEntry* le) {
  if (le->next == NULL) {
    if (le->previous == NULL) {
      l->head = NULL;
      l->tail = NULL;
      l->length--;
      return;
    }
    le->previous->next = NULL;
    l->tail = le->previous;
    l->length--;
    return;
  }
  if (le->previous == NULL) {
    if (le->next == NULL) {
      l->head = NULL;
      l->tail = NULL;
      l->length--;
      return;
    }
    le->next->previous = NULL;
    l->head = le->next;
    l->length--;
    return;
  }
  le->previous->next = le->next;
  le->next->previous = le->previous;
  l->length--;
}

void removeListEntry(List* l, findFunc f, void* findFunArg) {
  ListEntry* current = l->head;
  while (current != NULL) {
    if (f(current->data, findFunArg)) {
      _remove(l, current);
    }
    current = current->next;
  }
}

void walkList(List* l, listWalkFn f) {
  ListEntry* current = l->head;
  while (current != NULL) {
    f(current->data);
    current = current->next;
  }
}

void freeList(List* l) {
  printf("freeList()\n");
  ListEntry* current = l->head;
  ListEntry* next = NULL;
  while (current != NULL) {
    next = current->next;
    l->freeFn(current->data);
    free(current);
    current = next;
  }
  free(l);
  l = NULL;
}
