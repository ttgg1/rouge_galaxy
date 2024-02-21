#include "textGridRenderer.h"

textureCache_t *tgr_buildCache(char *chars, TTF_Font *f, SDL_Color fg,
                               SDL_Color bg) {
  textureCache_t *res = (textureCache_t *)malloc(sizeof(textureCache_t));

  const unsigned int len = (unsigned int)strlen(chars);

  // allocate cache
  res->chars = (char *)malloc(sizeof(char *) * len);
  res->s = (SDL_Surface **)malloc(sizeof(SDL_Surface *) * len);
  res->colors_fg = (SDL_Color *)malloc(sizeof(SDL_Color) * len);
  res->colors_bg = (SDL_Color *)malloc(sizeof(SDL_Color) * len);

  // fill arrays
  strcpy((char *)res->chars, (char *)chars);
  for (int i = 0; i < len; ++i) {
    res->s[i] = TTF_RenderText_Shaded(f, &chars[i], fg, bg);
    res->colors_fg[i] = fg;
    res->colors_bg[i] = bg;
  }
  return res;
}

textureCache_t *tgr_buildASCIIcache(SDL_Color fg, SDL_Color bg, TTF_Font *f) {
  char ascii_chars[223];
  char curr = (char)32;
  for (int i = 0; i < 223; ++i) {
    ascii_chars[i] = curr;
    ++curr;
  }
  return tgr_buildCache(ascii_chars, f, fg, bg);
}

// returns index of b in a, otherwise -1
int strContains(char *a, char b) {
  size_t len = strlen(a);
  for (int i = 0; i < len; ++i) {
    if (a[i] == b)
      return i;
  }
  return -1;
}

bool compColors(SDL_Color *a, SDL_Color *b) {
  return (a->r == b->r && a->g == b->g && a->b == b->b && a->a == b->a);
}

// TODO: Check Colormap
SDL_Texture *tgr_createGridTextureCached(textureCache_t *cache, SDL_Renderer *r,
                                         TTF_Font *f, char *grid,
                                         SDL_Color *colormap_fg,
                                         SDL_Color *colormap_bg,
                                         const unsigned int width,
                                         const unsigned int height) {
  // check if all chars are cached -> otherwise add to cache
  size_t len = strlen(grid);
  SDL_Surface *s = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
  SDL_SetSurfaceRLE(s, SDL_RLEACCEL);

  int i = 0;
  SDL_Rect pos = {0, 0, cache->s[i]->w, cache->s[i]->h};

  SDL_LockSurface(s);
  for (int w = 0; w < width; ++w) {
    for (int h = 0; h < height; ++h) {
      int tmp = strContains((char *)cache->chars, grid[i]);
      // if char is not cached, cache it
      // or char is cached but in the wrong color combination
      if ((tmp == -1) ||
          (!(compColors(&colormap_fg[i], &cache->colors_fg[i]) &&
             compColors(&colormap_bg[i], &cache->colors_bg[i])))) {
        tmp = tgr_addToCache(cache, f, grid[i], colormap_fg[i], colormap_bg[i]);
      }

      // Draw cached surface to temp surface
      if (tmp != NULL) {
        SDL_BlitSurface(cache->s[tmp], NULL, s, &pos);
      } else {
        debug_print("failed to get index of Surface to paste !\n");
      }
      // increase grid index and change the position of the char, which is
      // getting pasted next loop
      ++i;
      pos.x += pos.w;
      if (pos.x >= width) {
        pos.y += pos.h;
        pos.x = 0;
      }
    }
  }
  SDL_UnlockSurface(s);
  SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
  SDL_FreeSurface(s);
  return t;
}

// returns index of new cahed char
int tgr_addToCache(textureCache_t *cache, TTF_Font *f, char c, SDL_Color fg,
                   SDL_Color bg) {
  if (strContains((char *)cache->chars, c) == -1) {
    char *newStr = strcat((char *)cache->chars, &c);
    size_t len = strlen(newStr);
    cache->chars = (char *)realloc(cache->chars, len * sizeof(char));
    strcpy(cache->chars, newStr);

    // gen new texture
    cache->s = (SDL_Surface **)realloc(cache->s, len * sizeof(SDL_Surface *));
    cache->s[len - 1] = TTF_RenderText_Shaded(f, &c, fg, bg);

    return (int)(len - 1);
  } else {
    debug_print("char already in cache !\n");
  }
  return NULL;
}

void tgr_removeFromCache(textureCache_t *cache, char c) {
  if (strContains(cache->chars, c) != -1) {

    size_t len = strlen(cache->chars);

    char newStr[len - 1];
    SDL_Surface *newS[len - 1];
    SDL_Color newFg[len - 1];
    SDL_Color newBg[len - 1];

    for (int i = 0; i < len; ++i) {
      if (cache->chars[i] != c) {
        newStr[i] = cache->chars[i];
        newS[i] = cache->s[i];
        newFg[i] = cache->colors_fg[i];
        newBg[i] = cache->colors_bg[i];
      }
    }
    // realloc & re-set arrays
    cache->chars = (char *)realloc(cache->chars, (len - 1) * sizeof(char));
    strcpy(cache->chars, newStr);

    cache->s =
        (SDL_Surface **)realloc(cache->s, (len - 1) * sizeof(SDL_Surface *));
    memcpy(cache->s, newS, (len - 1) * sizeof(SDL_Surface *));

    cache->colors_bg =
        (SDL_Color *)realloc(cache->colors_bg, (len - 1) * sizeof(SDL_Color));
    memcpy(cache->colors_bg, newBg, (len - 1) * sizeof(SDL_Color));

    cache->colors_fg =
        (SDL_Color *)realloc(cache->colors_fg, (len - 1) * sizeof(SDL_Color));
    memcpy(cache->colors_fg, newFg, (len - 1) * sizeof(SDL_Color));

  } else {
    debug_print("char not in cache ! \n");
  }
}

void tgr_destroyCache(textureCache_t *cache) {
  free(cache->chars);
  free(cache->s);
  free(cache->colors_bg);
  free(cache->colors_fg);
  free(cache);
}
