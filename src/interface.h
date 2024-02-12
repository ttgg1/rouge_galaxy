#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "entity.h"
#include "vecMath.h"
#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct interface {
  uint8_t w;
  uint8_t h;
  char **pixels;
  WINDOW *win;
} interface_t;

interface_t in_create(uint8_t width, uint8_t height);
void in_destroy(interface_t *in);

void in_drawAt(interface_t *in, chtype c, ivec2_t pos);

// needs to be called AFTER draw calls
void in_drawRefresh(interface_t *in);

#endif // INTERFACE_H_
