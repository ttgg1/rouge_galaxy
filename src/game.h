#ifndef GAME_H_
#define GAME_H_

#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

#include "entity.h"
#include "interface.h"
#include "list.h"
#include "colors.h"
#include "map.h"
#include "player.h"
#include "ui.h"
#include "utils.h"
#include "vecMath.h"

// TODO: use Flags instead of bools

typedef struct game {
	bool isRunning;
	bool isSimulating;
	bool isInputCaptured;

	interface_t* interface;
	player_t* player;

	list_t* entity_list;
	list_t* active_entities;

	list_t* eventHooks;
	list_t* eventHooksData;

	list_t* alwaysEventHooks; // gets executed if isSimulating = false
	list_t* alwaysEventHooksData;

	list_t* notSimulatingEventHooks; // ONLY gets executed, when isSimulating = false
	list_t* notSimulatingEventHooksData;

	map_t* map;

	Camera2D* mainCamera;

	float deltaTime;
	int fps;
} game_t;

/* this struct contains an index of an event hook and its args. Arguments to eventHooks are optional, by default an eventhook gets the game struct,
 * which contains most of what you need */
typedef struct gm_event_func_data_struct {
	int index;
	void* dataStruct;
} gm_event_func_data_struct_t;

typedef void (*gm_event_func)(game_t*, ...);

/*ESSENTIALS******************************************************************/
game_t* gm_init(uint8_t gridWidth, uint8_t gridHeight, uint8_t textSize);
void gm_start(game_t* game);
void gm_stop(game_t* game);

/*ENTITIES**********************************************************************/
int gm_addEntity(entity_t* entity, game_t* game);
void gm_removeEntityAtIndex(game_t* game, int index);

/*EVENT HOOKS*****************************************************************/
int gm_addEventHook(game_t* game, gm_event_func func);
int gm_addAlwaysEventHook(game_t* game, gm_event_func func);
int gm_addNotSimulatingEventHook(game_t* game, gm_event_func func);

void gm_removeEventHookAtIndex(game_t* game, int index);
void gm_removeAlwaysEventHookAtIndex(game_t* game, int index);
void gm_removeNotSimulatingEventHookAtIndex(game_t* game, int index);

void gm_addEventHookData(game_t* game, gm_event_func_data_struct_t* data);
void gm_addAlwaysEventHookData(game_t* game, gm_event_func_data_struct_t* data);
void gm_addNotSimulatingEventHookData(game_t* game, gm_event_func_data_struct_t* data);

void gm_updateGrid(game_t* game);

#endif // GAME_H_
