#ifndef ENTITY_H_
#define ENTITY_H_

#include "vecMath.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct entity {
  uint8_t ID;
  ivec2_t pos;
  char c;
} entity_t;

void en_move(entity_t *e, int16_t x_step, int16_t y_step);

entity_t *en_create(uint8_t id, ivec2_t pos, char c);

#endif // ENTITY_H_
