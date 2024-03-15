#ifndef PLAYER_H_
#define PLAYER_H_

#include "entity.h"
#include "utils.h"
#include "vecMath.h"
#include <raylib.h>

typedef enum pl_class {
	none,
	soldier, // figher
	scientist, // mage
	engineer, // something like a druid ?
	spaceMarine, // space-fighter
} pl_class_t;

typedef struct player {
	entity_t* e;

	pl_class_t playerClass;

	// health
	int currentHealth;
	int maxHealth;

	// mana
	int currentPower;
	int maxPower;

	// ammo
	int currentAmmo;
	int maxAmmo;
} player_t;

player_t* pl_createPlayer(ivec2_t pos);
void pl_destroyPlayer(player_t* p);

// call after case KEYDOWN
void pl_handleMovement(player_t* p);

#endif // PLAYER_H_
