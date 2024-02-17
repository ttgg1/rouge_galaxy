#include "player.h"

player_t *pl_create_player(ivec2_t pos) {
  player_t *res;
  res = (player_t *) malloc(sizeof(player_t));
  res->e = en_create(0, pos, 'P');

  return res;
}

void pl_destroy_player(player_t *p) {
  free(p);
}

void pl_handleMovement(player_t *p, SDL_Event *e) {
  switch (e->key.keysym.sym) {
  case SDLK_UP:
    en_move(p->e, 0, -1);
    break;

  case SDLK_DOWN:
    en_move(p->e, 0, 1);
    break;

  case SDLK_RIGHT:
    en_move(p->e, 1, 0);
    break;

  case SDLK_LEFT:
    en_move(p->e, -1, 0);
    break;
  
  case SDLK_d:
    1;
    break;
  }
}
