#include "uiInput.h"

char* uiI_openPopup(game_t* game, char* text, bool showInput)
{
#if 0
	//Vector2 windowCenteredPos = {game->interface->width / 2 - POPUP_WIDTH, game->interface->height / 2 - POPUP_HEIGHT};
	Vector2 windowCenteredPos = {0, 0};

	ui_win_t* window = ui_createWindow(windowCenteredPos, POPUP_WIDTH, POPUP_HEIGHT, text, POPUP_TEXTSIZE, BORDER_PIPE, in_text, in_border,
	                                   in_windowBackground, &game->interface->font);

	// capture Input
	bool origSimulating = game->isSimulating;

	if(game->isSimulating) {
		game->isSimulating = false;
	}

	char inputBuffer[POPUP_BUFFERSIZE + 1] = "\0";
	char windowTextBuffer[3 * POPUP_BUFFERSIZE + 1] = "\0";

	TextCopy(windowTextBuffer, text);

	unsigned int bufferIndex = 0;

	bool isShown = true;
	int pressedChar = 0;

	do {
		pressedChar = GetCharPressed();

		if(pressedChar > 0) {
			if((pressedChar >= 32) && (pressedChar <= 125)) {
				if(bufferIndex + 1 >= POPUP_BUFFERSIZE) {
					debug_print("PopupWindow Input-Buffer full! \n");
					return NULL;
				}

				inputBuffer[bufferIndex] = (char) pressedChar;
				inputBuffer[bufferIndex + 1] = '\0';
				++bufferIndex;

				// add to window Text
				if(showInput) {
					strcat(windowTextBuffer, inputBuffer);
					ui_updateWindowText(window, windowTextBuffer);
				}
			}

		}

		if(IsKeyPressed(KEY_BACKSPACE)) {
			--bufferIndex;

			if(bufferIndex < 0) { bufferIndex = 0; }

			inputBuffer[bufferIndex] = '\0';
		}

		if(IsKeyPressed(KEY_ENTER)) {
			isShown = false;
		}

		ui_drawUiWindow(window);
		isShown = false;
	} while(isShown);

	ui_destroyWindow(window);

	game->isSimulating = origSimulating;

	return inputBuffer;
#else
	return "test";
#endif
}
