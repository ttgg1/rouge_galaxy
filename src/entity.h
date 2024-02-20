#ifndef ENTITY_H_
#define ENTITY_H_

#define COLOR 1

#include "vecMath.h"
#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef COLOR
typedef struct entity {
  uint8_t ID;
  ivec2_t pos;
  char c;
} entity_t;
#else
typedef struct entity {
  uint8_t ID;
  ivec2_t pos;
  uint32_t c; // Colored mode supports 32bit encoding, (even emojis)
  SDL_Color color;
} entity_t;
#endif

void en_move(entity_t *e, int16_t x_step, int16_t y_step);

entity_t *en_create(uint8_t id, ivec2_t pos, char c);

#endif // ENTITY_H_
