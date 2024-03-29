#include "game.h"

// the index of the upper most
static int16_t num_entities = -1;
static int16_t num_eventHooks = -1;

// static internal prototypes

static bool entityOnGrid(entity_t* entity, game_t* game);
static void loop(game_t* game);
static void handleEvents(game_t* game);
static void draw(game_t* game);
static void updateCamera(game_t* game);
static void checkPlayerBounds(game_t* game);

static void characterCreation()
{
	// TODO
}

static void drawPlayerUi(game_t* game)
{
	static const int textSize = 25;
	static const int padding = 5;

	Vector2 pos = {padding, WIN_H - textSize - padding};

	// draw player info to buffer
	// Class: ... Health: ../.. Power: ../.. Ammo: ../..

	player_t* player = game->player;

	const char* classText;

	switch(game->player->playerClass) {
	case none:
		classText = "none";
		break;

	case soldier:
		classText = "Soldier";
		break;

	case scientist:
		classText = "Scientist";
		break;

	case engineer:
		classText = "Engineer";
		break;

	case spaceMarine:
		classText = "Space Marine";
		break;
	}

	const char* buffer = TextFormat("Class: %s  Health: %d/%d  Power: %d/%d  Ammo: %d/%d", classText, player->currentHealth, player->maxHealth,
	                                player->currentPower, player->maxPower, player->currentAmmo, player->maxAmmo);

	DrawRectangle(0, (int) pos.y - padding, WIN_W, textSize + 2 * padding, in_windowBackground);
	DrawTextEx(game->interface->font, buffer, pos, textSize, 1.0f, in_text);
}

static void handleGameKeys(game_t* game)
{
	if (IsKeyPressed(KEY_SPACE)) {
		game->isSimulating = !game->isSimulating;
	}
}

static void checkPlayerBounds(game_t* game)
{
	ivec2_t* plPos = &game->player->e->pos;
	int grid_w = game->interface->width;
	int grid_h = game->interface->height;

	if (plPos->x >= grid_w - 1) {
		plPos->x = (int16_t)grid_w - 1;
	}

	if (plPos->y >= grid_h) {
		plPos->y = (int16_t)grid_h - 1;
	}

	if (plPos->x < 0) {
		plPos->x = 0;
	}

	if (plPos->y < 0) {
		plPos->y = 0;
	}
}

static void updateCamera(game_t* game)
{
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

	game->mainCamera->offset = (Vector2) {
		WIN_W / 2.0f, WIN_H / 2.0f
	};
	Vector2 diff = Vector2Subtract(plPos, game->mainCamera->target);
	float length = Vector2Length(diff);

	if (length > minEffectLength) {
		float speed = fmaxf(fractionSpeed * length, minSpeed);
		game->mainCamera->target =
		        Vector2Add(game->mainCamera->target,
		                   Vector2Scale(diff, speed * game->deltaTime / length));
	}
}

static void draw(game_t* game)
{
	in_clearScreen(game->interface);

	BeginDrawing();
	BeginMode2D(*game->mainCamera);

	/* START DRAW ***************************************************************/

	// pre draw stuff
	updateCamera(game);
	gm_updateGrid(game);

	// interface drawing
	in_drawGrid(game->interface, game->mainCamera);
	in_drawUi(game->interface);

	drawPlayerUi(game);
	/* END DRAW ********************************************************************/

	EndMode2D();
	EndDrawing();
}

static void handleEvents(game_t* game)
{
	// printf("delta: %f, FPS: %d\n", game->deltaTime, game->fps);
	pl_handleMovement(game->player);
	checkPlayerBounds(game);

	// call eventhooks
	if(num_eventHooks > -1) {
		node_t* current = game->eventHooks->head;

		while (current != NULL) {
			gm_event_func currentFunction = (gm_event_func)current->value;
			currentFunction(game);

			current = current->next;
		}
	}
}

static void loop(game_t* game)
{
	while (game->isRunning && !WindowShouldClose()) {
		game->deltaTime = GetFrameTime();
		game->fps = GetFPS();

		// handles Keyboard Events in paused state
		handleGameKeys(game);

		if (game->isSimulating) {
			handleEvents(game);
		}

		draw(game);
	}
}

void gm_start(game_t* game)
{
	game->isRunning = true;
	game->isSimulating = true;
	loop(game);
	// stop if isRunning = false
	gm_stop(game);
}

void gm_stop(game_t* game)
{
	game->isRunning = false;
	pl_destroyPlayer(game->player);
	in_destroy(game->interface);

	CallFuncOnEveryItem(game->entity_list, en_destroy);

	li_destroy(game->entity_list);
	li_destroy(game->active_entities);
	free(game->mainCamera);
	// TODO: traverse lists and delete all entities
	free(game);
}

int gm_addEntity(entity_t* entity, game_t* game)
{
	li_push(game->entity_list, entity);
	++num_entities;
	return num_entities;
}

int gm_addEventHook(game_t* game, gm_event_func func)
{
	li_push(game->eventHooks, func);
	++num_eventHooks;
	return num_eventHooks;
}

void gm_removeEntityAtIndex(game_t* game, int index)
{
	if(index <= num_eventHooks) {
		li_removeIndex(game->entity_list, index);
		--num_entities;
	} else {
		debug_print("tried to remove Enitity out of bounds !\n");
	}
}

void gm_removeEventHookAtIndex(game_t* game, int index)
{
	if(index <= num_eventHooks) {
		li_removeIndex(game->eventHooks, index);
		--num_eventHooks;
	} else {
		debug_print("tried to remove Eventhook out of bounds !\n");
	}
}
/*
  odd grid_w and grid_h for centered player
*/
game_t* gm_init(uint8_t gridWidth, uint8_t gridHeight, uint8_t textSize)
{

	srand(time(NULL));

	game_t* g = malloc(sizeof(game_t));
	interface_t* inter;

	// create text interface and store it in game struct
	inter = in_create(gridWidth, gridHeight, textSize);

	g->interface = inter;
	// Spawns player at 0,0
	g->player = pl_createPlayer((ivec2_t) {
		0, 0
	});

	// init Camera2D
	g->mainCamera = (Camera2D*)malloc(sizeof(Camera2D));
	*(g->mainCamera) =
	(Camera2D) {
		.offset = (Vector2) {WIN_W / 2.0f, WIN_H / 2.0f},
		.rotation = 0.0f,
		.zoom = 1.0f,
		.target = ivec2ToScreenspace(g->player->e->pos,
		                             g->interface->gridCellSize)
	};

	// init entity linked list
	g->entity_list = li_emptyList();
	g->active_entities = li_emptyList();
	g->eventHooks = li_emptyList();

	// init map
	g->map = m_create();

	static const bool tempCons[NUM_CONSTRAINTS * 4 * NUM_CONSTRAINTS] = {
		true,  false, false, true,  true,  false, false, true,
		false, true,  true,  false, false, true,  true,  false
	};
	/*{
	  true,false,false,false,false, true,true,true,true,true,
	true,true,true,true,true, true,false,false,false,false,
	  true,true,false,false,false, false,true,true,true,true,
	false,true,true,true,true, true,true,false,false,false,
	  true,true,true,false,false, false,false,true,true,true,
	false,false,true,true,true, true,true,true,false,false,
	  true,true,true,true,false, false,false,false,true,true,
	false,false,false,true,true, true,true,true,true,false,
	  true,true,true,true,true, false,false,false,false,true,
	false,false,false,false,true, true,true,true,true,true
	};*/
	memcpy(g->map->constraints, tempCons, sizeof tempCons);

	static const uint32_t tempTiles[NUM_CONSTRAINTS] = {
		(uint32_t)'#',
		(uint32_t)'.'
	}; //{(uint32_t) '1', (uint32_t) '2', (uint32_t) '3',
	//(uint32_t) '4', (uint32_t) '5'};
	memcpy(g->map->tileset, tempTiles, sizeof tempTiles);

	m_setAt(g->map, 1200, 1200, (uint32_t)'T');
	m_generateMap(g->map, -1000, 1000, -1000, 1000);

	// add Player to Entity list
	gm_addEntity(g->player->e, g);

	return g;
}

static bool entityOnGrid(entity_t* entity, game_t* game)
{
	return entity->pos.x >= game->player->e->pos.x - game->interface->width / 2 &&
		       entity->pos.x <= game->player->e->pos.x + game->interface->width / 2 &&
		       entity->pos.y >=
		       game->player->e->pos.y - game->interface->height / 2 &&
		       entity->pos.y <= game->player->e->pos.y + game->interface->height / 2;
}

void gm_updateGrid(game_t* game)
{
	// clear grid
	// memset(g->in->grid, '.', g->in->w * g->in->h * sizeof(char));

	int offsetY, offsetX;
	offsetY = game->interface->height / 2;
	offsetX = game->interface->width / 2;

	// handle map

	for (int y = 0; y < game->interface->height; y++) {
		for (int x = 0; x < game->interface->width; x++) {
			in_drawAtColored(game->interface,
			                 m_getAt(game->map, y + game->player->e->pos.y - offsetY,
			                         x + game->player->e->pos.x - offsetX),
			(Color) {
				200, 200, 200, 255
			}, (ivec2_t) {
				x, y
			});
		}
	}

	// handle entities
	// TODO: refactor to use active_entities list

	node_t* current = game->entity_list->head;

	while (current != NULL) {
		entity_t* curr_val = (entity_t*)current->value;

		if (curr_val != NULL && entityOnGrid(curr_val, game)) {
			// TODO: add UTF8 support HERE
			in_drawAtColored(game->interface, (char)curr_val->c, curr_val->color,
			                 curr_val->pos);
		}

		current = current->next;
	}
}
