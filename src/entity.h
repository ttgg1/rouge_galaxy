#ifndef ENTITY_H_
#define ENTITY_H_

#include "vecMath.h"
#include <stdint.h>

typedef struct entity {
  uint8_t ID;
  ivec2_t pos;
  ivec2_t vel;
  char c;
} entity_t;

void en_move(entity_t *e);

#endif // ENTITY_H_
