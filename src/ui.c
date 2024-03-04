#include "ui.h"

static void calcGlyphWidth(ui_win_t *win) {
  // calculate width of one border Glyth
  if (win->borderStyle > 1) {
    int glyphIndex =
        GetGlyphIndex(*win->font, win->borderStyle ? (int)196 : (int)205);
    win->glyphWidth =
        (float)win->font->recs[glyphIndex].width * win->scaleFactor;
  } else {

    int glyphIndex = GetGlyphIndex(*win->font, (int)win->borderStyle);
    win->glyphWidth =
        (float)win->font->recs[glyphIndex].width * win->scaleFactor;
  }
}

static void drawBorders(ui_win_t *win, Image *backgroundImage) {
  // set Borders
  int borderChar = 0;

  switch (win->borderStyle) {
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

  int topBottom[win->width];
  int sideChar = win->borderStyle ? (int)179 : (int)186;

  if (borderChar > 1) {
    // set top and bottom
    for (int i = 0; i < win->width; ++i) {
      topBottom[i] = (int)borderChar;
    }

    // set sides
    sideChar = (int)borderChar;

  } else { // handle constructed borders
    // set top and bottom
    for (int i = 1; i < win->width - 1; ++i) {
      // sets corresponding char -> borderStyle ? BORDER_LINE : BORDER_PIPE
      topBottom[i] = win->borderStyle ? (int)196 : (int)205;
    }

    // set sides
    sideChar = win->borderStyle ? (int)179 : (int)186;

    // set corners
    topBottom[0] = win->borderStyle ? (int)218 : (int)201;
    topBottom[win->width - 1] = win->borderStyle ? (int)191 : (int)187;
  }

  // Codepoints -> UTF-8 text
  char *topBottom_str = LoadUTF8(topBottom, win->width);

  int sideChar_len = 0;
  char *sideChar_str = CodepointToUTF8(sideChar, &sideChar_len);

  // generate Image
  Image topBottomImage = ImageTextEx(*win->font, topBottom_str, win->textSize,
                                     win->borderSpacing, win->borderColor);

  Image sideCharImage = ImageTextEx(*win->font, sideChar_str, win->textSize,
                                    win->borderSpacing, win->borderColor);
  // top border
  ImageDraw(backgroundImage, topBottomImage,
            (Rectangle){0, 0, (float)topBottomImage.width,
                        (float)topBottomImage.height},
            (Rectangle){0, 0, (float)backgroundImage->width,
                        (float)topBottomImage.height},
            WHITE);
  // now bottom border
  ImageFlipVertical(&topBottomImage);

  float ypos = (float)(backgroundImage->height - topBottomImage.height);

  ImageDraw(backgroundImage, topBottomImage,
            (Rectangle){0, 0, (float)topBottomImage.width,
                        (float)topBottomImage.height},
            (Rectangle){0, ypos, (float)backgroundImage->width,
                        (float)topBottomImage.height},
            WHITE);

  float xpos = (float)(backgroundImage->width - sideCharImage.width);
  for (int i = 0; i < win->height - 2; ++i) {

    ImageDraw(
        backgroundImage, sideCharImage,
        (Rectangle){0, 0, (float)sideCharImage.width,
                    (float)sideCharImage.height},
        (Rectangle){xpos,
                    (float)(topBottomImage.height + i * sideCharImage.height),
                    (float)sideCharImage.width, (float)(sideCharImage.height)},
        WHITE);
    ImageDraw(backgroundImage, sideCharImage,
              (Rectangle){0, 0, (float)sideCharImage.width,
                          (float)sideCharImage.height},
              (Rectangle){
                  0, (float)(topBottomImage.height + i * sideCharImage.height),
                  (float)sideCharImage.width, (float)(sideCharImage.height)},
              WHITE);
  }
  UnloadImage(topBottomImage);
  UnloadImage(sideCharImage);

  UnloadUTF8(topBottom_str);
}

// From:
// https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.  The return
// value must NOT be deallocated using free() etc.
static char *trimwhitespace(char *str) {
  char *end;

  // Trim leading space
  while (isspace((unsigned char)*str))
    str++;

  if (*str == 0) // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end))
    end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}

static void writeText(ui_win_t *win, Image *backgroundImage) {
  const int text_len = (int)strlen(win->text);

  // check if text fits in window
  if (text_len >
      (win->width - 2 * INNER_PADDING) * (win->height - 2 * INNER_PADDING)) {
    debug_print("Text doesnt fit inside ui-window !\n");
  }

  int wraps = (int)text_len / win->width + 1;

  int paddedWidth = win->width - INNER_PADDING;
  int paddedHeight = win->height - INNER_PADDING;

  Image textCanvas =
      GenImageColor((int)(floor(paddedWidth * win->glyphWidth)),
                    (int)(floor(paddedHeight * win->glyphWidth)), BLANK);

  if (wraps > 0) {
    char *lines[wraps];
    Vector2 pos = {0., 0.};
    // each line of text
    for (int i = 0; i < wraps; ++i) {
      lines[i] = TextSubtext(win->text, i * paddedWidth, paddedWidth);

      // remove whitespaces
      char *editedLine = trimwhitespace(lines[i]);

      ImageDrawTextEx(&textCanvas, *win->font, editedLine, pos, win->textSize,
                      win->textSpacing, win->textColor);
      pos.y += (float)(win->textSize + win->textSize / 2);
    }
  } else {
    ImageDrawTextEx(&textCanvas, *win->font, win->text, (Vector2){0., 0.},
                    win->textSize, win->textSpacing, win->textColor);
  }

  ImageDraw(
      backgroundImage, textCanvas,
      (Rectangle){0., 0., (float)textCanvas.width, (float)textCanvas.height},
      (Rectangle){INNER_PADDING * win->glyphWidth,
                  INNER_PADDING * win->glyphWidth, (float)textCanvas.width,
                  (float)textCanvas.height},
      WHITE);

  UnloadImage(textCanvas);
}

ui_win_t *ui_createWindow(ivec2_t pos, uint16_t width, uint16_t height,
                          char *text, float textSize,
                          ui_win_border_t borderStyle, Color textColor,
                          Color borderColor, Color backgroundColor,
                          Font *font) {
  ui_win_t *win = (ui_win_t *)malloc(sizeof(ui_win_t));

  win->width = width;
  win->height = height;
  win->pos = pos;
  win->textColor = textColor;
  win->borderColor = borderColor;
  win->backgroundColor = backgroundColor;
  win->borderStyle = borderStyle;
  win->text = text;
  win->textSize = textSize;
  win->font = font;
  win->scaleFactor = (float)(win->textSize / win->font->baseSize);
  win->textSpacing = 1.0f * win->scaleFactor;
  win->borderSpacing = 0.0f * win->scaleFactor;

  calcGlyphWidth(win);

  ui_generateWindowTexture(win);

  return win;
}

void ui_generateWindowTexture(ui_win_t *win) {
  // generate Window and text image

  Image backgroundImage =
      GenImageColor((int)(win->width * win->glyphWidth),
                    (int)(win->height * win->textSize), win->backgroundColor);

  drawBorders(win, &backgroundImage);

  writeText(win, &backgroundImage);

  // generate Texture2D
  win->WindowTexture = (Texture2D *)malloc(sizeof(Texture2D));
  *win->WindowTexture = LoadTextureFromImage(backgroundImage);

  UnloadImage(backgroundImage);
}

void ui_clearWindowText(ui_win_t *win) {
  win->text[0] = (char)'\0';
  ui_generateWindowTexture(win);
}

void ui_updateWindowText(ui_win_t *win, char *text) {
  win->text = text;
  ui_generateWindowTexture(win);
}

void ui_updateWindowTextColor(ui_win_t *win, Color textColor) {
  win->textColor = textColor;
  ui_generateWindowTexture(win);
}

void ui_updateWindowTextAndColor(ui_win_t *win, char *text, Color textColor) {
  win->textColor = textColor;
  win->text = text;
  ui_generateWindowTexture(win);
}

void ui_destroyWindow(ui_win_t *win) {
  UnloadTexture(*win->WindowTexture);
  free(win->WindowTexture);
  free(win);
}
