#ifndef UIINPUT_H_
#define UIINPUT_H_

#include "ui.h"
#include "game.h"
#include "colors.h"

#include <raylib.h>

#define POPUP_WIDTH 20
#define POPUP_HEIGHT 10

#define POPUP_TEXTSIZE 20
#define POPUP_BUFFERSIZE 255

// waits for input, closes with Enter -> returns input
extern char* uiI_openPopup(game_t* game, char* text, bool showInput);

#endif // UIINPUT_H_
