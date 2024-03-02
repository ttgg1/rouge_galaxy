#include "game.h"

uint8_t num_entities = 0;

void drawUi(game_t *g) {
  node_t *curr = g->uiWindowList->head;
  while (curr != NULL) {
    ui_win_t *curr_val = (ui_win_t *)curr->value;

    ui_drawWindow(curr_val, g->in);

    curr = curr->next;
  }
}

void draw(game_t *g) {
  in_clearScreen(g->in);
  // draw stuff
  gm_updateGrid(g);
  drawUi(g);

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
  li_destroy(g->active_entities);
  free(g);
}

void gm_addEntity(entity_t *e, game_t *g) { li_push(g->en_list, e); }

void gm_addUiWindow(ui_win_t *win, game_t *g) { li_push(g->uiWindowList, win); }

/*
  odd grid_w and grid_h for centered player
*/
game_t *gm_init(uint8_t grid_w, uint8_t grid_h, uint8_t ptsize) {

  srand(time(NULL));

  game_t *g = malloc(sizeof(game_t));
  interface_t *inter;

  // create text interface and store it in game struct
  inter = in_create(grid_w, grid_h, ptsize);

  g->in = inter;
  // Spawns player at 0,0
  g->p = pl_createPlayer((ivec2_t){0, 0});

  // init entity linked list
  g->en_list = li_emptyList();
  g->active_entities = li_emptyList();
  g->uiWindowList = li_emptyList();

  // init map
  g->map = m_create();

  static const bool tempCons[NUM_CONSTRAINTS * 4 * NUM_CONSTRAINTS] = 
  {
    true,false, false,true, true,false, false,true,
    false,true, true,false, false,true, true,false
  };
  /*{
    true,false,false,false,false, true,true,true,true,true, true,true,true,true,true, true,false,false,false,false,
    true,true,false,false,false, false,true,true,true,true, false,true,true,true,true, true,true,false,false,false,
    true,true,true,false,false, false,false,true,true,true, false,false,true,true,true, true,true,true,false,false,
    true,true,true,true,false, false,false,false,true,true, false,false,false,true,true, true,true,true,true,false,
    true,true,true,true,true, false,false,false,false,true, false,false,false,false,true, true,true,true,true,true
  };*/  
  memcpy(g->map->constraints, tempCons, sizeof tempCons);

  static const uint32_t tempTiles[NUM_CONSTRAINTS] = {(uint32_t) '#', (uint32_t) '.'};//{(uint32_t) '1', (uint32_t) '2', (uint32_t) '3', (uint32_t) '4', (uint32_t) '5'};
  memcpy(g->map->tileset, tempTiles, sizeof tempTiles);

  m_setAt(g->map, 1200, 1200, (uint32_t)'T');
  m_generateMap(g->map, -1000, 1000, -1000, 1000);

  // add Player to Entity list
  gm_addEntity(g->p->e, g);

  return g;
}

bool gm_entityOnGrid(entity_t *e, game_t *g) {
  return e->pos.x >= g->p->e->pos.x - g->in->w / 2 &&
         e->pos.x <= g->p->e->pos.x + g->in->w / 2 &&
         e->pos.y >= g->p->e->pos.y - g->in->h / 2 &&
         e->pos.y <= g->p->e->pos.y + g->in->h / 2;
}

void gm_updateGrid(game_t *g) {
  // clear grid
  // memset(g->in->grid, '.', g->in->w * g->in->h * sizeof(char));

  int offsetY, offsetX;
  offsetY = g->in->h / 2;
  offsetX = g->in->w / 2;

  // handle map

  for (int y = 0; y < g->in->h; y++) {
    for (int x = 0; x < g->in->w; x++) {
      in_drawAtColored(
        g->in,
        m_getAt(g->map, y + g->p->e->pos.y - offsetY, x + g->p->e->pos.x - offsetX),
        (SDL_Color){200, 200, 200, 255},
        (ivec2_t){x,y}
      );
    }
  }

  // handle entities
  // TODO: refactor to use active_entities list
  node_t *current = g->en_list->head;
  while (current != NULL) {
    entity_t *curr_val = (entity_t *)current->value;
    if (curr_val != NULL && gm_entityOnGrid(curr_val, g)) {
      ivec2_t pos =
          (ivec2_t){(int16_t)((curr_val->pos.x) - g->p->e->pos.x + offsetX),
                    (int16_t)((curr_val->pos.y) - g->p->e->pos.y + offsetY)};
      in_drawAtColored(g->in, curr_val->c, curr_val->color, pos);
    }
    current = current->next;
  }
}
