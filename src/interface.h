#ifndef INTERFACE_H_
#define INTERFACE_H_

// foreground and background colors
#define in_bg                                                                  \
  (Color) { 20, 20, 20, 255 }
#define in_fg                                                                  \
  (Color) { 200, 200, 200, 255 }

#define WIN_W 1000
#define WIN_H 1000

#include "entity.h"
#include "utils.h"
#include "vecMath.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <raylib.h>

typedef struct interface {
    uint16_t w;
    uint16_t h;
    uint8_t grid_cell;
    float ptsize;
    int **grid; // uses raylib codepoints
    Color **colormap;

    Font f;
} interface_t;

interface_t *in_create(uint8_t grid_w, uint8_t grid_h, float ptsize);
void in_destroy(interface_t *in);

void in_drawAt(interface_t *in, char c, ivec2_t pos);
void in_drawAtColored(interface_t *in, char c, Color color, ivec2_t pos);

void in_drawArrayColored(interface_t *in, char **chars, Color **colors,
                         ivec2_t start_pos, int width, int height);

void in_drawEntity(interface_t *in, entity_t *e);

// maybe add support to draw codepoints ?

// needs to be called AFTER draw calls
void in_drawPresent(interface_t *in, Camera2D *cam);
void in_clearScreen(interface_t *in);

#endif // INTERFACE_H_
