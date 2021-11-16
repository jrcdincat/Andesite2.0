#include "../pch.h"
#include "Rock.h"
#include "../Constants.h"
#include "../Graphics/TextureManager.h"
#include "../Audio/AudioManager.h"
using namespace constants;
using namespace EnemyStates;

Rock::Rock(Properties* properties) : Actor(properties)
{
	row = 0;
	frameCount = 23;
	animationSpeed = 80;
	animation = new Animation();
	isExplode = false;

	animation->SetProperties(textureID, true, row, frameCount, animationSpeed);
	collisionWidth = 40;
	collisionHeight = 37;
	physicsBody = Physics::GetInstance()->AddRockRect(
		properties->position.x,
		properties->position.y,
		collisionWidth,
		collisionHeight,
		this
	);
	physicsBody->SetGravityScale(0.1f);
	physicsBody->SetLinearDamping(1.3f);
	playerInstance = Game::GetInstance()->GetPlayer();
	playerBody = playerInstance->GetPlayerBody();
	detectRange = 20.0f;
	physicsBody->SetEnabled(false);
	isNotVisible = false;
}

Rock::~Rock()
{
	physicsBody->GetWorld()->DestroyBody(physicsBody);
	delete animation;
}

void Rock::Draw()
{
	if (isNotVisible)
	{
		return;
	}
	if (isExplode)
	{
		animation->Draw(physicsBody->GetPosition().x * PIXEL_PER_METER - EXPLOSION1_X_OFFSET_ANIMATION,
			physicsBody->GetPosition().y * PIXEL_PER_METER - EXPLOSION1_Y_OFFSET_ANIMATION,
			160,
			160
		);
	}
	else
	{
		animation->Draw(
			physicsBody->GetPosition().x * PIXEL_PER_METER - ROCK1_X_OFFSET_ANIMATION,
			physicsBody->GetPosition().y * PIXEL_PER_METER - ROCK1_Y_OFFSET_ANIMATION,
			width,
			height
		);
	}

	// TextureManager::GetInstance()->DrawRect(physicsBody->GetPosition().x * PIXEL_PER_METER, physicsBody->GetPosition().y * PIXEL_PER_METER, collisionWidth, collisionHeight);
}

void Rock::Update(float dt)
{
	FallWhenPlayerInRange();
	UpdateAnimationState();
	animation->Update();
	origin->x = physicsBody->GetPosition().x * PIXEL_PER_METER + width / 2;
	origin->y = physicsBody->GetPosition().y * PIXEL_PER_METER + height / 2;
}

void Rock::Clean()
{
}

void Rock::Fall()
{
	currentState = HazardStates::RockState::FALL;
	b2Vec2 velocity = b2Vec2(0.0f, physicsBody->GetLinearVelocity().y);
	physicsBody->SetLinearVelocity(velocity);
}

void Rock::MoveRight()
{
	flipSprite = SDL_FLIP_NONE;
	currentState = EnemyState::MOVE;
	b2Vec2 velocity = b2Vec2(0.3f, physicsBody->GetLinearVelocity().y);
	physicsBody->SetLinearVelocity(velocity);
}

void Rock::MoveLeft()
{
	currentState = EnemyState::MOVE;
	flipSprite = SDL_FLIP_HORIZONTAL;
	b2Vec2 velocity = b2Vec2(-0.3f, physicsBody->GetLinearVelocity().y);
	physicsBody->SetLinearVelocity(velocity);
}

void Rock::Explode()
{
	currentState = HazardStates::RockState::EXPLODE;

	AudioManager::GetInstance()->PlaySfx("explosion");

	for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext())
	{
		b2Filter filter = fixture->GetFilterData();
		filter.maskBits = BOUNDARY;
		fixture->SetFilterData(filter);
	}
}

void Rock::UpdateAnimationState()
{
	switch (currentState)
	{
	case HazardStates::RockState::FALL:
		animation->SetProperties("rock1", true, 0, 23, 80, flipSprite);
		break;
	case HazardStates::RockState::EXPLODE:
		animation->SetProperties("explosion1", false, 0, 100, 60, flipSprite);
		isExplode = true;
		if (animation->IsEnded())
		{
			isNotVisible = true;
			physicsBody->SetEnabled(false);
		}

		break;
	}
	previousState = currentState;
	previousFlipSprite = flipSprite;
}

void Rock::FallWhenPlayerInRange()
{
	//std::cout << "enemy: " << physicsBody->GetPosition().x << " w:" << physicsBody->GetWorldCenter().y << std::endl;
	//std::cout << "--- player: " << playerBody->GetPosition().x << " y:" << playerBody->GetWorldCenter().y << std::endl;
	float xAxisDistance = physicsBody->GetPosition().x - playerBody->GetPosition().x;
	float yAxisDistance = physicsBody->GetPosition().y - playerBody->GetPosition().y;
	float distance = sqrt(xAxisDistance * xAxisDistance + yAxisDistance * yAxisDistance);

	if (distance < detectRange && currentState != HazardStates::RockState::EXPLODE)
	{
		physicsBody->SetEnabled(true);
	}
}