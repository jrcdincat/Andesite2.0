#pragma once
#include "GameObject.h"

class Actor : public GameObject {
	public: 
		Actor(Properties* properties) : GameObject(properties){}
		virtual ~Actor() {}

		virtual void Draw() = 0;
		virtual void Update(float dt) = 0;
		virtual void Clean() = 0;

		virtual void UpdateAnimationState() = 0;
		virtual int GetCurrentState() = 0;
};