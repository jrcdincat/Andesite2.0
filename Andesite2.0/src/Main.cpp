#include <iostream>
#include "pch.h"
#include <vector>
#include <SDL_image.h>
#include <box2d.h>

#include <map>
#include "Time/Timer.h"
#include "Game.h"
#include "Constants.h"
using namespace constants;

const int XPOS = SDL_WINDOWPOS_UNDEFINED;
const int YPOS = SDL_WINDOWPOS_UNDEFINED;
 
int main(int argc, char*args[])
{
	bool fullscreen = false;

	// Create and Initialize Game
	Game::GetInstance()->Init("Andesite", XPOS, YPOS, SCREEN_WIDTH, SCREEN_HEIGHT, fullscreen);

	// Game Loop
	while (Game::GetInstance()->IsRunning() == true)
	{
		Game::GetInstance()->HandleEvent();
		Game::GetInstance()->Update();
		Game::GetInstance()->Render();
		Timer::GetInstance()->Tick();
	}

	delete Game::GetInstance();
	delete Timer::GetInstance();
	return 0;
}
