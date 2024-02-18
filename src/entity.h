#ifndef ENTITY_H_
#define ENTITY_H_

#define COLOR 1

#include "vecMath.h"
#include <SDL2/SDL.h>
#include <stdint.h>

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

void en_move(entity_t *e, int8_t x_step, int8_t y_step);

#endif // ENTITY_H_
