#include "interface.h"
#include "SDL_error.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_ttf.h"
#include "utils.h"
#include "vecMath.h"
#include <stdint.h>
#include <string.h>

SDL_Color in_bg = {20, 20, 20, 255};
SDL_Color in_fg = {200, 200, 200, 255};

interface_t *in_create(uint8_t grid_w, uint8_t grid_h, uint8_t ptsize) {
  interface_t *res = malloc(sizeof(interface_t));

  res->w = grid_w;
  res->h = grid_h;

  // init SDL2 & SDL2_ttf

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    debug_print("SDL2 Failed to intitialize Video: %s \n", SDL_GetError());
  }
  if (TTF_Init() < 0) {
    debug_print("SDL2_ttf Failed to intitialize: %s \n", SDL_GetError());
  }

  // Open Font
  char *relPath;
#ifdef __LINUX__
  relPath = "/../../assets/unifont-15.1.04.otf";
#else
  relPath = "\\..\\assets\\unifont-15.1.04.otf";
#endif

  int pathLenght = cwdPathLenght(relPath);
  char path[pathLenght];
  cwdJoinPath(relPath, path);
  res->f = TTF_OpenFont(path, (int)ptsize);
  if (res->f == NULL) {
    debug_print("SDL2_ttf Failed to intitialize the Font: %s \n",
                SDL_GetError());
  }

  // get grid_cell_w and h
  if (TTF_SizeUTF8(res->f, " ", (int *)&res->grid_cell_w,
                   (int *)&res->grid_cell_h) < 0) {
    debug_print("SDL2_ttf Failed to fetch the Fontsize: %s \n", SDL_GetError());
  }

  // create SDL window and renderer
  int win_w = res->w * res->grid_cell_w * 2;
  int win_h = res->h * res->grid_cell_h;

  if (SDL_CreateWindowAndRenderer(win_w, win_h, SDL_WINDOW_OPENGL, &res->win,
                                  &res->r) < 0) {
    debug_print("SDL2 Failed to intitialize Window and/or Renderer: %s \n",
                SDL_GetError());
  }

  SDL_SetWindowTitle(res->win, "Rouge Galaxy");

  // init Grid

  size_t nobjs = (res->w * res->h) + 1;
#ifndef COLOR
  res->grid = calloc(nobjs, sizeof(char));
#else
  res->grid = calloc(nobjs, sizeof(uint32_t));

  // init colormap
  res->colormap = malloc(nobjs * sizeof(SDL_Color));
  for (int i = 0; i < nobjs; ++i) {
    res->colormap[i] = in_fg;
  }
#endif
  // terminate String
  res->grid[nobjs] = '\0';

  return res;
}

void in_destroy(interface_t *in) {
  free(in->grid);

  TTF_CloseFont(in->f);
  TTF_Quit();

  SDL_DestroyRenderer(in->r);
  SDL_DestroyWindow(in->win);
  SDL_Quit();

  free(in);
}

void in_drawAt(interface_t *in, char c, ivec2_t pos) {
#ifndef COLOR
  int index = pos.x + pos.y * in->w;
  if (index < 0 || index >= in->h * in->w) {
    debug_print("Tried to write to out-of-bounds !\n");
    return;
  }
  if (in->grid == NULL) {
    debug_print("Tried to write to an uninitialized grid array !\n");
  } else {
    in->grid[index] = c;
  }
#else
  in_drawAtColored(in, (uint32_t)c, in_fg, pos);
#endif
}

void in_drawAtColored(interface_t *in, uint32_t c, SDL_Color color,
                      ivec2_t pos) {
#ifdef COLOR
  int index = GET_GRID_INDEX(pos.x, pos.y, in->w);

  if (in->grid == NULL || in->colormap == NULL) {
    debug_print(
        "Tried to write to an unititialized Grid array or colormap !\n");
  } else {
    in->grid[index] = c;
    in->colormap[index] = color;
  }
#else
  in_drawAt(in, (char)c, pos);
#endif
}

void in_drawEntity(interface_t *in, entity_t *e) {
#ifndef COLOR
  in_drawAt(in, e->c, e->pos);
#else
  in_drawAtColored(in, e->c, e->color, e->pos);
#endif
}

void in_drawPresent(interface_t *in) {
#ifndef COLOR
  // Draw gird text to Surface
  Uint32 wl = in->grid_cell_w * in->w;
  SDL_Surface *s =
      TTF_RenderText_Shaded_Wrapped(in->f, in->grid, in_fg, in_bg, wl);

  if (s == NULL) {
    debug_print("Failed to render text to SDL_Surface: %s \n", SDL_GetError());
  }

  // Create Texture & render
  SDL_Texture *t = SDL_CreateTextureFromSurface(in->r, s);

  if (t == NULL) {
    debug_print("Failed to create SDL Texture form surface %s \n",
                SDL_GetError());
  }

  SDL_RenderCopy(in->r, t, NULL, NULL);
  SDL_RenderPresent(in->r);

  SDL_FreeSurface(s);
  SDL_DestroyTexture(t);
#else
  // TODO: maybe use one big texture that is filled with surfaces
  //  draw colored in 32 bit encoding (much slower)
  SDL_Rect pos_r = {.x = 0, .y = 0, .w = in->grid_cell_w, .h = in->grid_cell_h};
  for (int i = 0; i < in->h * in->w; ++i) {
    SDL_Surface *s =
        TTF_RenderGlyph32_Shaded(in->f, in->grid[i], in->colormap[i], in_bg);
    SDL_Texture *t = SDL_CreateTextureFromSurface(in->r, s);

    SDL_RenderCopy(in->r, t, NULL, &pos_r);

    SDL_FreeSurface(s);
    SDL_DestroyTexture(t);

    // increase position
    pos_r.x += in->grid_cell_w * 2;
    if (pos_r.x > in->w * in->grid_cell_w * 2) {
      pos_r.x = 0;
      pos_r.y += in->grid_cell_h;
    }
  }
  SDL_RenderPresent(in->r);
#endif
}

void in_clearScreen(interface_t *in) {
  int nobjs = in->w * in->h;
#ifndef COLOR
  memset(in->grid, '.', nobjs * sizeof(char));
#else
  for (int i = 0; i < nobjs; ++i) {
    in->colormap[i] = in_fg;
    in->grid[i] = (uint32_t)'.';
  }
#endif
  SDL_SetRenderDrawColor(in->r, in_bg.r, in_bg.g, in_bg.b, in_bg.a);
  SDL_RenderClear(in->r);
}
