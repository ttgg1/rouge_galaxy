#include "ui.h"

void drawBorders(ui_win_t *win, ui_win_border_t borderStyle,
                 Color borderColor) {
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

    if (borderChar != 0) {
        // set top and bottom
        for (int i = 0; i < win->w; ++i) {
            win->content[0][i] = (int)borderChar;
            win->content[win->h - 1][i] = (int)borderChar;

            win->colormap[0][i] = borderColor;
            win->colormap[win->h - 1][i] = borderColor;
        }
        // set sides
        for (int i = 0; i < win->h; ++i) {
            win->content[i][0] = (int)borderChar;
            win->content[i][win->w - 1] = (int)borderChar;

            win->colormap[i][0] = borderColor;
            win->colormap[i][win->w - 1] = borderColor;
        }
    } else { // handle constructed borders
        // set Colormap
        // set top and bottom
        for (int i = 0; i < win->w; ++i) {
            win->colormap[0][i] = borderColor;
            win->colormap[win->h - 1][i] = borderColor;
        }
        // set sides
        for (int i = 0; i < win->h; ++i) {
            win->colormap[i][0] = borderColor;
            win->colormap[i][win->w - 1] = borderColor;
        }
        // set top and bottom
        for (int i = 1; i < win->w - 1; ++i) {
            // sets corresponding char -> borderStyle ? BORDER_LINE : BORDER_PIPE
            win->content[0][i] = borderStyle ? (int)196 : (int)205;
            win->content[win->h - 1][i] = borderStyle ? (int)196 : (int)205;
        }
        // set sides
        for (int i = 1; i < win->h - 1; ++i) {
            win->content[i][0] = borderStyle ? (int)179 : (int)186;
            win->content[i][win->w - 1] = borderStyle ? (int)179 : (int)186;
        }
        // set corners
        // top-left
        win->content[0][0] = borderStyle ? (int)218 : (int)201;
        // top-right
        win->content[0][win->w] = borderStyle ? (int)191 : (int)187;
        // bottom-left
        win->content[win->h - 1][0] = borderStyle ? (int)192 : (int)200;
        // bottom-right
        win->content[win->h - 1][win->w - 1] = borderStyle ? (int)217 : (int)188;
    }
}

void writeTextColored(ui_win_t *win, char *text, Color textColor) {
    const int text_len = (int)strlen(text);
    // check if text fits in window
    if (text_len > (win->w - 1 - INNER_PADDING) * (win->h - 1 - INNER_PADDING)) {
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
        if (curr_x - 1 == win->w) {
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
    if (text_len > (win->w - 1 - INNER_PADDING) * (win->h - 1 - INNER_PADDING)) {
        debug_print("Text doesnt fit inside ui-window !\n");
        return;
    }

    // draw text wrapped
    int curr_y = 1 + INNER_PADDING;
    int curr_x = 1 + INNER_PADDING;
    for (int i = 0; i < text_len; ++i) {
        win->content[curr_y][curr_x] = (int)text[i];

        // line wrap
        if (curr_x - 1 == win->w) {
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

    win->w = width;
    win->h = height;
    win->pos = pos;

    win->content = (int**)create2dArray(win->w, win->w, int);//(int **)calloc(nobjs, sizeof(int));
    win->colormap =(Color**)create2dArray(win->w, win->h, Color); //(Color **)malloc(nobjs * sizeof(Color));

    // set Colormap to backgroundcolor
    for (int j = 0; j < win->h; ++j) {
        for (int i = 0; i < win->w; ++i) {
            win->colormap[j][i] = in_bg;
        }
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
    // set Color of entire text
    int curr_y = 1 + INNER_PADDING;
    int curr_x = 1 + INNER_PADDING;

    int max_index = (win->w - 1 - INNER_PADDING) * (win->h - 1 - INNER_PADDING);

    for (int i = 0; i < max_index; ++i) {
        win->content[curr_y][curr_x] = (int)' ';

        // line wrap
        if (curr_x - 1 == win->w) {
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

    int max_index = (win->w - 1 - INNER_PADDING) * (win->h - 1 - INNER_PADDING);

    for (int i = 0; i < max_index; ++i) {
        win->colormap[curr_y][curr_x] = textColor;

        // line wrap
        if (curr_x - 1 == win->w) {
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
    free2dArray(win->h, win->content);
    free2dArray(win->h,win->colormap);
    free(win);
}
