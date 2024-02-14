#ifndef GAME_H_
#define GAME_H_

#include "entity.h"
#include "interface.h"
#include "utils.h"
#include <stdbool.h>
#include <string.h>

typedef struct game {
  bool isRunning;
  interface_t *in;
  entity_t *en_list;
} game_t;

void gm_start(game_t *g);
void gm_stop(game_t *g);

void gm_addEntity(entity_t *e, game_t *g);

game_t gm_init(uint8_t window_w, uint8_t window_h);

#endif // GAME_H_
