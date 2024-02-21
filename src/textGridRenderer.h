#ifndef TEXTGRIDRENDERER_H_
#define TEXTGRIDRENDERER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "utils.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct textureCache {
  char *chars;
  SDL_Surface **s;
  SDL_Color *colors_fg;
  SDL_Color *colors_bg;
} textureCache_t;

textureCache_t *tgr_buildCache(char *chars, TTF_Font *f, SDL_Color fg,
                               SDL_Color bg);
textureCache_t *tgr_buildASCIIcache(SDL_Color fg, SDL_Color bg, TTF_Font *f);

SDL_Texture *tgr_createGridTextureCached(textureCache_t *cache, SDL_Renderer *r,
                                         TTF_Font *f, char *grid,
                                         SDL_Color *colormap_fg,
                                         SDL_Color *colormap_bg,
                                         const unsigned int width,
                                         const unsigned int height);

int tgr_addToCache(textureCache_t *cache, TTF_Font *f, char c, SDL_Color fg,
                   SDL_Color bg);
void tgr_removeFromCache(textureCache_t *cache, char c);

void tgr_destroyCache(textureCache_t *cache);

#endif // TEXTGRIDRENDERER_H_
