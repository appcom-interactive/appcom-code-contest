#include "sdl-util.h"

SDL_Surface* createTextSurface(const char* text, TTF_Font* font, SDL_Color color) {
  return TTF_RenderUTF8_Blended(font, text, color);
}

SDL_Texture * createTextTexture(SDL_Renderer* renderer, const char* text, SDL_Color color, TTF_Font* font, int* w, int* h) {
  SDL_Surface * s = createTextSurface(text, font, color);
  if (!s) {
    return NULL;
  }
  SDL_Texture * t = SDL_CreateTextureFromSurface(renderer, s);
  if (!t) {
    SDL_FreeSurface(s);
    return NULL;
  }
  (*w) = s->w;
  (*h) = s->h;
  SDL_FreeSurface(s);
  return t;
}

SDL_Surface* createSurface(int w, int h) {
  Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif
  return SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
}

SDL_Texture* renderTextToSurface(SDL_Renderer* renderer, SDL_Surface* surface, const char* text, SDL_Color color, TTF_Font* font) {
  SDL_Surface* targetSurface = createSurface(surface->w, surface->h);
  SDL_BlitSurface(surface, NULL, targetSurface, NULL); // copy surface to temp surface to avoid changing th original surface

  SDL_Surface* textSurface = createTextSurface(text, font, color);
  SDL_Rect destRect;
  destRect.w = textSurface->w;
  destRect.h = textSurface->h;
  destRect.x = targetSurface->w / 2 - textSurface->w / 2;
  destRect.y = targetSurface->h / 2 - textSurface->h / 2;
  SDL_BlitSurface(textSurface, NULL, targetSurface, &destRect);
  SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, targetSurface);
  SDL_FreeSurface(textSurface);
  SDL_FreeSurface(targetSurface);
  return t;
}
