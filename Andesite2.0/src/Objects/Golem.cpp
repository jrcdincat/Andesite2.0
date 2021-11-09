#include "../pch.h"
#include "Golem.h"
#include "../Constants.h"
#include "../Graphics/TextureManager.h"
using namespace constants;

Golem::Golem(Properties* properties): Actor(properties)
{
	row = 0;
	frameCount = 18; 
	animationSpeed = 80;
	animation = new Animation();
	animation->SetProperties(textureID, row, frameCount, animationSpeed);
	collisionWidth = 25;
	collisionHeight = 40;
	physicsBody = Physics::GetInstance()->AddEnemyRect(properties->position.x, properties->position.y, collisionWidth, collisionHeight, this);
	physicsBody->SetGravityScale(0.1f);
	physicsBody->SetLinearDamping(1.3f);
}

Golem::~Golem()
{
	delete animation;
}

void Golem::Draw()
{
	animation->Draw(physicsBody->GetPosition().x * PIXEL_PER_METER - GOLEM_X_OFFSET_ANIMATION, physicsBody->GetPosition().y * PIXEL_PER_METER - GOLEM_Y_OFFSET_ANIMATION, width, height);
	// TextureManager::GetInstance()->DrawRect(physicsBody->GetPosition().x * PIXEL_PER_METER, physicsBody->GetPosition().y * PIXEL_PER_METER, collisionWidth, collisionHeight);
}

void Golem::Update(float dt)
{
	Idle();

	animation->Update();
	origin->x = physicsBody->GetPosition().x * PIXEL_PER_METER + width / 2;
	origin->y = physicsBody->GetPosition().y * PIXEL_PER_METER + height / 2;
}

void Golem::Clean()
{
}

void Golem::Idle()
{
	animation->SetProperties("golem_idle", 0, 18, 80, SDL_FLIP_HORIZONTAL);
	b2Vec2 velocity = b2Vec2(0.0f, physicsBody->GetLinearVelocity().y);
	physicsBody->SetLinearVelocity(velocity);
}

void Golem::MoveRight()
{
	animation->SetProperties("golem_walking", 0, 24, 80);
	b2Vec2 velocity = b2Vec2(1.0f, physicsBody->GetLinearVelocity().y);
	physicsBody->SetLinearVelocity(velocity);
}

void Golem::MoveLeft()
{
	animation->SetProperties("golem_walking", 0, 24, 80, SDL_FLIP_HORIZONTAL);
	b2Vec2 velocity = b2Vec2(-1.0f, physicsBody->GetLinearVelocity().y);
	physicsBody->SetLinearVelocity(velocity);
}

void Golem::Die()
{
	animation->SetProperties("golem_dying", 0, 18, 80, SDL_FLIP_HORIZONTAL);
	
	for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext())
	{
		b2Filter filter = fixture->GetFilterData();
		filter.maskBits = BOUNDARY;
		fixture->SetFilterData(filter);
	}
}