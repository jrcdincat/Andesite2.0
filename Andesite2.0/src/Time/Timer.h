#pragma once
const int TARGET_FPS = 60;
const float TARGET_DELTATIME = 1.5f;

class Timer {
public: 
	void Tick();
	inline float GetDeltaTime() { return deltaTime; }
	inline static Timer* GetInstance() { return instance = (instance != nullptr) ? instance : new Timer(); }
	
private: 
	Timer() = default; 
	static Timer* instance; 
	float deltaTime;
	float prevTime;
};