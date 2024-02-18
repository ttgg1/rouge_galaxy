#include <stdio.h>

#include "entity.h"
#include "game.h"
#include "vecMath.h"

int main(int argc, char **argv) {
  game_t *game = gm_init(30, 30, 40);

  entity_t ent = {0, {10, 10}, 'E'};
  entity_t ent2 = {0, {11, 10}, 'B'};

  gm_addEntity(&ent, game);
  gm_addEntity(&ent2, game);
  gm_start(game);

  gm_stop(game);
  return 0;
}
