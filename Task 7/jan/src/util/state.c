#include "state.h"

State * createState() {
  State * s = malloc(sizeof(State));
  s->onBeforeRender = NULL;
  s->onEvent = NULL;
  s->onExit = NULL;
  s->onEnter = NULL;
  s->onRender = NULL;
  s->mustRender = 1;
  s->stateExt = NULL;
  return s;
}
