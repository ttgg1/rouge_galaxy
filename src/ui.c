#include "ui.h"

void drawBorders(ui_win_t *win, ui_win_border_t borderStyle,
                 Color borderColor) {
  // set Borders
  char borderChar = 0;

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

  if (borderChar > 1) {
    // set top and bottom
    for (int i = 0; i < win->width; ++i) {
      win->content[0][i] = (char)borderChar;
      win->content[win->height - 1][i] = (char)borderChar;

      win->colormap[0][i] = borderColor;
      win->colormap[win->height - 1][i] = borderColor;
    }

    // set sides
    for (int i = 0; i < win->height; ++i) {
      win->content[i][0] = (char)borderChar;
      win->content[i][win->width - 1] = (char)borderChar;

      win->colormap[i][0] = borderColor;
      win->colormap[i][win->width - 1] = borderColor;
    }
  } else { // handle constructed borders
    // set Colormap
    // set top and bottom
    for (int i = 0; i < win->width; ++i) {
      win->colormap[0][i] = borderColor;
      win->colormap[win->height - 1][i] = borderColor;
    }

    // set sides
    for (int i = 0; i < win->height; ++i) {
      win->colormap[i][0] = borderColor;
      win->colormap[i][win->width - 1] = borderColor;
    }

    // set top and bottom
    for (int i = 1; i < win->width - 1; ++i) {
      // sets corresponding char -> borderStyle ? BORDER_LINE : BORDER_PIPE
      win->content[0][i] = borderStyle ? (char)196 : (char)205;
      win->content[win->height - 1][i] = borderStyle ? (char)196 : (char)205;
    }

    // set sides
    for (int i = 1; i < win->height - 1; ++i) {
      win->content[i][0] = borderStyle ? (char)179 : (char)186;
      win->content[i][win->width - 1] = borderStyle ? (char)179 : (char)186;
    }

    // set corners
    // top-left
    win->content[0][0] = borderStyle ? (char)218 : (char)201;
    // top-right
    win->content[0][win->width] = borderStyle ? (char)191 : (char)187;
    // bottom-left
    win->content[win->height - 1][0] = borderStyle ? (char)192 : (char)200;
    // bottom-right
    win->content[win->height - 1][win->width - 1] =
        borderStyle ? (char)217 : (char)188;
  }
  ut_print2dArray(win->content, win->width, win->height);
}

void writeTextColored(ui_win_t *win, char *text, Color textColor) {
  const int text_len = (int)strlen(text);

  // check if text fits in window
  if (text_len >
      (win->width - 1 - INNER_PADDING) * (win->height - 1 - INNER_PADDING)) {
    debug_print("Text doesnt fit inside ui-window !\n");
    return;
  }

  // draw text wrapped
  int curr_y = 1 + INNER_PADDING;
  int curr_x = 1 + INNER_PADDING;

  for (int i = 0; i < text_len; ++i) {
    win->content[curr_y][curr_x] = (int)text[i];
    win->colormap[curr_y][curr_x] = textColor;

    // line wrap
    if (curr_x - 1 == win->width) {
      ++curr_y;
      curr_x = 1 + INNER_PADDING;
    } else {
      ++curr_x;
    }
  }
}

void writeText(ui_win_t *win, char *text) {
  const int text_len = (int)strlen(text);

  // check if text fits in window
  if (text_len >
      (win->width - 1 - INNER_PADDING) * (win->height - 1 - INNER_PADDING)) {
    debug_print("Text doesnt fit inside ui-window !\n");
    return;
  }

  // draw text wrapped
  int curr_y = 1 + INNER_PADDING;
  int curr_x = 1 + INNER_PADDING;

  for (int i = 0; i < text_len; ++i) {
    win->content[curr_y][curr_x] = (int)text[i];

    // line wrap
    if (curr_x - 1 == win->width) {
      ++curr_y;
      curr_x = 1 + INNER_PADDING;
    } else {
      ++curr_x;
    }
  }
}

ui_win_t *ui_createWindow(ivec2_t pos, uint16_t width, uint16_t height,
                          char *text, ui_win_border_t borderStyle,
                          Color borderColor, Color textColor) {
  ui_win_t *win = (ui_win_t *)malloc(sizeof(ui_win_t));

  win->width = width;
  win->height = height;
  win->pos = pos;

  win->content = (char **)create2dArray(
      win->width, win->height, char); //(int **)calloc(nobjs, sizeof(int));
  win->colormap =
      (Color **)create2dArray(win->width, win->height,
                              Color); //(Color **)malloc(nobjs * sizeof(Color));

  // set Colormap to backgroundcolor
  for (int j = 0; j < win->height; ++j) {
    for (int i = 0; i < win->width; ++i) {
      win->colormap[j][i] = in_bg;
    }
  }

  drawBorders(win, borderStyle, borderColor);

  writeTextColored(win, text, textColor);

  return win;
}

void ui_drawWindow(ui_win_t *win, interface_t *in) {
  // TODO: Remove (char**) cast when using codepoints !
  in_drawArrayColored(in, (char **)win->content, win->colormap, win->pos,
                      win->width, win->height);
}

void ui_clearWindowText(ui_win_t *win) {
  // set Color of entire text
  int curr_y = 1 + INNER_PADDING;
  int curr_x = 1 + INNER_PADDING;

  int max_index =
      (win->width - 1 - INNER_PADDING) * (win->height - 1 - INNER_PADDING);

  for (int i = 0; i < max_index; ++i) {
    win->content[curr_y][curr_x] = (int)' ';

    // line wrap
    if (curr_x - 1 == win->width) {
      ++curr_y;
      curr_x = 1 + INNER_PADDING;
    } else {
      ++curr_x;
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

void ui_updateWindowTextColor(ui_win_t *win, Color textColor) {
  // set Color of entire text
  int curr_y = 1 + INNER_PADDING;
  int curr_x = 1 + INNER_PADDING;

  int max_index =
      (win->width - 1 - INNER_PADDING) * (win->height - 1 - INNER_PADDING);

  for (int i = 0; i < max_index; ++i) {
    win->colormap[curr_y][curr_x] = textColor;

    // line wrap
    if (curr_x - 1 == win->width) {
      ++curr_y;
      curr_x = 1 + INNER_PADDING;
    } else {
      ++curr_x;
    }
  }
}

void ui_updateWindowTextAndColor(ui_win_t *win, char *text, Color textColor) {
  ui_clearWindowText(win);
  ui_clearWindowTextColor(win);
  writeTextColored(win, text, textColor);
}

void ui_destroyWindow(ui_win_t *win) {
  free2dArray(win->height, win->content);
  free2dArray(win->height, win->colormap);
  free(win);
}
