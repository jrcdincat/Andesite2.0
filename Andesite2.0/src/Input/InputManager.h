#pragma once
#include "Command.h"

class InputManager {
public:
	static InputManager* GetInstance() { return inputManagerInstance = (inputManagerInstance != nullptr) ? inputManagerInstance : new InputManager(); }
	InputManager();
	~InputManager();
	Command* handleKeyInput(const Uint8* state);

private: 
	Command* buttonA; 
	Command* buttonS;
	Command* buttonD;
	Command* buttonW;
	Command* buttonSpace;
	static InputManager* inputManagerInstance;
};