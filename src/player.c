#include "player.h"

player_t pl_createPlayer(ivec2_t pos) {
  player_t res;
  res.e = (entity_t){.ID = 0, .pos = pos, .c = 'P'};

  return res;
}

void pl_handleMovement(player_t *p, SDL_Event *e) {
  switch (e->key.keysym.sym) {
  case SDLK_UP:
    en_move(&p->e, 0, -1);
    break;

  case SDLK_DOWN:
    en_move(&p->e, 0, 1);
    break;

  case SDLK_RIGHT:
    en_move(&p->e, 1, 0);
    break;

  case SDLK_LEFT:
    en_move(&p->e, -1, 0);
    break;
  }
}
