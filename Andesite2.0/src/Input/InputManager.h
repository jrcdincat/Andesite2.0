#pragma once
#include "Command.h"
#include "../Menu/MainMenu.h"

class InputManager {
public:
	static InputManager* GetInstance() { return inputManagerInstance = (inputManagerInstance != nullptr) ? inputManagerInstance : new InputManager(); }
	InputManager();
	~InputManager();
	Command* handleKeyInput(const Uint8* state);
	bool HandleButtonEvent(SDL_Rect rect);
	void HandleEvent(Player* player);

private: 
	const Uint8* keyState;
	Command* command;
	Command* buttonA; 
	Command* buttonS;
	Command* buttonD;
	Command* buttonW;
	Command* buttonSpace;
	Command* button_A_Space;
	Command* button_D_Space;
	Command* buttonEscape;
	static InputManager* inputManagerInstance;
};