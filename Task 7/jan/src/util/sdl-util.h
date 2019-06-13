#ifndef J_SDL_UTIL_H
#define J_SDL_UTIL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Surface* createSurface(int, int);
SDL_Surface* createTextSurface(const char*, TTF_Font*, SDL_Color);
SDL_Texture* createTextTexture(SDL_Renderer*, const char*, SDL_Color, TTF_Font*, int*, int*);
SDL_Texture* renderTextToSurface(SDL_Renderer*, SDL_Surface* surface, const char* text, SDL_Color color, TTF_Font* font);

#endif
