#pragma once
#include <box2d.h>
#include <b2_settings.h>
#include <vector>
#include "ContactListener.h"

class Physics {
	public:
		static Physics* GetInstance() {
			return physicsInstance = (physicsInstance != nullptr) ? physicsInstance : new Physics();
		}

		~Physics();
		void Render();

		// Add physic body rects
		b2Body* AddBoundaryRect(int x, int y, int w, int h);
		b2Body* AddPlayerRect(int x, int y, int w, int h, void* object);
		b2Body* AddEnemyRect(int x, int y, int w, int h, int type, void* object);
		b2Body* AddRockRect(int x, int y, int w, int h, void* object);

		int numFootContacts; // Used to determine when jumping is allowed

		struct FixtureUserData 
		{
			int type;
			void* object;
		};

	private:
		Physics();

		static Physics* physicsInstance;
		b2World* physicsWorld;
		float worldGravity;
		ContactListener* contactListener;
		std::vector<Physics::FixtureUserData*> userDataList;
};