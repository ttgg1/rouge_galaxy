#include "player.h"
#include "entity.h"

player_t* pl_createPlayer(ivec2_t pos)
{
	player_t* res = malloc(sizeof(player_t));

	if (res == NULL) {
		debug_print("Player allocation failed !\n");
	}

	res->e =
	en_create(0, pos, 'P', (Color) {.r = 10, .g = 128, .b = 128, .a = 255});

	return res;
}

void pl_destroyPlayer(player_t* p)
{
	// cleanup player
	en_destroy(p->e);
	// free player struct
	free(p);
}

void pl_handleMovement(player_t* p, float delta)
{
	if (IsKeyPressed(KEY_UP)) {
		en_move(p->e, 0, -1);
	}

	if (IsKeyPressed(KEY_DOWN)) {
		en_move(p->e, 0, 1);
	}

	if (IsKeyPressed(KEY_LEFT)) {
		en_move(p->e, -1, 0);
	}

	if (IsKeyPressed(KEY_RIGHT)) {
		en_move(p->e, 1, 0);
	}
}
