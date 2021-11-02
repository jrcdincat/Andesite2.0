#pragma once
#include <box2d.h>
#include "ContactListener.h"

class Physics {
public:
	struct FixtureUserData {
		int type;
	};

	static Physics* GetInstance() {
		return physicsInstance = (physicsInstance != nullptr) ? physicsInstance : new Physics();
	}

	~Physics();
	
	void Clean();
	void Render();

	b2Body* AddRect(int x, int y, int w, int h, bool isDynamic);

	int numFootContacts;
private:
	Physics();
	static Physics* physicsInstance;
	b2World* physicsWorld;
	float worldGravity;
	ContactListener* contactListener;
};