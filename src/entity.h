#ifndef ENTITY_H_
#define ENTITY_H_

#include "vecMath.h"
#include <stdint.h>

typedef struct entity {
  uint8_t ID;
  ivec2_t pos;
  char c;
} entity_t;

void en_move(entity_t *e, int8_t x_step, int8_t y_step);

#endif // ENTITY_H_
