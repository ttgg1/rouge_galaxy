#include "interface.h"
#include <raylib.h>

static int uiWindowCount = -1;

interface_t* in_create(uint8_t gridWidth, uint8_t gridHeight, float textSize)
{
	interface_t* res = malloc(sizeof(interface_t));

	res->width = gridWidth;
	res->height = gridHeight;
	res->textSize = textSize;

	res->uiCamera = (Camera2D) {
		.offset = (Vector2) {0.0f, 0.0f},
		.rotation = 0.0f,
		.zoom = 1.0f,
		.target = (Vector2) {0.0f, 0.0f}
	};
	res->uiWindowList = li_emptyList();

	// create window
	InitWindow(WIN_W, WIN_H, "Rouge Galaxy");
	SetTargetFPS(240);

	// Open Font
	char* relPath;
#define __LINUX__
#ifdef __LINUX__
	relPath = "/../../assets/256font.ttf";
#else
	relPath = "\\..\\..\\assets\\256font.ttf";
#endif

	int pathLenght = cwdPathLenght(relPath);
	char path[pathLenght];
	cwdJoinPath(relPath, path);

	// load all codepoints of font
	int codepoints[256];

	for (int i = 0; i < 256; ++i) {
		codepoints[i] = i;
	}

	// LoadFontEx can automatically load codepoints for utf8 support
	res->font = LoadFontEx(path, (int)res->textSize, codepoints, 255);

	// get grid_cell
	res->gridCellSize =
	        (uint8_t)ceil(0.5f * (WIN_W / res->width + WIN_H / res->height));

	// init Grid
	res->grid = (int**)malloc(res->height * sizeof(int*));

	// init colormap
	res->colormap = (Color**)malloc(res->height * sizeof(Color*));

	for (int j = 0; j < res->height; ++j) {
		// init cols
		res->grid[j] = (int*)malloc(res->width * sizeof(int));
		res->colormap[j] = (Color*)malloc(res->width * sizeof(Color));
	}

	return res;
}

void in_destroy(interface_t* interface)
{
	for (int j = 0; j < interface->height; ++j) {
		free(interface->grid[j]);
		free(interface->colormap[j]);
	}

	free(interface->grid);
	free(interface->colormap);

	free(interface);
	UnloadFont(interface->font);
	CloseWindow();
}

void in_drawAt(interface_t* interface, char character, ivec2_t position)
{
	interface->grid[position.y][position.x] = (int)character;
}

void in_drawAtColored(interface_t* interface, char character, Color color,
                      ivec2_t position)
{
	if (position.x > interface->width || position.y > interface->height ||
	        position.x < 0 || position.y < 0) {
		debug_print("Tried to draw character out of bounds !\n");
		return;
	}

	if (interface->grid == NULL || interface->colormap == NULL) {
		debug_print(
		        "Tried to write to an unititialized Grid array or colormap !\n");
		return;
	} else {
		interface->grid[position.y][position.x] = (int)character;
		interface->colormap[position.y][position.x] = color;
	}
}

void in_drawArrayColored(interface_t* interface, char** characters,
                         Color** colors, ivec2_t startingPosition, int width,
                         int height)
{
	if (width > interface->width || height > interface->height) {
		debug_print("drawArray to big to fit !!\n");
		return;
	}

	ivec2_t pos = startingPosition;

	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			in_drawAtColored(interface, characters[j][i], colors[j][i], pos);
			++pos.x;

			if (pos.x > width) {
				++pos.y;
				pos.x = 0;

				if (pos.y > interface->height) {
					debug_print("Array item Pos out of bounds !!\n");
					return;
				}
			}
		}
	}
}

int in_addUiWindow(ui_win_t* ui_window, interface_t* interface)
{
	li_push(interface->uiWindowList, ui_window);
	++uiWindowCount;
	return uiWindowCount;
}

void in_drawEntity(interface_t* interface, entity_t* entity)
{
	in_drawAtColored(interface, entity->c, entity->color, entity->pos);
}

static void drawUiWindow(ui_win_t* win, interface_t* in)
{
	if (win->isShown) {
		DrawTexture(*win->WindowTexture, win->pos.x, win->pos.y, WHITE);
	}
}
void in_drawUi(interface_t* interface)
{
	node_t* curr = interface->uiWindowList->head;

	BeginMode2D(interface->uiCamera);

	while (curr != NULL) {
		ui_win_t* curr_val = (ui_win_t*)curr->value;

		// set camera to target  ui text
		interface->uiCamera.offset = curr_val->pos;

		drawUiWindow(curr_val, interface);

		curr = curr->next;
	}

	EndMode2D();
}

void in_drawGrid(interface_t* interface, Camera2D* camera)
{
	Vector2 pos = {0, 0};

	for (int j = 0; j < interface->height; ++j) {
		for (int i = 0; i < interface->width; ++i) {
			DrawTextCodepoint(interface->font, interface->grid[j][i], pos,
			                  interface->textSize, interface->colormap[j][i]);
			pos.x += interface->gridCellSize;
		}

		pos.y += interface->gridCellSize;
		pos.x = 0;
	}
}

void in_clearScreen(interface_t* interface)
{
	for (int j = 0; j < interface->height; ++j) {
		for (int i = 0; i < interface->width; ++i) {
			interface->colormap[j][i] = in_fg;
			interface->grid[j][i] = (int)'.';
		}
	}

	ClearBackground(in_bg);
}

void in_destroyTopUiWindow(interface_t* interface)
{
	// because only push is used it functions like a stack
	ui_win_t* last = li_pop(interface->uiWindowList);
	ui_destroyWindow(last);
	--uiWindowCount;
}

ui_win_t* in_getUiWindowAtIndex(interface_t* interface, int index)
{
	return (ui_win_t*)li_getAtIndex(interface->uiWindowList, index);
}

void in_destroyUiWindowAtIndex(interface_t* interface, int index)
{
	li_removeIndex(interface->uiWindowList, index);
	--uiWindowCount;
}
