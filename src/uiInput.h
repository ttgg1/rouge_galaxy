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
#define POPUP_WINDOWBUFFERFACTOR 3

// char*: input string
typedef void (*uiI_onCloseCallback)(char*);

typedef struct uiI_popup {
	char* inputBufferPointer;
	char* windowTextBufferPointer;
	uiI_onCloseCallback callbackFunction;
	ui_win_t* windowPointer;

	game_t* game;

	bool showInput;
	bool gameSimulating;

	int windowIndex;
	int eventHookIndex;
} uiI_popup_t;

// waits for input, closes with Enter -> returns input
// WARNING: char* needs to be freed after callback

extern uiI_popup_t* uiI_createPopupWindow(game_t* game, char* text, bool doShowInput, uiI_onCloseCallback callback);
extern void uiI_destroyPopupWindow(uiI_popup_t* popup);

extern void uiI_openPopup(uiI_popup_t* popup);

extern void uiI_closePopupWindow(uiI_popup_t* popup);
#endif // UIINPUT_H_
