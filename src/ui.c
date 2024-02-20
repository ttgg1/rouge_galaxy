#include "ui.h"
#include "interface.h"

void drawBorders(ui_win_t *win, ui_win_border_t borderStyle,
                 SDL_Color borderColor) {
  // set Borders
  uint32_t borderChar = 0;

  switch (borderStyle) {
  case BORDER_SOLID:
    borderChar = (uint32_t)BORDER_SOLID;
    break;

  case BORDER_LIGHT_DOTTED:
    borderChar = (uint32_t)BORDER_LIGHT_DOTTED;
    break;

  case BORDER_MEDIUM_DOTTED:
    borderChar = (uint32_t)BORDER_MEDIUM_DOTTED;
    break;

  case BORDER_BOLD_DOTTED:
    borderChar = (uint32_t)BORDER_BOLD_DOTTED;
    break;

  case BORDER_PIPE:
  case BORDER_LINE:
    break;

  case BORDER_NONE:
  default:
    borderChar = (uint32_t)BORDER_NONE;
    break;
  }

  // set Colormap
  // set top and bottom
  for (int i = 0; i < win->w; ++i) {
    win->colormap[i] = borderColor;
    win->colormap[(win->h * win->w - 1) - i] = borderColor;
  }
  // set sides
  for (int i = 0; i < win->h; ++i) {
    win->colormap[i * win->h] = borderColor;
    win->colormap[i * win->h + win->w] = borderColor;
  }

  if (borderChar != 0) {
    // set top and bottom
    for (int i = 0; i < win->w; ++i) {
      win->content[i] = borderChar;
      win->content[(win->h * win->w - 1) - i] = borderChar;
    }
    // set sides
    for (int i = 0; i < win->h; ++i) {
      win->content[i * win->h] = borderChar;
      win->content[i * win->h + win->w] = borderChar;
    }
  } else { // handle constructed borders
    // set top and bottom
    for (int i = 1; i < win->w - 1; ++i) {
      // sets corresponding char -> borderStyle ? BORDER_LINE : BORDER_PIPE
      win->content[i] = borderStyle ? (uint32_t)196 : (uint32_t)205;
      win->content[(win->h * win->w - 1) - i] =
          borderStyle ? (uint32_t)196 : (uint32_t)205;
    }
    // set sides
    for (int i = 1; i < win->h - 1; ++i) {
      win->content[i * win->h] = borderStyle ? (uint32_t)179 : (uint32_t)186;
      win->content[i * win->h + win->w] =
          borderStyle ? (uint32_t)179 : (uint32_t)186;
    }
    // set corners
    // top-left
    win->content[0] = borderStyle ? (uint32_t)218 : (uint32_t)201;
    // top-right
    win->content[win->w] = borderStyle ? (uint32_t)191 : (uint32_t)187;
    // bottom-left
    win->content[win->w * win->h - 1 - win->w] =
        borderStyle ? (uint32_t)192 : (uint32_t)200;
    // bottom-right
    win->content[win->w * win->h - 1] =
        borderStyle ? (uint32_t)217 : (uint32_t)188;
  }
}

void writeTextColored(ui_win_t *win, char *text, SDL_Color textColor) {
  const int text_len = (int)strlen(text);
  // check if text fits in window
  if (text_len > (win->w - 1 - INNER_PADDING) * (win->h - 1 - INNER_PADDING)) {
    debug_print("Text doesnt fit inside ui-window !\n");
    return;
  }

  // draw text wrapped
  int curr_Index = 1 + INNER_PADDING;
  for (int i = 0; i < text_len; ++i) {
    win->content[curr_Index] = (uint32_t)text[i];
    win->colormap[curr_Index] = textColor;

    // line wrap
    if (i % win->w == 0) {
      curr_Index += 1 + 2 * INNER_PADDING;
    } else {
      ++curr_Index;
    }
  }
}

void writeText(ui_win_t *win, char *text) {
  const int text_len = (int)strlen(text);
  // check if text fits in window
  if (text_len > (win->w - 1 - INNER_PADDING) * (win->h - 1 - INNER_PADDING)) {
    debug_print("Text doesnt fit inside ui-window !\n");
    return;
  }

  // draw text wrapped
  int curr_Index = 1 + INNER_PADDING;
  for (int i = 0; i < text_len; ++i) {
    win->content[curr_Index] = (uint32_t)text[i];

    // line wrap
    if (i % win->w == 0) {
      curr_Index += 1 + 2 * INNER_PADDING;
    } else {
      ++curr_Index;
    }
  }
}

ui_win_t *ui_createWindow(ivec2_t pos, uint16_t width, uint16_t height,
                          char *text, ui_win_border_t borderStyle,
                          SDL_Color borderColor, SDL_Color textColor) {
  ui_win_t *win = (ui_win_t *)malloc(sizeof(ui_win_t));

  win->w = width;
  win->h = height;
  win->pos = pos;

  size_t nobjs = win->w * win->h;

  win->content = (uint32_t *)calloc(nobjs, sizeof(uint32_t));
  win->colormap = (SDL_Color *)malloc(nobjs * sizeof(SDL_Color));

  // set Colormap to backgroundcolor
  for (int i = 0; i < (int)nobjs; ++i) {
    win->colormap[i] = in_bg;
  }

  drawBorders(win, borderStyle, borderColor);

  writeTextColored(win, text, textColor);

  return win;
}

void ui_drawWindow(ui_win_t *win, interface_t *in) {
  in_drawArrayColored(in, win->content, win->colormap, win->pos,
                      win->w * win->h, win->w);
}

void ui_clearWindowText(ui_win_t *win) {
  int max_index = win->w * win->h - win->h - 1; // last line can be ignored
  for (int i = 1 + INNER_PADDING; i < max_index; ++i) {
    win->content[i] = ' ';

    // line wrap
    if (i % win->w == 0) {
      i += 2 * INNER_PADDING;
    }
  }
}

void ui_clearWindowTextColor(ui_win_t *win) {
  ui_updateWindowTextColor(win, in_fg);
}
void ui_updateWindowText(ui_win_t *win, char *text) {
  ui_clearWindowText(win);
  writeText(win, text);
}

void ui_updateWindowTextColor(ui_win_t *win, SDL_Color textColor) {
  int max_index = win->w * win->h - win->h - 1; // last line can be ignored
  for (int i = 1 + INNER_PADDING; i < max_index; ++i) {
    win->colormap[i] = textColor;

    // line wrap
    if (i % win->w == 0) {
      i += 2 * INNER_PADDING;
    }
  }
}

void ui_updateWindowTextAndColor(ui_win_t *win, char *text,
                                 SDL_Color textColor) {
  ui_clearWindowText(win);
  ui_clearWindowTextColor(win);
  writeTextColored(win, text, textColor);
}

void ui_destroyWindow(ui_win_t *win) {
  free(win->content);
  free(win);
}
