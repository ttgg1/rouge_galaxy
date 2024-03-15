#include "uiInput.h"

char* openPopup(game_t* game, char* text, bool showInput)
{
	ui_inputWindow_t popupWindow;

	popupWindow.showInput = showInput;

	ivec2_t windowCenteredPos = {game->interface->width / 2 - POPUP_WIDTH, game->interface->height / 2 - POPUP_HEIGHT};

	popupWindow.window = ui_createWindow(windowCenteredPos, POPUP_WIDTH, POPUP_HEIGHT, text, POPUP_TEXTSIZE, BORDER_PIPE, in_text, in_border,
	                                     in_windowBackground, &game->interface->font);

	int windowIndex = in_addUiWindow(popupWindow.window, game->interface);
	const int textLength = strlen(text);

	// capture Input
	bool origSimulating = game->isSimulating;

	if(game->isSimulating) {
		game->isSimulating = false;
	}

	char inputBuffer[POPUP_BUFFERSIZE];
	char windowTextBuffer[POPUP_BUFFERSIZE + textLength];

	unsigned int bufferIndex = 0;

	while(!IsKeyPressed(KEY_ENTER)) {
		if(GetCharPressed()) {
			inputBuffer[bufferIndex] = (char) GetCharPressed();
			++bufferIndex;

			if(bufferIndex >= POPUP_BUFFERSIZE) {
				debug_print("PopupWindow Input-Buffer full! \n");
				return NULL;
			}

			// add to window Text
			strcat(windowTextBuffer, inputBuffer);
			ui_updateWindowText(popupWindow.window, windowTextBuffer);
		}
	}

	// cleanup
	in_destroyUiWindowAtIndex(game->interface, windowIndex);

	strcpy(popupWindow.inputString, inputBuffer);

	game->isSimulating = origSimulating;

	return popupWindow.inputString;
}
