#include <ncurses.h>
#include <stdio.h>

#include "entity.h"
#include "game.h"
#include "vecMath.h"

int main(int argc, char **argv) {
  game_t game = gm_init(200, 100);

  entity_t ent = {0, {20, 20}, {0, 0}, 'E' | A_BLINK | COLOR_PAIR(2)};

  gm_addEntity(&ent, &game);
  gm_start(&game);

  return 0;
}
