#include <stdio.h>

#include "entity.h"
#include "game.h"
#include "vecMath.h"

int main(int argc, char **argv) {
  game_t *game = gm_init(25, 25, 30);

  entity_t *ent = en_create(1, (ivec2_t){10, 10}, 'E');
  entity_t *ent2 = en_create(1, (ivec2_t){11, 10}, 'B');

  gm_addEntity(ent, game);
  gm_addEntity(ent2, game);
  gm_start(game);

  gm_stop(game);
  return 0;
}
