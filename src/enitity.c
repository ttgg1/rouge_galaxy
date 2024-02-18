#include "entity.h"

void en_move(entity_t *e, int8_t x_step, int8_t y_step) {
  e->pos.x += x_step;
  e->pos.y += y_step;

  if (e->pos.x < 0)
    e->pos.x = 0;
  if (e->pos.y < 0)
    e->pos.y = 0;
}
