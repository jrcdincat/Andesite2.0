#include "../pch.h"
#include "InputManager.h"
InputManager* InputManager::inputManagerInstance = nullptr;

InputManager::InputManager() {
	buttonEscape = new EscapeCommand();
	buttonSpace = new JumpCommand();
	buttonA = new MoveLeftCommand();
	buttonD = new MoveRightCommand();
	buttonS = nullptr;
	buttonW = nullptr;
}

InputManager::~InputManager() {
	delete buttonEscape;
	delete buttonSpace;
	delete buttonA;
	delete buttonD;
}

// Get keyboard input state
Command* InputManager::handleKeyInput(const Uint8* state) {
	if (state[SDL_SCANCODE_A]) {
		return buttonA;
	}

	else if (state[SDL_SCANCODE_SPACE])
	{
		return buttonSpace;
	}

	else if (state[SDL_SCANCODE_D])
	{
		return buttonD;
	}

	else if (state[SDL_SCANCODE_ESCAPE])
	{
		return buttonEscape;
	}

	return NULL;
}
