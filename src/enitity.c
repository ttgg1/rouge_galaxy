#include "entity.h"

void en_move(entity_t *e, int16_t x_step, int16_t y_step) {
  e->pos.x += x_step;
  e->pos.y += y_step;
}

entity_t *en_create(uint8_t id, ivec2_t pos, int c, Color color) {
  entity_t *e;

  e = (entity_t *)malloc(sizeof(entity_t));

  if (e == NULL) {
    debug_print("Entity allocation failed !\n");
    return NULL;
  }

  e->ID = id;
  e->pos = pos;
  e->c = c;
  e->color = color;
  return e;
}

void en_destroy(entity_t *e) { free(e); }
