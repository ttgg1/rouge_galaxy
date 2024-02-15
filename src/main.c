#include <ncurses.h>
#include <stdio.h>

#include "entity.h"
#include "game.h"
#include "vecMath.h"

int main(int argc, char **argv) {
  game_t game = gm_init(20, 20, 20);

  entity_t ent = {0, {10, 10}, {0, 0}, 'E'};

  gm_addEntity(&ent, &game);
  gm_start(&game);

  return 0;
}
