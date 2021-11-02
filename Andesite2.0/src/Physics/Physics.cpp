#include "../pch.h"
#include "Physics.h"
#include "../Time/Timer.h"
#include "../Constants.h"
#include "b2_settings.h"
#include <vector>

using namespace constants;

Physics* Physics::physicsInstance = nullptr;
std::vector<Physics::FixtureUserData*> userDataList;

Physics::Physics()
{
	worldGravity = 3.8f;
	physicsWorld = new b2World(b2Vec2(0.0f, worldGravity)); // Create box2d world with 9.8 gravity
	physicsWorld->SetAllowSleeping(true);
	numFootContacts = 0;
	contactListener = new ContactListener();
	physicsWorld->SetContactListener(contactListener);
}

Physics::~Physics() 
{
	Clean();
}

void Physics::Clean() 
{
	for (FixtureUserData* userData : userDataList)
	{
		delete userData;
	}


	delete physicsWorld;
	delete contactListener;
}

void Physics::Render()
{
	int dt = Timer::GetInstance()->GetDeltaTime();
	int velocityIterations = 8;
	int positionIterations = 3;
	physicsWorld->Step(dt, velocityIterations, positionIterations);
}

b2Body* Physics::AddRect(int x, int y, int w, int h, bool isDynamic) {
	b2BodyDef bodyDef;
	b2PolygonShape shape;

	bodyDef.position.Set(x / PIXEL_PER_METER, y / PIXEL_PER_METER);
	
	if (isDynamic)
	{
		bodyDef.type = b2_dynamicBody;
	}

	b2Body* body = physicsWorld->CreateBody(&bodyDef);

	shape.SetAsBox((w / PIXEL_PER_METER ) / 2, (h / PIXEL_PER_METER) / 2);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.01f;
	fixtureDef.restitution = 0.0f;
	FixtureUserData* userData = new FixtureUserData();
	userData->type = 2;
	userDataList.push_back(userData);
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(userData);
	body->CreateFixture(&fixtureDef);

	if (isDynamic)
	{
		FixtureUserData* userData = new FixtureUserData();
		userData->type = 1;
		userDataList.push_back(userData);
		shape.SetAsBox(0.3, 1, b2Vec2(0, 1), 0);
		fixtureDef.isSensor = true;
		fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(userData);
		body->CreateFixture(&fixtureDef);
	}
	
	return body;
}
