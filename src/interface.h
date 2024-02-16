#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "entity.h"
#include "utils.h"
#include "vecMath.h"
#include <stdint.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct interface {
  uint8_t w;
  uint8_t h;
  uint8_t grid_cell_w;
  uint8_t grid_cell_h;
  char *grid;

  SDL_Window *win;
  SDL_Renderer *r;
  TTF_Font *f;
} interface_t;

interface_t *in_create(uint8_t grid_w, uint8_t grid_h, uint8_t ptsize);
void in_destroy(interface_t *in);

void in_drawAt(interface_t *in, char c, ivec2_t pos);

// needs to be called AFTER draw calls
void in_drawPresent(interface_t *in);
void in_clearScreen(interface_t *in);

#endif // INTERFACE_H_
