#include "interface.h"
#include "SDL_render.h"
#include "SDL_ttf.h"
#include "SDL_video.h"

interface_t in_create(uint8_t grid_w, uint8_t grid_h, uint8_t ptsize) {
  interface_t res;

  res.w = grid_w;
  res.h = grid_h;

  // init SDL2 & SDL2_ttf

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    debug_print("SDL2 Failed to intitialize Video: %s \n", SDL_GetError());
  }
  if (TTF_Init() < 0) {
    debug_print("SDL2_ttf Failed to intitialize: %s \n", SDL_GetError());
  }

  // Open Font
  res.f = TTF_OpenFont("../../assets/FiraMono.ttf", (int)ptsize);
  if (res.f == NULL) {
    debug_print("SDL2_ttf Failed to intitialize the Font: %s \n",
                SDL_GetError());
  }

  // get grid_cell_w and h
  if (TTF_SizeUTF8(res.f, " ", (int *)&res.grid_cell_w,
                   (int *)&res.grid_cell_h) < 0) {
    debug_print("SDL2_ttf Failed to fetch the Fontsize: %s \n", SDL_GetError());
  }

  // create SDL window and renderer
  int win_w = res.w * res.grid_cell_w;
  int win_h = res.h * res.grid_cell_h;

  if (SDL_CreateWindowAndRenderer(win_w, win_h, NULL, &res.win, &res.r) < 0) {
    debug_print("SDL2 Failed to intitialize Window and/or Renderer: %s \n",
                SDL_GetError());
  }

  // init Grid
  res.grid = malloc(res.w * res.h * sizeof(char));

  return res;
}

void in_destroy(interface_t *in) {
  free(in->grid);

  TTF_CloseFont(in->f);
  TTF_Quit();

  SDL_DestroyRenderer(in->r);
  SDL_DestroyWindow(in->win);
  SDL_Quit();
}

void in_drawAt(interface_t *in, char c, ivec2_t pos) {
  int index = pos.x + pos.y * in->w;

  in->grid[index] = c;
}
void in_drawPresent(interface_t *in) { SDL_RenderPresent(in->r); }

void in_clearScreen(interface_t *in) { SDL_RenderClear(in->r); }
