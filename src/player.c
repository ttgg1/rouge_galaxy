#include "player.h"

player_t *pl_createPlayer(ivec2_t pos) {
  player_t *res = malloc(sizeof(player_t));
  *res->e = (entity_t){.ID = 0,
                       .pos = pos,
                       .c = 'P',
                       .color = {.r = 10, .g = 128, .b = 128, .a = 255}};

  return res;
}

void pl_destroyPlayer(player_t *p) {
  // cleanup player

  // free player struct
  free(p);
}

void pl_handleMovement(player_t *p) {
  switch (GetKeyPressed()) {
  case KEY_UP:
    en_move(p->e, 0, -1);
    break;

  case KEY_DOWN:
    en_move(p->e, 0, 1);
    break;

  case KEY_RIGHT:
    en_move(p->e, 1, 0);
    break;

  case KEY_LEFT:
    en_move(p->e, -1, 0);
    break;
  }
}
