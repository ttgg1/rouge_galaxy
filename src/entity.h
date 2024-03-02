#ifndef ENTITY_H_
#define ENTITY_H_

#include "utils.h"
#include "vecMath.h"
#include <math.h>
#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

void en_move(entity_t* e, int16_t x_step, int16_t y_step);

entity_t* en_create(uint8_t id, ivec2_t pos, int c, Color color);

void en_destroy(entity_t* e);

#endif // ENTITY_H_
