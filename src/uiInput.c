#include "uiInput.h"

/*INTERNAL FUNCTIONS**********************************************************/

void gameInputHook(game_t* game, uiI_popup_t* popup)
{
	if(popup->inputBufferPointer != NULL) {
		int pressedKey = GetCharPressed();
		static unsigned int bufferIndex = 0;
		static char pressedKeyString[2];

		// terminate String
		pressedKeyString[1] = '\0';
		pressedKeyString[0] = (char) pressedKey;

		while(pressedKey > 0) {
			if((pressedKey >= 32) && (pressedKey <= 125)) {
				if(bufferIndex + 1 >= POPUP_BUFFERSIZE) {
					debug_print("PopupWindow Input-Buffer full! \n");
					return;
				}

				popup->inputBufferPointer[bufferIndex] = (char) pressedKey;
				popup->inputBufferPointer[bufferIndex + 1] = '\0';
				++bufferIndex;

				// add to window Text
				if(popup->showInput) {
					strcat(popup->windowTextBufferPointer, pressedKeyString);
					ui_updateWindowText(popup->windowPointer, popup->windowTextBufferPointer);
				}
			}

			pressedKey = GetCharPressed();
			pressedKeyString[0] = (char) pressedKey;
		}

		if(IsKeyPressed(KEY_BACKSPACE)) {
			--bufferIndex;
			popup->inputBufferPointer[bufferIndex] = '\0';

			if(bufferIndex < 0) { bufferIndex = 0; }

			// remove last char form Window
			if(popup->showInput) {
				popup->windowTextBufferPointer[strlen(popup->windowTextBufferPointer) - 1] = '\0';
				ui_updateWindowText(popup->windowPointer, popup->windowTextBufferPointer);
			}
		}

		if(IsKeyPressed(KEY_ENTER)) {
			uiI_closePopupWindow(popup);
		}
	} else {
		debug_print("inputBufferPointer not initialized! \n");
		return;
	}
}

/*API FUNCTIONS***************************************************************/

uiI_popup_t* uiI_createPopupWindow(game_t* game, char* text, bool doShowInput, uiI_onCloseCallback callback)
{
	uiI_popup_t* popup = malloc(sizeof(uiI_popup_t));

	//Vector2 windowCenteredPos = {(game->interface->width / 2 - POPUP_WIDTH)* game->interface->gridCellSize, (game->interface->height / 2 - POPUP_HEIGHT)* game->interface->gridCellSize};

	Vector2 windowCenteredPos = {0, 0};

	popup->windowPointer = ui_createWindow(windowCenteredPos, POPUP_WIDTH, POPUP_HEIGHT, text, POPUP_TEXTSIZE, BORDER_PIPE, in_text, in_border,
	                                       in_windowBackground, &game->interface->font);
	popup->gameSimulating = game->isSimulating;

	popup->inputBufferPointer = malloc(sizeof(char) * (POPUP_BUFFERSIZE + 1));
	popup->windowTextBufferPointer = malloc(sizeof(char) * (POPUP_WINDOWBUFFERFACTOR * POPUP_BUFFERSIZE + 1));

	popup->showInput = doShowInput;
	popup->callbackFunction = callback;

	popup->game = game;

	// set Buffers
	popup->inputBufferPointer[0] = '\0';
	TextCopy(popup->windowTextBufferPointer, text);

	return popup;
}
void uiI_destroyPopupWindow(uiI_popup_t* popup)
{
	ui_destroyWindow(popup->windowPointer);
	free(popup->windowTextBufferPointer);

	free(popup);
}

void uiI_openPopup(uiI_popup_t* popup)
{
	if(popup == NULL) {
		debug_print("Tried to open uninitialized Popup !\n");
		return;
	}

	if(popup->game->isSimulating) {
		popup->game->isSimulating = false;
	}

	popup->game->isInputCaptured = true;

	// clear char queue
	GetCharPressed();

	// create Hooks
	popup->eventHookIndex = gm_addAlwaysEventHook(popup->game, (gm_event_func) gameInputHook);
	popup->windowIndex = in_addUiWindow(popup->windowPointer, popup->game->interface);

}

void uiI_closePopupWindow(uiI_popup_t* popup)
{
	gm_removeAlwaysEventHookAtIndex(popup->game, popup->eventHookIndex);
	in_removeUiWindowAtIndex(popup->game->interface, popup->windowIndex);

	popup->game->isSimulating = popup->gameSimulating;
	popup->game->isInputCaptured = false;

	char* result = malloc(sizeof(char) * (strlen(popup->inputBufferPointer) + 1));
	TextCopy(result, popup->inputBufferPointer);

	if(result == NULL) {
		debug_print("failed to copy input to result!\n");
		return;
	}

	if(popup->callbackFunction == NULL) {
		debug_print("invalid callback function !\n");
	} else {
		popup->callbackFunction(result);
	}
}
