#ifndef GAME_H_
#define GAME_H_

#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "entity.h"
#include "interface.h"
#include "list.h"
#include "map.h"
#include "player.h"
#include "ui.h"
#include "utils.h"
#include "vecMath.h"

typedef struct game {
	bool isRunning;
	bool isSimulating;
	interface_t* interface;
	player_t* player;

	list_t* entity_list;
	list_t* active_entities;
	list_t* eventHooks;

	map_t* map;

	Camera2D* mainCamera;

	float deltaTime;
	int fps;
} game_t;

typedef void (*gm_event_func)(game_t*);

void gm_start(game_t* game);
void gm_stop(game_t* game);

int gm_addEntity(entity_t* entity, game_t* game);
int gm_addEventHook(game_t* game, gm_event_func func);

void gm_removeEventHookAtIndex(game_t* game, int index);

game_t* gm_init(uint8_t gridWidth, uint8_t gridHeight, uint8_t textSize);

void gm_updateGrid(game_t* game);

#endif // GAME_H_
