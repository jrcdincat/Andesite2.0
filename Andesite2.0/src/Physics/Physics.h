#pragma once
#include <box2d.h>

class Physics {
public:
	static Physics* GetInstance() {
		return physicsInstance = (physicsInstance != nullptr) ? physicsInstance : new Physics();
	}

	~Physics();
	
	void Clean();
	void Render();

	b2Body* AddRect(int x, int y, int w, int h, bool isDynamic);


private:
	Physics();
	static Physics* physicsInstance;
	b2World* physicsWorld;
	float worldGravity;
};