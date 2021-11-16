#pragma once
#include "Command.h"
#include "../Menu/MainMenu.h"

class InputManager {
	public:
		static InputManager* GetInstance() { 
			return inputManagerInstance = (inputManagerInstance != nullptr) ? inputManagerInstance : new InputManager(); 
		}
		InputManager();
		~InputManager();

		Command* HandleKeyInput(const Uint8* state);
		void HandleEvent(Player* player);
		bool HandleButtonEvent(SDL_Rect rect);

	private: 
		static InputManager* inputManagerInstance;
		const Uint8* keyState;
		Command* command;
		Command* buttonA; 
		Command* buttonD;
		Command* buttonSpace;
		Command* button_A_Space;
		Command* button_D_Space;
};