#ifndef PLAYER_H_
#define PLAYER_H_

#include "entity.h"
#include "utils.h"
#include "vecMath.h"
#include <raylib.h>

typedef struct player {
  entity_t *e;
} player_t;

player_t *pl_createPlayer(ivec2_t pos);
void pl_destroyPlayer(player_t *p);

// call after case KEYDOWN
void pl_handleMovement(player_t *p);

#endif // PLAYER_H_
