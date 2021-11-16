#pragma once
#include <box2d.h>
#include "ContactListener.h"

class Physics {
public:
	struct FixtureUserData {
		int type;
		void* object;
	};

	static Physics* GetInstance() {
		return physicsInstance = (physicsInstance != nullptr) ? physicsInstance : new Physics();
	}

	~Physics();
	
	void Clean();
	void Render();

	b2Body* AddBoundaryRect(int x, int y, int w, int h, bool isDynamic, bool isPlayer);
	b2Body* AddEnemyRect(int x, int y, int w, int h, int type, void* object);
	b2Body* AddPlayerRect(int x, int y, int w, int h, void* object);
	b2Body* AddRockRect(int x, int y, int w, int h, void* object);

	int numFootContacts;
	int numEnemyHeadContacts;

private:
	Physics();
	static Physics* physicsInstance;
	b2World* physicsWorld;
	float worldGravity;
	ContactListener* contactListener;
};