#include "../pch.h"
#include "Physics.h"
#include "../Time/Timer.h"
#include "../Constants.h"
using namespace constants;

Physics* Physics::physicsInstance = nullptr;

Physics::Physics()
{
	// Initialize physics world
	worldGravity = PHYSICS_WORLD_GRAVITY;
	physicsWorld = new b2World(b2Vec2(0.0f, worldGravity));
	physicsWorld->SetAllowSleeping(true);

	// Initialize physics world contact listener
	contactListener = new ContactListener();
	physicsWorld->SetContactListener(contactListener);

	numFootContacts = 0;
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

	physicsWorld->Step(dt, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
}

b2Body* Physics::AddBoundaryRect(int x, int y, int w, int h)
{
	b2BodyDef bodyDef;
	b2Body* body;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;
	FixtureUserData* userData;

	// Create boundary static body definition
	bodyDef.position.Set(x / PIXEL_PER_METER, y / PIXEL_PER_METER);
	body = physicsWorld->CreateBody(&bodyDef);

	// Set fixture definition
	shape.SetAsBox((w / PIXEL_PER_METER ) / 2, (h / PIXEL_PER_METER) / 2);
	fixtureDef.shape = &shape;
	fixtureDef.density = 400.0f;
	fixtureDef.friction = 0.01f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.filter.categoryBits = BOUNDARY;
	fixtureDef.filter.maskBits = PLAYER | PLAYER_FEET| ENEMY | ENEMY_HEAD;

	// Set boundary fixture user data
	userData = new FixtureUserData();
	userData->type = USER_TYPE_BOUDNARY;
	userDataList.push_back(userData);
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(userData);

	body->CreateFixture(&fixtureDef);
	
	return body;
}

b2Body* Physics::AddPlayerRect(int x, int y, int w, int h, void* object) 
{
	b2BodyDef bodyDef;
	b2Body* body;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;
	FixtureUserData* userData;

	// Create player dynamic body definition
	bodyDef.position.Set(x / PIXEL_PER_METER, y / PIXEL_PER_METER);
	bodyDef.type = b2_dynamicBody;
	body = physicsWorld->CreateBody(&bodyDef);
	body->SetFixedRotation(true);


	// Set fixture definition
	shape.SetAsBox((w / PIXEL_PER_METER) / 2, (h / PIXEL_PER_METER) / 2);
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.01f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.filter.categoryBits = PLAYER;
	fixtureDef.filter.maskBits = BOUNDARY | ENEMY | ENEMY_HEAD | GEM;

	// Set player fixture user data
	userData = new FixtureUserData();
	userData->type = USER_TYPE_PLAYER;
	userData->object = object;
	userDataList.push_back(userData);
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(userData);

	body->CreateFixture(&fixtureDef);

	// Set player foot sensor fixture user data
	userData = new FixtureUserData();
	userData->type = USER_TYPE_PLAYER_FEET;
	userDataList.push_back(userData);

	// Set player foot sensor fixture definition
	shape.SetAsBox(0.3f, 0.3f, b2Vec2(0.0f, 2.0f), 0);
	fixtureDef.filter.categoryBits = PLAYER_FEET;
	fixtureDef.isSensor = true;
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(userData);

	body->CreateFixture(&fixtureDef);
	return body;
}

b2Body* Physics::AddEnemyRect(int x, int y, int w, int h, int type, void* object) 
{
	b2BodyDef bodyDef;
	b2Body* body;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;
	FixtureUserData* userData;

	// Create enemy dynamic body definition
	bodyDef.position.Set(x / PIXEL_PER_METER, y / PIXEL_PER_METER);
	bodyDef.type = b2_dynamicBody;
	body = physicsWorld->CreateBody(&bodyDef);\
	body->SetFixedRotation(true);

	// Set fixture definition
	shape.SetAsBox((w / PIXEL_PER_METER) / 2, (h / PIXEL_PER_METER) / 2);
	fixtureDef.shape = &shape;
	fixtureDef.density = 200.0f;
	fixtureDef.friction = 0.01f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.filter.categoryBits = ENEMY;
	fixtureDef.filter.maskBits = BOUNDARY | PLAYER;

	// Set enemy fixture user data
	userData = new FixtureUserData();
	userData->type = type;
	userData->object = object;
	userDataList.push_back(userData);
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(userData);

	body->CreateFixture(&fixtureDef);

	// Set enemy head fixture definition and user data
	shape.SetAsBox(0.3, 0.3, b2Vec2(0.0f, -0.5f), 0);
	fixtureDef.filter.categoryBits = ENEMY_HEAD;
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(userData);

	body->CreateFixture(&fixtureDef);
	return body;
}

b2Body* Physics::AddRockRect(int x, int y, int w, int h, void* object) 
{
	b2BodyDef bodyDef;
	b2Body* body;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;
	FixtureUserData* userData;

	// Create rock dynamic body definition
	bodyDef.position.Set(x / PIXEL_PER_METER, y / PIXEL_PER_METER);
	bodyDef.type = b2_dynamicBody;
	body = physicsWorld->CreateBody(&bodyDef);

	// Set fixture definition
	shape.SetAsBox((w / PIXEL_PER_METER) / 2, (h / PIXEL_PER_METER) / 2);
	fixtureDef.shape = &shape;
	fixtureDef.density = 200.0f;
	fixtureDef.friction = 0.01f;
	fixtureDef.restitution = 0.0f;
	fixtureDef.filter.categoryBits = ROCK;
	fixtureDef.filter.maskBits = BOUNDARY | PLAYER;

	// Set rock fixture user data
	userData = new FixtureUserData();
	userData->type = USER_TYPE_ROCK;
	userData->object = object;
	userDataList.push_back(userData);
	fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(userData);

	body->CreateFixture(&fixtureDef);
	return body;
}