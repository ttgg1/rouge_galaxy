#include "entity.h"

void en_move(entity_t *e) {
  e->pos.x += e->vel.x;
  e->pos.y += e->vel.y;
}
