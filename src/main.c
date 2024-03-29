#include <raylib.h>
#include <stdio.h>

#include "entity.h"
#include "game.h"
#include "interface.h"
#include "ui.h"
#include "vecMath.h"
#include "map.h"

int main(int argc, char** argv)
{
	game_t* game = gm_init(30, 30, 30);

	entity_t* entity1 =
	en_create(1, (ivec2_t) {10, 10}, (int)'E', (Color) {255, 0, 0, 255});
	entity_t* entity2 =
	en_create(2, (ivec2_t) {11, 10}, (int)'B', (Color) {128, 128, 0, 255});

	gm_addEntity(entity1, game);
	gm_addEntity(entity2, game);
	gm_start(game);

	gm_stop(game);
	return 0;
}
