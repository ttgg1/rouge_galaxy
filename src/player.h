#ifndef PLAYER_H_
#define PLAYER_H_

#include "entity.h"
#include "vecMath.h"
#include <SDL2/SDL.h>

typedef struct player {
  entity_t *e;
} player_t;

player_t *pl_createPlayer(ivec2_t pos);
void pl_destroyPlayer(player_t *);


// call after case SDL_KEYDOWN
void pl_handleMovement(player_t *p, SDL_Event *e);

#endif // PLAYER_H_
