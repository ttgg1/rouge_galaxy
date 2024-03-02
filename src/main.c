#include <stdio.h>

#define COLOR 1

#include "entity.h"
#include "game.h"
#include "ui.h"
#include "vecMath.h"
#include "map.h"

int main(int argc, char **argv) {

  game_t *game = gm_init(15, 15, 20);

  entity_t ent = {0, {10, 10}, 'E', {255, 0, 0, 255}};
  entity_t ent2 = {0, {11, 10}, 'B', {0, 255, 0, 255}};

  gm_addEntity(&ent, game);
  gm_addEntity(&ent2, game);

  gm_start(game);

  gm_stop(game);
  return 0;
}
