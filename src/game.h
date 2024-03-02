#ifndef GAME_H_
#define GAME_H_

#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "entity.h"
#include "interface.h"
#include "list.h"
#include "player.h"
#include "ui.h"
#include "utils.h"
#include "map.h"
#include "vecMath.h"

typedef struct game {
	bool isRunning;
	interface_t* interface;
	player_t* player;
	list_t* entity_list;
	list_t* active_entities;
  map_t *map;
	list_t* uiWindowList;
	Camera2D* mainCamera;

	float deltaTime;
	int fps;
} game_t;

void gm_start(game_t* game);
void gm_stop(game_t* game);

void gm_addEntity(entity_t* entity, game_t* game);
void gm_addUiWindow(ui_win_t* ui_window, game_t* game);

game_t* gm_init(uint8_t gridWidth, uint8_t gridHeight, uint8_t textSize);

void gm_updateGrid(game_t* game);

bool gm_entityOnGrid(entity_t* entity, game_t* game);

#endif // GAME_H_
