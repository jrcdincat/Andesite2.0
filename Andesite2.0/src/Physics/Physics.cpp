#include "../pch.h"
#include "Physics.h"
#include "../Time/Timer.h"

Physics* Physics::physicsInstance = nullptr;

Physics::Physics()
{
	worldGravity = 9.8f;
	physicsWorld = new b2World(b2Vec2(0.0f, worldGravity)); // Create box2d world with 9.8 gravity
}

Physics::~Physics() 
{
	Clean();
}

void Physics::Clean() 
{
	delete physicsWorld;
}

void Physics::Render()
{
	int dt = Timer::GetInstance()->GetDeltaTime();
	int velocityIterations = 6;
	int positionIterations = 2;
	physicsWorld->Step(dt, velocityIterations, positionIterations);
}

b2Body* Physics::AddRect(int x, int y, int w, int h, bool isDynamic) {
	b2BodyDef bodyDef;
	bodyDef.position.Set(x, y);
	
	if (isDynamic)
	{
		bodyDef.type = b2_dynamicBody;
	}

	b2Body* body = physicsWorld->CreateBody(&bodyDef);
	b2PolygonShape shape;
	shape.SetAsBox(w / 2, h / 2);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	body->CreateFixture(&fixtureDef);
	
	return body;
}
