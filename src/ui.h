#ifndef UI_H_
#define UI_H_

#include "interface.h"
#include "utils.h"
#include "vecMath.h"
#include <stdint.h>
#include <string.h>

#define INNER_PADDING 1

typedef enum ui_win_border {
  BORDER_SOLID = 219,         // █
  BORDER_LIGHT_DOTTED = 176,  // ░
  BORDER_MEDIUM_DOTTED = 177, // ▒
  BORDER_BOLD_DOTTED = 178,   // ▓
  BORDER_NONE = 46,           // .
  BORDER_PIPE = 0,            // ║
  BORDER_LINE = 1             // │
} ui_win_border_t;

typedef struct ui_window {
  uint16_t w;
  uint16_t h;
  ivec2_t pos;
  uint32_t *content;
  SDL_Color *colormap;
} ui_win_t;

ui_win_t *ui_createWindow(ivec2_t pos, uint16_t width, uint16_t height,
                          char *text, ui_win_border_t borderStyle,
                          SDL_Color borderColor, SDL_Color textColor);

void ui_drawWindow(ui_win_t *win, interface_t *in);

void ui_destroyWindow(ui_win_t *win);

#endif // UI_H_
