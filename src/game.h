#ifndef GAME_H_
#define GAME_H_

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "entity.h"
#include "interface.h"
#include "list.h"
#include "player.h"
#include "ui.h"
#include "utils.h"

typedef struct game {
  bool isRunning;
  interface_t *in;
  player_t *p;

  list_t *en_list;
  list_t *active_entities;

  list_t *uiWindowList;
} game_t;

void gm_start(game_t *g);
void gm_stop(game_t *g);

void gm_addEntity(entity_t *e, game_t *g);
void gm_addUiWindow(ui_win_t *win, game_t *g);

game_t *gm_init(uint8_t grid_w, uint8_t grid_h, uint8_t ptsize);

void gm_updateGrid(game_t *g);

bool gm_entityOnGrid(entity_t *e, game_t *g);

#endif // GAME_H_
