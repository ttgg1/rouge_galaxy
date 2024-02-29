#include "interface.h"
#include <raylib.h>

interface_t *in_create(uint8_t gridWidth, uint8_t gridHeight, float textSize) {
  interface_t *res = malloc(sizeof(interface_t));

  res->width = gridWidth;
  res->height = gridHeight;
  res->textSize = textSize;

  // create window
  InitWindow(WIN_W, WIN_H, "Rouge Galaxy");
  SetTargetFPS(240);

  // Open Font
  char *relPath;
#define __LINUX__
#ifdef __LINUX__
  relPath = "/../../assets/unifont-15.1.04.otf";
#else
  relPath = "\\..\\..\\assets\\unifont-15.1.04.otf";
#endif

  int pathLenght = cwdPathLenght(relPath);
  char path[pathLenght];
  cwdJoinPath(relPath, path);
  // LoadFontEx can automatically load codepoints for utf8 support
  res->font = LoadFont(path);

  // get grid_cell
  res->gridCellSize =
      (uint8_t)ceil(0.5f * (WIN_W / res->width + WIN_H / res->height));

  // init Grid
  res->grid = (int **)malloc(res->height * sizeof(int *));

  // init colormap
  res->colormap = (Color **)malloc(res->height * sizeof(Color *));

  for (int j = 0; j < res->height; ++j) {
    // init cols
    res->grid[j] = (int *)malloc(res->width * sizeof(int));
    res->colormap[j] = (Color *)malloc(res->width * sizeof(Color));
  }

  return res;
}

void in_destroy(interface_t *interface) {
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

void in_drawAt(interface_t *interface, char character, ivec2_t position) {
  interface->grid[position.y][position.x] = (int)character;
}

void in_drawAtColored(interface_t *interface, char character, Color color,
                      ivec2_t position) {
  if (position.x > interface->width || position.y > interface->height) {
    debug_print("Tried to draw character out of bounds !\n");
  }

  if (interface->grid == NULL || interface->colormap == NULL) {
    debug_print(
        "Tried to write to an unititialized Grid array or colormap !\n");
  } else {
    interface->grid[position.y][position.x] = (int)character;
    interface->colormap[position.y][position.x] = color;
  }
}

void in_drawArrayColored(interface_t *interface, char **characters,
                         Color **colors, ivec2_t startingPosition, int width,
                         int height) {
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

void in_drawEntity(interface_t *interface, entity_t *entity) {
  in_drawAtColored(interface, entity->c, entity->color, entity->pos);
}

void in_drawPresent(interface_t *interface, Camera2D *camera) {
  BeginDrawing();
  BeginMode2D(*camera);
  //---------------------------------------------------------------
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

  //---------------------------------------------------------------
  EndMode2D();
  EndDrawing();
}

void in_clearScreen(interface_t *interface) {
  for (int j = 0; j < interface->height; ++j) {
    for (int i = 0; i < interface->width; ++i) {
      interface->colormap[j][i] = in_fg;
      interface->grid[j][i] = (int)'.';
    }
  }

  ClearBackground(in_bg);
}
