#include "interface.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_ttf.h"

SDL_Color in_bg = {20, 20, 20, 255};
SDL_Color in_fg = {200, 200, 200, 255};

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

  SDL_SetWindowTitle(res.win, "Rouge Galaxy");

  // init Grid
  size_t mem_size = res.w * res.h * sizeof(char);

  res.grid = malloc(mem_size);
  memset(res.grid, '.', mem_size);

  // format grid array so it can be drawn directly
  /* for(int i = 0; i< ((res.w+1)*res.h)-1;++i){ */
  /*   if(i % res.w == 0)res.grid[i+1] = '\n'; */
  /* } */

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
void in_drawPresent(interface_t *in) {
  // Draw gird text to Surface
  Uint32 wl = in->grid_cell_w * in->w;
  SDL_Surface *s =
      TTF_RenderText_Shaded_Wrapped(in->f, in->grid, in_fg, in_bg, wl);

  // Create Texture & render
  SDL_Texture *t = SDL_CreateTextureFromSurface(in->r, s);

  SDL_RenderCopy(in->r, t, NULL, NULL);
  SDL_RenderPresent(in->r);

  SDL_FreeSurface(s);
  SDL_DestroyTexture(t);
}

void in_clearScreen(interface_t *in) {
  SDL_SetRenderDrawColor(in->r, in_bg.r, in_bg.g, in_bg.b, in_bg.a);
  SDL_RenderClear(in->r);
}
