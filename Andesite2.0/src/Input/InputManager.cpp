#include "../pch.h"
#include "InputManager.h"
#include "../Game.h"
#include "../Constants.h"
using namespace constants;

InputManager* InputManager::inputManagerInstance = nullptr;

InputManager::InputManager() {
	command = nullptr;
	keyState = nullptr;
	buttonSpace = new JumpCommand();
	button_A_Space = new JumpLeftCommand();
	button_D_Space = new JumpRightCommand();
	buttonA = new MoveLeftCommand();
	buttonD = new MoveRightCommand();
}

InputManager::~InputManager() {
	delete buttonSpace;
	delete button_A_Space;
	delete button_D_Space;
	delete buttonA;
	delete buttonD;
}

/// Get keyboard input state
Command* InputManager::HandleKeyInput(const Uint8* state) {
	if (state[SDL_SCANCODE_A] && state[SDL_SCANCODE_SPACE])
	{
		return button_A_Space;
	}
	else if (state[SDL_SCANCODE_D] && state[SDL_SCANCODE_SPACE]) 
	{
		return button_D_Space;
	}
	else if (state[SDL_SCANCODE_A])
	{
		return buttonA;
	}
	else if (state[SDL_SCANCODE_D])
	{
		return buttonD;
	}
	else if (state[SDL_SCANCODE_SPACE])
	{
		return buttonSpace;
	}
	else
	{
		return NULL;

	}
}

void InputManager::HandleEvent(Player* player)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		keyState = SDL_GetKeyboardState(NULL);
		switch (event.type)
		{
			case SDL_QUIT:
				Game::GetInstance()->SetIsRunning(false);
				break;

			case SDL_KEYDOWN:
				if (event.key.repeat == 0)
				{
					command = InputManager::GetInstance()->HandleKeyInput(keyState);
					if (command)
					{
						command->execute(player);
					}
				}
				break;

			case SDL_KEYUP:
				if (event.key.keysym.scancode == SDL_SCANCODE_D)
				{
					player->isMoveRight = false;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_A)
				{
					player->isMoveLeft = false;
				}
				break;

			default:
				break;
		}
	}
}

bool InputManager::HandleButtonEvent(SDL_Rect rect)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				Game::GetInstance()->SetIsRunning(false);
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					int x, y;
					SDL_GetMouseState(&x, &y);

					if (x > rect.x && x < (rect.x + rect.w) && y > rect.y && y < (rect.y + rect.h))
					{
						return true;
					}
				}
				break;
		}
	}
	return false;
}
