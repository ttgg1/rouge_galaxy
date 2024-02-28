#include "game.h"

uint8_t num_entities = 0;

void checkPlayerBounds(game_t *g) {
  ivec2_t plPos = g->p->e->pos;
  int grid_w = g->in->w;
  int grid_h = g->in->h;

  if (plPos.x > grid_w) {
    plPos.x = (int16_t)grid_w;
  }
  if (plPos.y > grid_h) {
    plPos.y = (int16_t)grid_h;
  }

  if (plPos.x < 0) {
    plPos.x = 0;
  }
  if (plPos.y < 0) {
    plPos.y = 0;
  }
}

void updateCamera(game_t *g) {
  g->cam->zoom += ((float)GetMouseWheelMove() * 0.05f);

  if (g->cam->zoom > 3.0f)
    g->cam->zoom = 3.0f;
  else if (g->cam->zoom < 0.25f)
    g->cam->zoom = 0.25f;

  static float minSpeed = 30;
  static float minEffectLength = 5;
  static float fractionSpeed = 0.8f;

  Vector2 plPos = ivec2ToScreenspace(g->p->e->pos, g->in->grid_cell);

  g->cam->offset = (Vector2){WIN_W / 2.0f, WIN_H / 2.0f};
  Vector2 diff = Vector2Subtract(plPos, g->cam->target);
  float length = Vector2Length(diff);

  if (length > minEffectLength) {
    float speed = fmaxf(fractionSpeed * length, minSpeed);
    g->cam->target = Vector2Add(g->cam->target,
                                Vector2Scale(diff, speed * g->delta / length));
  }
}

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
  updateCamera(g);
  // draw stuff
  gm_updateGrid(g);
  drawUi(g);

  in_drawPresent(g->in, g->cam);
}

void handleEvents(game_t *g) {
  printf("delta: %f, FPS: %d\n", g->delta, g->fps);
  pl_handleMovement(g->p, g->delta);
  checkPlayerBounds(g);
}

void loop(game_t *g) {
  while (g->isRunning && !WindowShouldClose()) {
    g->delta = GetFrameTime();
    g->fps = GetFPS();
    handleEvents(g);
    draw(g);
  }
}

void gm_start(game_t *g) {
  g->isRunning = true;
  loop(g);
  // stop if isRunning = false
  gm_stop(g);
}

void gm_stop(game_t *g) {
  g->isRunning = false;
  pl_destroyPlayer(g->p);
  in_destroy(g->in);
  li_destroy(g->en_list);
  li_destroy(g->active_entities);
  free(g->cam);
  // TODO: traverse lists and delete all entities
  free(g);
}

void gm_addEntity(entity_t *e, game_t *g) { li_push(g->en_list, e); }

void gm_addUiWindow(ui_win_t *win, game_t *g) { li_push(g->uiWindowList, win); }

/*
  odd grid_w and grid_h for centered player
*/
game_t *gm_init(uint8_t grid_w, uint8_t grid_h, uint8_t ptsize) {
  game_t *g = (game_t *)malloc(sizeof(game_t));
  interface_t *inter;

  // create text interface and store it in game struct
  inter = in_create(grid_w, grid_h, ptsize);

  g->in = inter;
  // Spawns player at 0,0
  g->p = pl_createPlayer((ivec2_t){0, 0});

  // init Camera2D
  g->cam = (Camera2D *)malloc(sizeof(Camera2D));
  *(g->cam) = (Camera2D){.offset = (Vector2){WIN_W / 2.0f, WIN_H / 2.0f},
                         .rotation = 0.0f,
                         .zoom = 1.0f,
                         .target = ivec2ToVector2(g->p->e->pos)};

  // init entity linked list
  g->en_list = li_emptyList();
  g->active_entities = li_emptyList();
  g->uiWindowList = li_emptyList();

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

  /* int offsetY, offsetX; */
  /* offsetY = g->in->h / 2; */
  /* offsetX = g->in->w / 2; */

  // handle map

  // TODO

  // handle entities
  // TODO: refactor to use active_entities list
  node_t *current = g->en_list->head;
  while (current != NULL) {
    entity_t *curr_val = (entity_t *)current->value;
    if (curr_val != NULL && gm_entityOnGrid(curr_val, g)) {
      /* ivec2_t pos = */
      /*     (ivec2_t){(int16_t)((curr_val->pos.x) - g->p->e->pos.x + offsetX),
       */
      /*               (int16_t)((curr_val->pos.y) - g->p->e->pos.y + offsetY)};
       */
      // TODO: add UTF8 support HERE
      in_drawAtColored(g->in, (char)curr_val->c, curr_val->color,
                       curr_val->pos);
    }
    current = current->next;
  }
}
