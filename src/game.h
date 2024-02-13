#ifndef GAME_H_
#define GAME_H_

#include "entity.h"
#include "interface.h"
#include <stdbool.h>

typedef struct game {
  bool isRunning;
  interface_t *in;
  entity_t *en_list;
} game_t;

void gm_start(void);
void gm_stop(void);

void gm_init(void);

#endif // GAME_H_
