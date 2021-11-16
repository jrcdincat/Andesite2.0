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
	numEnemyHeadContacts = 0;
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

b2Body* Physics::AddBoundaryRect(int x, int y, int w, int h, bool isDynamic, bool isPlayer) {
	b2BodyDef bodyDef;
	b2PolygonShape shape;
	b2Body* body;
	b2FixtureDef fixtureDef;
	FixtureUserData* userData;

	// Static Boundary
	bodyDef.position.Set(x / PIXEL_PER_METER, y / PIXEL_PER_METER);
	body = physicsWorld->CreateBody(&bodyDef);
	shape.SetAsBox((w / PIXEL_PER_METER ) / 2, (h / PIXEL_PER_METER) / 2);
	fixtureDef.shape = &shape;
	fixtureDef.density = 400.0f;
	fixtureDef.friction = 0.01f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.filter.categoryBits = BOUNDARY;
	fixtureDef.filter.maskBits = PLAYER | PLAYER_FEET| ENEMY | ENEMY_HEAD;
	userData = new FixtureUserData();
	userData->type = USER_TYPE_BOUDNARY;
	userDataList.push_back(userData);
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(userData);
	body->CreateFixture(&fixtureDef);
	
	return body;
}


b2Body* Physics::AddPlayerRect(int x, int y, int w, int h, void* object) {
	b2BodyDef bodyDef;
	b2PolygonShape shape;
	b2Body* body;
	b2FixtureDef fixtureDef;
	FixtureUserData* userData;

	// Player Body
	bodyDef.position.Set(x / PIXEL_PER_METER, y / PIXEL_PER_METER);
	bodyDef.type = b2_dynamicBody;
	body = physicsWorld->CreateBody(&bodyDef);
	shape.SetAsBox((w / PIXEL_PER_METER) / 2, (h / PIXEL_PER_METER) / 2);
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.01f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.filter.categoryBits = PLAYER;
	fixtureDef.filter.maskBits = BOUNDARY | ENEMY | ENEMY_HEAD | GEM;
	userData = new FixtureUserData();
	userData->type = USER_TYPE_PLAYER;
	userData->object = object;
	userDataList.push_back(userData);
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(userData);
	body->CreateFixture(&fixtureDef);

	// Player Feet
	userData = new FixtureUserData();
	userData->type = USER_TYPE_PLAYER_FEET;
	userDataList.push_back(userData);
	shape.SetAsBox(0.3f, 0.3f, b2Vec2(0.0f, 2.0f), 0);
	fixtureDef.filter.categoryBits = PLAYER_FEET;
	fixtureDef.isSensor = true;
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(userData);
	body->CreateFixture(&fixtureDef);
	body->SetFixedRotation(true);

	return body;
}

b2Body* Physics::AddEnemyRect(int x, int y, int w, int h, int type, void* object) {
	b2BodyDef bodyDef;
	b2PolygonShape shape;
	b2Body* body;
	b2FixtureDef fixtureDef;
	FixtureUserData* userData;

	// Enemy Body
	bodyDef.position.Set(x / PIXEL_PER_METER, y / PIXEL_PER_METER);
	bodyDef.type = b2_dynamicBody;
	body = physicsWorld->CreateBody(&bodyDef);
	shape.SetAsBox((w / PIXEL_PER_METER) / 2, (h / PIXEL_PER_METER) / 2);
	fixtureDef.shape = &shape;
	fixtureDef.density = 200.0f;
	fixtureDef.friction = 0.01f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.filter.categoryBits = ENEMY;
	fixtureDef.filter.maskBits = BOUNDARY | PLAYER;
	userData = new FixtureUserData();
	userData->type = type;
	userData->object = object;
	userDataList.push_back(userData);
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(userData);
	body->CreateFixture(&fixtureDef);

	// Enemy Head
	shape.SetAsBox(0.3, 0.3, b2Vec2(0.0f, -0.5f), 0);
	fixtureDef.filter.categoryBits = ENEMY_HEAD;
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(userData);
	body->CreateFixture(&fixtureDef);
	body->SetFixedRotation(true);

	return body;
}

b2Body* Physics::AddRockRect(int x, int y, int w, int h, void* object) {
	b2BodyDef bodyDef;
	b2PolygonShape shape;
	b2Body* body;
	b2FixtureDef fixtureDef;
	FixtureUserData* userData;

	// Enemy Body
	bodyDef.position.Set(x / PIXEL_PER_METER, y / PIXEL_PER_METER);
	bodyDef.type = b2_dynamicBody;
	body = physicsWorld->CreateBody(&bodyDef);
	shape.SetAsBox((w / PIXEL_PER_METER) / 2, (h / PIXEL_PER_METER) / 2);
	fixtureDef.shape = &shape;
	fixtureDef.density = 200.0f;
	fixtureDef.friction = 0.01f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.filter.categoryBits = ROCK;
	fixtureDef.filter.maskBits = BOUNDARY | PLAYER;
	userData = new FixtureUserData();
	userData->type = USER_TYPE_ROCK;
	userData->object = object;
	userDataList.push_back(userData);
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(userData);
	body->CreateFixture(&fixtureDef);

	return body;
}