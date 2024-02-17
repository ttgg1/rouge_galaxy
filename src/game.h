#ifndef GAME_H_
#define GAME_H_

#include "entity.h"
#include "interface.h"
#include "player.h"
#include "utils.h"
#include <stdbool.h>
#include <string.h>

#define LIST_NODE_TYPE entity_t
#undef LIST_H_
#include "list.h"


typedef struct game {
  bool isRunning;
  interface_t *in;
  entity_t *(*en_list);
  linked_list_entity_t_t *ent_list;
  player_t *p;
} game_t;

void gm_start(game_t *g);
void gm_stop(game_t *g);

void gm_addEntity(entity_t *e, game_t *g);

game_t *gm_init(uint8_t grid_w, uint8_t grid_h, uint8_t ptsize);

#endif // GAME_H_
