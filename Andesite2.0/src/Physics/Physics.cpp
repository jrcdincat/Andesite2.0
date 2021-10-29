#include "../pch.h"
#include "Physics.h"
#include "../Time/Timer.h"
#include "../Constants.h"
using namespace constants;

Physics* Physics::physicsInstance = nullptr;

Physics::Physics()
{
	worldGravity = 3.8f;
	physicsWorld = new b2World(b2Vec2(0.0f, worldGravity)); // Create box2d world with 9.8 gravity
	physicsWorld->SetAllowSleeping(true);
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
	int velocityIterations = 8;
	int positionIterations = 3;
	physicsWorld->Step(dt, velocityIterations, positionIterations);
}

b2Body* Physics::AddRect(int x, int y, int w, int h, bool isDynamic) {
	b2BodyDef bodyDef;
	bodyDef.position.Set(x / PIXEL_PER_METER, y / PIXEL_PER_METER);
	
	if (isDynamic)
	{
		bodyDef.type = b2_dynamicBody;
	}

	b2Body* body = physicsWorld->CreateBody(&bodyDef);
	b2PolygonShape shape;
	shape.SetAsBox((w / PIXEL_PER_METER ) / 2, (h / PIXEL_PER_METER) / 2);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.01f;
	fixtureDef.restitution = 0.0f;
	body->CreateFixture(&fixtureDef);
	
	return body;
}
