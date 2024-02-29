#ifndef INTERFACE_H_
#define INTERFACE_H_

// foreground and background colors
#define in_bg                                                                  \
	(Color) { 20, 20, 20, 255 }
#define in_fg                                                                  \
	(Color) { 200, 200, 200, 255 }

#define WIN_W 1000
#define WIN_H 1000

#include "entity.h"
#include "utils.h"
#include "vecMath.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <raylib.h>

typedef struct interface {
	uint16_t width;
	uint16_t height;
	uint8_t gridCellSize;
	float textSize;
	int** grid; // uses raylib codepoints
	Color** colormap;

	Font font;
} interface_t;

interface_t* in_create(uint8_t gridWidth, uint8_t gridHeight, float textSize);
void in_destroy(interface_t* interface);

void in_drawAt(interface_t* interface, char character, ivec2_t position);
void in_drawAtColored(interface_t* interface, char character, Color color, ivec2_t position);

void in_drawArrayColored(interface_t* interface, char** characters, Color** colors,
                         ivec2_t startingPosition, int width, int height);

void in_drawEntity(interface_t* interface, entity_t* entity);

// maybe add support to draw codepoints ?

// needs to be called AFTER draw calls
void in_drawPresent(interface_t* interface, Camera2D* camera);
void in_clearScreen(interface_t* interface);

#endif // INTERFACE_H_
