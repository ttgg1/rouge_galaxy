#ifndef ENTITY_H_
#define ENTITY_H_

#include "utils.h"
#include "vecMath.h"
#include <math.h>
#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct entity {
  uint8_t ID;
  ivec2_t pos;
  int c; // Colored mode supports unicode
  Color color;
} entity_t;

void en_move(entity_t *e, int16_t x_step, int16_t y_step);

entity_t *en_create(uint8_t id, ivec2_t pos, int c, Color color);

void en_destroy(entity_t *e);

#endif // ENTITY_H_
