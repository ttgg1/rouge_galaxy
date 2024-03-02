#include "ui.h"

static void drawBorders(ui_win_t *win, ui_win_border_t borderStyle) {
  // set Borders
  int borderChar = 0;

  switch (borderStyle) {
  case BORDER_SOLID:
    borderChar = (int)BORDER_SOLID;
    break;

  case BORDER_LIGHT_DOTTED:
    borderChar = (int)BORDER_LIGHT_DOTTED;
    break;

  case BORDER_MEDIUM_DOTTED:
    borderChar = (int)BORDER_MEDIUM_DOTTED;
    break;

  case BORDER_BOLD_DOTTED:
    borderChar = (int)BORDER_BOLD_DOTTED;
    break;

  case BORDER_PIPE:
  case BORDER_LINE:
    break;

  case BORDER_NONE:
  default:
    borderChar = (int)BORDER_NONE;
    break;
  }

  if (borderChar > 1) {
    // set top and bottom
    for (int i = 0; i < win->width; ++i) {
      win->windowBorder[0][i] = (int)borderChar;
      win->windowBorder[win->height - 1][i] = (int)borderChar;
    }

    // set sides
    for (int i = 0; i < win->height; ++i) {
      win->windowBorder[i][0] = (int)borderChar;
      win->windowBorder[i][win->width - 1] = (int)borderChar;
    }
  } else { // handle constructed borders
    // set top and bottom
    for (int i = 1; i < win->width - 1; ++i) {
      // sets corresponding char -> borderStyle ? BORDER_LINE : BORDER_PIPE
      win->windowBorder[0][i] = borderStyle ? (int)196 : (int)205;
      win->windowBorder[win->height - 1][i] = borderStyle ? (int)196 : (int)205;
    }

    // set sides
    for (int i = 1; i < win->height - 1; ++i) {
      win->windowBorder[i][0] = borderStyle ? (int)179 : (int)186;
      win->windowBorder[i][win->width - 1] = borderStyle ? (int)179 : (int)186;
    }

    // set corners
    // top-left
    win->windowBorder[0][0] = borderStyle ? (int)218 : (int)201;
    // top-right
    win->windowBorder[0][win->width - 1] = borderStyle ? (int)191 : (int)187;
    // bottom-left
    win->windowBorder[win->height - 1][0] = borderStyle ? (int)192 : (int)200;
    // bottom-right
    win->windowBorder[win->height - 1][win->width - 1] =
        borderStyle ? (int)217 : (int)188;
  }
}

static void writeText(ui_win_t *win, char *text) {
  const int text_len = (int)strlen(text);

  // check if text fits in window
  if (text_len >
      (win->width + 1 + INNER_PADDING) * (win->height + 1 + INNER_PADDING)) {
    debug_print("Text doesnt fit inside ui-window !\n");
    return;
  }

  // draw text wrapped
  int curr_y = INNER_PADDING;
  int curr_x = INNER_PADDING;

  for (int i = 0; i < text_len; ++i) {
    if (text[i] == '\n') {
      ++curr_y;
      curr_x = INNER_PADDING;
      ++i;
      continue;
    }

    win->text[curr_y][curr_x] = text[i];

    // line wrap
    if ((curr_x + INNER_PADDING + 1 > win->width && text[i] == ' ') ||
        curr_x > win->width - 2) {
      ++curr_y;
      curr_x = INNER_PADDING;
    } else {
      ++curr_x;
    }
  }
}

ui_win_t *ui_createWindow(ivec2_t pos, uint16_t width, uint16_t height,
                          char *text, ui_win_border_t borderStyle,
                          Color textColor, Color borderColor) {
  ui_win_t *win = (ui_win_t *)malloc(sizeof(ui_win_t));

  win->width = width;
  win->height = height;
  win->pos = pos;
  win->textColor = textColor;
  win->borderColor = borderColor;

  win->windowBorder = (int **)create2dArray((win->width + 1), win->height, int);
  win->text = (char **)create2dArray((win->width + 1), win->height, char);

  // set content
  for (int j = 0; j < win->height; ++j) {
    for (int i = 0; i < win->width; ++i) {
      win->windowBorder[j][i] = (int)' ';
      win->text[j][i] = (char)' ';
    }
    // terminate String
    win->windowBorder[j][win->width] = '\0';
    win->text[j][win->width] = '\0';
  }

  writeText(win, text);

  drawBorders(win, borderStyle);

  return win;
}

void ui_clearWindowText(ui_win_t *win) {
  // set Color of entire text
  int curr_y = 1 + INNER_PADDING;
  int curr_x = 1 + INNER_PADDING;

  int max_index =
      (win->width - 1 - INNER_PADDING) * (win->height - 1 - INNER_PADDING);

  for (int i = 0; i < max_index; ++i) {
    win->text[curr_y][curr_x] = (int)' ';

    // line wrap
    if (curr_x + 1 + INNER_PADDING == win->width) {
      ++curr_y;
      curr_x = 1 + INNER_PADDING;
    } else {
      ++curr_x;
    }
  }
}

void ui_updateWindowText(ui_win_t *win, char *text) {
  ui_clearWindowText(win);
  writeText(win, text);
}

void ui_updateWindowTextColor(ui_win_t *win, Color textColor) {
  win->textColor = textColor;
}

void ui_updateWindowTextAndColor(ui_win_t *win, char *text, Color textColor) {
  ui_clearWindowText(win);
  writeText(win, text);
  win->textColor = textColor;
}

void ui_destroyWindow(ui_win_t *win) {
  free2dArray(win->height, win->windowBorder);
  free(win);
}
