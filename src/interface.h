#ifndef INTERFACE_H_
#define INTERFACE_H_

#define WIN_W 1920
#define WIN_H 1080

#include "entity.h"
#include "list.h"
#include "ui.h"
#include "utils.h"
#include "vecMath.h"
#include "colors.h"
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

	Camera2D uiCamera;
	list_t* uiWindowList;

} interface_t;

interface_t* in_create(uint8_t gridWidth, uint8_t gridHeight, float textSize);
void in_destroy(interface_t* interface);

void in_drawAt(interface_t* interface, char character, ivec2_t position);
void in_drawAtColored(interface_t* interface, char character, Color color,
                      ivec2_t position);

void in_drawArrayColored(interface_t* interface, char** characters,
                         Color** colors, ivec2_t startingPosition, int width,
                         int height);

void in_drawEntity(interface_t* interface, entity_t* entity);
// returns Index
int in_addUiWindow(ui_win_t* ui_window, interface_t* interface);
void in_destroyTopUiWindow(interface_t* interface);
// more recently added -> lower index
ui_win_t* in_getUiWindowAtIndex(interface_t* interface, int index);
void in_removeUiWindowAtIndex(interface_t* interface, int index);
// maybe add support to draw codepoints ?

// needs to be called AFTER draw calls
void in_drawGrid(interface_t* interface, Camera2D* camera);
void in_drawUi(interface_t* interface);
void in_clearScreen(interface_t* interface);

#endif // INTERFACE_H_
