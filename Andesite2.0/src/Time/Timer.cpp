#include "../pch.h"
#include "Timer.h"

Timer* Timer::instance = nullptr;

void Timer::Tick() 
{
	deltaTime = (SDL_GetTicks() - prevTime) * (TARGET_FPS / 1000.0f);
	
	if (deltaTime > TARGET_DELTATIME) {
		deltaTime = TARGET_DELTATIME;
	}
	
	prevTime = SDL_GetTicks();
}