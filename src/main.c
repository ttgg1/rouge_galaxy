#include <ncurses.h>
#include <stdio.h>

#include "entity.h"
#include "game.h"
#include "vecMath.h"

int main(int argc, char **argv) {
  game_t game = gm_init(200, 100);

  entity_t ent = {0, {2, 2}, {0, 0}, 'E' | A_BOLD};

  gm_start(&game);
  gm_addEntity(&ent, &game);

  return 0;
}
