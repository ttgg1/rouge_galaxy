#include "game.h"

uint8_t num_entities = 0;

void draw(game_t *g) {
  in_clearScreen(g->in);
  // draw stuff
  for (int i = 0; i < num_entities; ++i) {
    // draw all entities
    in_drawAt(g->in, g->en_list[i].c, g->en_list[i].pos);
  }
  in_drawPresent(g->in);
}
// TODO
void handleEvents(game_t *g) {}

void loop(game_t *g) {
  while (g->isRunning) {
    handleEvents(g);
    draw(g);
  }
}

void gm_start(game_t *g) {
  g->isRunning = true;
  loop(g);
}

void gm_stop(game_t *g) {
  g->isRunning = false;
  free(g->en_list);
  in_destroy(g->in);
  free(g);
}

void gm_addEntity(entity_t *e, game_t *g) {
  // allocate memory if not created
  if (num_entities == 0) {
    g->en_list = malloc(sizeof(entity_t));
  } else {
    // if there is already an entity list allocated -> realloc
    entity_t *en_temp_p =
        realloc(g->en_list, (num_entities + 1) * sizeof(entity_t));
    // check if realloc worked
    if (en_temp_p == NULL) {
      debug_print("Enity-List failed to reallocate !!\n");
    } else {
      g->en_list = en_temp_p;
    }
  }
  // add Entity to list
  g->en_list[num_entities] = *e;
  ++num_entities;
}

game_t *gm_init(uint8_t grid_w, uint8_t grid_h, uint8_t ptsize) {
  game_t *g = malloc(sizeof(game_t));
  interface_t *inter;

  // create text interface and store it in game struct
  inter = in_create(grid_w, grid_h, ptsize);

  g->in = inter;

  // leave entity list empty for now
  num_entities = 0;

  return g;
}
