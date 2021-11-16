#pragma once
#include "../Constants.h"
using namespace constants;

class Timer {
public: 
	inline static Timer* GetInstance() { return instance = (instance != nullptr) ? instance : new Timer(); }
	inline float GetDeltaTime() { return deltaTime; }
	void Tick();
	
private: 
	Timer() = default; 
	static Timer* instance; 
	float deltaTime;
	float prevTime;
};