#pragma once
#include "GameObject.h"
#include <string>
class Actor : public GameObject {

public: 
	Actor() = default;
	Actor(Properties* properties) : GameObject(properties){}
	virtual ~Actor() {}

	virtual void Draw() = 0;
	virtual void Update(float dt) = 0;
	virtual void Clean() = 0;

protected: 
	std::string name; 
};