#include "../pch.h"
#include "InputManager.h"
#include "../Game.h"
InputManager* InputManager::inputManagerInstance = nullptr;

InputManager::InputManager() {
	command = nullptr;
	keyState = nullptr;
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
				command = InputManager::GetInstance()->handleKeyInput(keyState);
				if (keyState[SDL_SCANCODE_D])
				{
					std::cout << "TESTING" << std::endl;
				}

				if (command)
				{
					std::cout << "in command" << std::endl;
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

			if (!player->isMoveLeft && !player->isMoveRight && !player->isJump)
			{
				player->Idle();
			}
			break;
		default:
			break;
		}
	}	
}