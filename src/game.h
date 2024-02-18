#ifndef GAME_H_
#define GAME_H_

#include <stdbool.h>
#include <string.h>

#include "entity.h"
#include "interface.h"
#include "player.h"
#include "utils.h"
#include "entityList.h"


typedef struct game {
  bool isRunning;
  interface_t *in;
  entity_list_t *en_list;
  player_t *p;
} game_t;

void gm_start(game_t *g);
void gm_stop(game_t *g);

void gm_addEntity(entity_t *e, game_t *g);

game_t *gm_init(uint8_t grid_w, uint8_t grid_h, uint8_t ptsize);

#endif // GAME_H_
