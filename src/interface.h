#ifndef INTERFACE_H_
#define INTERFACE_H_

#define COLOR 1
#define GET_GRID_INDEX(x, y, w) (x + (y * w))
// foreground and background colors
#define in_bg                                                                  \
  (SDL_Color) { 20, 20, 20, 255 }
#define in_fg                                                                  \
  (SDL_Color) { 200, 200, 200, 255 }

#include "entity.h"
#include "utils.h"
#include "vecMath.h"
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#ifndef COLOR
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
#else
typedef struct interface {
  uint8_t w;
  uint8_t h;
  uint8_t grid_cell_w;
  uint8_t grid_cell_h;
  uint32_t *grid;
  SDL_Color *colormap;

  SDL_Window *win;
  SDL_Renderer *r;
  TTF_Font *f;
} interface_t;
#endif

interface_t *in_create(uint8_t grid_w, uint8_t grid_h, uint8_t ptsize);
void in_destroy(interface_t *in);

void in_drawAt(interface_t *in, char c, ivec2_t pos);
void in_drawAtColored(interface_t *in, uint32_t c, SDL_Color color,
                      ivec2_t pos);

void in_drawArrayColored(interface_t *in, uint32_t *chars, SDL_Color *colors,
                         ivec2_t start_pos, int len, int wrap_length);

void in_drawEntity(interface_t *in, entity_t *e);

// needs to be called AFTER draw calls
void in_drawPresent(interface_t *in);
void in_clearScreen(interface_t *in);

#endif // INTERFACE_H_
