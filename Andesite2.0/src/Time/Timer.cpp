#include "../pch.h"
#include "Timer.h"

Timer* Timer::instance = nullptr;

void Timer::Tick() 
{
	deltaTime = (SDL_GetTicks() - prevTime);
	
	if (deltaTime < TARGET_DELTATIME)
	{
		SDL_Delay((int)floor(16.666f - deltaTime));
	}

	prevTime = (float)SDL_GetTicks();
}