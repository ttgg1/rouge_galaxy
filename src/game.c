#include "game.h"

uint8_t num_entities = 0;

void checkPlayerBounds(game_t *game) {
  ivec2_t plPos = game->player->e->pos;
  int grid_w = game->interface->width;
  int grid_h = game->interface->height;

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

void updateCamera(game_t *game) {
  game->mainCamera->zoom += ((float)GetMouseWheelMove() * 0.05f);

  if (game->mainCamera->zoom > 3.0f) {
    game->mainCamera->zoom = 3.0f;
  } else if (game->mainCamera->zoom < 0.25f) {
    game->mainCamera->zoom = 0.25f;
  }

  static float minSpeed = 30;
  static float minEffectLength = 5;
  static float fractionSpeed = 0.8f;

  Vector2 plPos =
      ivec2ToScreenspace(game->player->e->pos, game->interface->gridCellSize);

  game->mainCamera->offset = (Vector2){WIN_W / 2.0f, WIN_H / 2.0f};
  Vector2 diff = Vector2Subtract(plPos, game->mainCamera->target);
  float length = Vector2Length(diff);

  if (length > minEffectLength) {
    float speed = fmaxf(fractionSpeed * length, minSpeed);
    game->mainCamera->target =
        Vector2Add(game->mainCamera->target,
                   Vector2Scale(diff, speed * game->deltaTime / length));
  }
}

void drawUi(game_t *game) {
  node_t *curr = game->uiWindowList->head;

  while (curr != NULL) {
    ui_win_t *curr_val = (ui_win_t *)curr->value;

    ui_drawWindow(curr_val, game->interface);

    curr = curr->next;
  }
}

void draw(game_t *game) {
  in_clearScreen(game->interface);
  updateCamera(game);
  // draw stuff
  gm_updateGrid(game);
  drawUi(game);

  in_drawPresent(game->interface, game->mainCamera);
}

void handleEvents(game_t *game) {
  // printf("delta: %f, FPS: %d\n", game->deltaTime, game->fps);
  pl_handleMovement(game->player, game->deltaTime);
  checkPlayerBounds(game);
}

void loop(game_t *game) {
  while (game->isRunning && !WindowShouldClose()) {
    game->deltaTime = GetFrameTime();
    game->fps = GetFPS();
    handleEvents(game);
    draw(game);
  }
}

void gm_start(game_t *game) {
  game->isRunning = true;
  loop(game);
  // stop if isRunning = false
  gm_stop(game);
}

void gm_stop(game_t *game) {
  game->isRunning = false;
  pl_destroyPlayer(game->player);
  in_destroy(game->interface);
  li_destroy(game->entity_list);
  li_destroy(game->active_entities);
  free(game->mainCamera);
  // TODO: traverse lists and delete all entities
  free(game);
}

void gm_addEntity(entity_t *entity, game_t *game) {
  li_push(game->entity_list, entity);
}

void gm_addUiWindow(ui_win_t *ui_window, game_t *game) {
  li_push(game->uiWindowList, ui_window);
}

/*
  odd grid_w and grid_h for centered player
*/
game_t *gm_init(uint8_t grid_w, uint8_t grid_h, uint8_t ptsize) {

  srand(time(NULL));

  game_t *g = malloc(sizeof(game_t));
  interface_t *inter;

  // create text interface and store it in game struct
  inter = in_create(gridWidth, gridHeight, textSize);

  g->interface = inter;
  // Spawns player at 0,0
  g->player = pl_createPlayer((ivec2_t){0, 0});

  // init Camera2D
  g->mainCamera = (Camera2D *)malloc(sizeof(Camera2D));
  *(g->mainCamera) = (Camera2D){.offset = (Vector2){WIN_W / 2.0f, WIN_H / 2.0f},
                                .rotation = 0.0f,
                                .zoom = 1.0f,
                                .target = ivec2ToVector2(g->player->e->pos)};

  // init entity linked list
  g->entity_list = li_emptyList();
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
  gm_addEntity(g->player->e, g);

  return g;
}

bool gm_entityOnGrid(entity_t *entity, game_t *game) {
  return entity->pos.x >= game->player->e->pos.x - game->interface->width / 2 &&
         entity->pos.x <= game->player->e->pos.x + game->interface->width / 2 &&
         entity->pos.y >=
             game->player->e->pos.y - game->interface->height / 2 &&
         entity->pos.y <= game->player->e->pos.y + game->interface->height / 2;
}

void gm_updateGrid(game_t *game) {
  // clear grid
  // memset(g->in->grid, '.', g->in->w * g->in->h * sizeof(char));

  /* int offsetY, offsetX; */
  /* offsetY = g->in->h / 2; */
  /* offsetX = g->in->w / 2; */

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
  node_t *current = game->entity_list->head;

  while (current != NULL) {
    entity_t *curr_val = (entity_t *)current->value;

    if (curr_val != NULL && gm_entityOnGrid(curr_val, game)) {
      /* ivec2_t pos = */
      /*     (ivec2_t){(int16_t)((curr_val->pos.x) - g->p->e->pos.x + offsetX),
       */
      /*               (int16_t)((curr_val->pos.y) - g->p->e->pos.y + offsetY)};
       */
      // TODO: add UTF8 support HERE
      in_drawAtColored(game->interface, (char)curr_val->c, curr_val->color,
                       curr_val->pos);
    }

    current = current->next;
  }
}
