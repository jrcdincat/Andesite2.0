#pragma once
#include "Command.h"

class InputManager {
public:
	static InputManager* GetInstance() { return inputManagerInstance = (inputManagerInstance != nullptr) ? inputManagerInstance : new InputManager(); }
	InputManager();
	~InputManager();
	Command* handleKeyInput(const Uint8* state);
	void HandleEvent(Player* player);

private: 
	const Uint8* keyState;
	Command* command;
	Command* buttonA; 
	Command* buttonS;
	Command* buttonD;
	Command* buttonW;
	Command* buttonSpace;
	Command* buttonEscape;
	static InputManager* inputManagerInstance;
};