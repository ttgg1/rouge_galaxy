#include "game.h"
#include <stdio.h>

uint8_t num_entities = 0;

void draw(game_t *g) {
  in_clearScreen(g->in);
  //draw stuff
  gm_updateGrid(g);

  in_drawPresent(g->in);
}

void handleEvents(game_t *g) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_QUIT:
      g->isRunning = false;
      break;

    case SDL_KEYDOWN:
      pl_handleMovement(g->p, &e);
      break;
    }
  }
}

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
  pl_destroyPlayer(g->p);
  in_destroy(g->in);
  li_destroy(g->en_list);
  free(g);
}

void gm_addEntity(entity_t *e, game_t *g) {
  li_push(g->en_list, e);
}

/*
  odd grid_w and grid_h for centered player
*/
game_t *gm_init(uint8_t grid_w, uint8_t grid_h, uint8_t ptsize) {
  game_t *g = malloc(sizeof(game_t));
  interface_t *inter;

  // create text interface and store it in game struct
  inter = in_create(grid_w, grid_h, ptsize);

  g->in = inter;
  // Spawns player at 0,0
  g->p = pl_createPlayer((ivec2_t){0, 0});

  // init entity linked list
  g->en_list = li_emptyList();


  // add Player to Entity list
  gm_addEntity(g->p->e, g);

  return g;
}

bool gm_entityOnGrid(entity_t *e, game_t *g) {
  return e->pos.x >= g->p->e->pos.x - g->in->w / 2 
      && e->pos.x <= g->p->e->pos.x + g->in->w / 2
      && e->pos.y >= g->p->e->pos.y - g->in->h / 2 
      && e->pos.y <= g->p->e->pos.y + g->in->h / 2;
}


void gm_updateGrid(game_t *g) {
  // clear grid
  memset(g->in->grid, '.', g->in->w * g->in->h * sizeof(char));

  int offsetY, offsetX;
  offsetY = g->in->h / 2;
  offsetX = g->in->w / 2;

  // handle map 

  // TODO

  // handle entities

  entity_node_t *current = g->en_list->head;
  while (current != NULL) {
    if (current->value != NULL && gm_entityOnGrid(current->value, g)) {
      in_drawAt(g->in, current->value->c, (ivec2_t){current->value->pos.x - g->p->e->pos.x + offsetX, current->value->pos.y - g->p->e->pos.y + offsetY});
    }
    current = current->next;
  }

  // set EoL
  g->in->grid[g->in->w * g->in->h] = '\0';
}