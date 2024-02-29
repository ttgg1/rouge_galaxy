#include "interface.h"
#include <raylib.h>

interface_t *in_create(uint8_t grid_w, uint8_t grid_h, float ptsize) {
    interface_t *res = malloc(sizeof(interface_t));

    res->w = grid_w;
    res->h = grid_h;
    res->ptsize = ptsize;

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
    res->f = LoadFont(path);

    // get grid_cell
    res->grid_cell = (uint8_t)ceil(0.5f * (WIN_W / res->w + WIN_H / res->h));

    // init Grid
    res->grid = (int **)malloc(res->h * sizeof(int *));

    // init colormap
    res->colormap = (Color **)malloc(res->h * sizeof(Color *));
    for (int j = 0; j < res->h; ++j) {
        // init cols
        res->grid[j] = (int *)malloc(res->w * sizeof(int));
        res->colormap[j] = (Color *)malloc(res->w * sizeof(Color));
    }

    return res;
}

void in_destroy(interface_t *in) {
    for (int j = 0; j < in->h; ++j) {
        free(in->grid[j]);
        free(in->colormap[j]);
    }
    free(in->grid);
    free(in->colormap);

    free(in);
    UnloadFont(in->f);
    CloseWindow();
}

void in_drawAt(interface_t *in, char c, ivec2_t pos) {
    in->grid[pos.x][pos.y] = (int)c;
}

void in_drawAtColored(interface_t *in, char c, Color color, ivec2_t pos) {
    if (pos.x > in->w || pos.y > in->h) {
        debug_print("Tried to draw character out of bounds !\n");
    }

    if (in->grid == NULL || in->colormap == NULL) {
        debug_print(
            "Tried to write to an unititialized Grid array or colormap !\n");
    } else {
        in->grid[pos.y][pos.x] = (int)c;
        in->colormap[pos.y][pos.x] = color;
    }
}

void in_drawArrayColored(interface_t *in, char **chars, Color **colors,
                         ivec2_t start_pos, int width, int height) {
    ivec2_t pos = start_pos;
    for (int j = 0; j < height; ++j) {
        for(int i = 0; i< width; ++i) {
            in_drawAtColored(in, chars[j][i], colors[j][i], pos);
            ++pos.x;
            if(pos.x > width) {
                ++pos.y;
                pos.x = 0;
                if (pos.y > in->h) {
                    debug_print("Array item Pos out of bounds !!\n");
                    return;
                }
            }
        }
    }
}

void in_drawEntity(interface_t *in, entity_t *e) {
    in_drawAtColored(in, e->c, e->color, e->pos);
}

void in_drawPresent(interface_t *in, Camera2D *cam) {
    BeginDrawing();
    BeginMode2D(*cam);
    //---------------------------------------------------------------
    Vector2 pos = {0, 0};
    for (int j = 0; j < in->h; ++j) {
        for (int i = 0; i < in->w; ++i) {
            DrawTextCodepoint(in->f, in->grid[j][i], pos, in->ptsize,
                              in->colormap[j][i]);
            pos.x += in->grid_cell;
        }
        pos.y += in->grid_cell;
        pos.x = 0;
    }
    //---------------------------------------------------------------
    EndMode2D();
    EndDrawing();
}

void in_clearScreen(interface_t *in) {
    for (int j = 0; j < in->h; ++j) {
        for (int i = 0; i < in->w; ++i) {
            in->colormap[j][i] = in_fg;
            in->grid[j][i] = (int)'.';
        }
    }
    ClearBackground(in_bg);
}
