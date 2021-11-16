#include "../pch.h"
#include "Rock.h"
#include "../Constants.h"
#include "../Graphics/TextureManager.h"
#include "../Audio/AudioManager.h"
using namespace constants;

Rock::Rock(Properties* properties) : Actor(properties)
{
	// Set initial animation properties
	row = 0;
	frameCount = 23;
	animationSpeed = 80;
	animation = new Animation();
	animation->SetProperties(textureID, true, row, frameCount, animationSpeed);
	isExplode = false;
	isNotVisible = false;
	explosionWidth = 160;
	explosionHeight = 160;

	// Set physics body
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

	// Get player physics body for detection
	playerInstance = Game::GetInstance()->GetPlayer();
	playerBody = playerInstance->GetPlayerBody();
	physicsBody->SetEnabled(false);
	detectRange = 20.0f;
}

Rock::~Rock()
{
	Clean();
}

void Rock::Draw()
{
	if (isNotVisible)
	{
		return;
	}
	else if (isExplode)
	{
		animation->Draw(
			physicsBody->GetPosition().x * PIXEL_PER_METER - EXPLOSION1_X_OFFSET_ANIMATION,
			physicsBody->GetPosition().y * PIXEL_PER_METER - EXPLOSION1_Y_OFFSET_ANIMATION,
			explosionWidth,
			explosionHeight
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
}

void Rock::Update(float dt)
{
	FallWhenPlayerInRange();
	UpdateAnimationState();
	animation->Update();
}

void Rock::Clean()
{
	physicsBody->GetWorld()->DestroyBody(physicsBody);
	delete animation;
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
		}
		break;
	}
	previousState = currentState;
	previousFlipSprite = flipSprite;
}

void Rock::Fall()
{
	currentState = HazardStates::RockState::FALL;
	b2Vec2 velocity = b2Vec2(0.0f, physicsBody->GetLinearVelocity().y);
	physicsBody->SetLinearVelocity(velocity);
}

void Rock::Explode()
{
	currentState = HazardStates::RockState::EXPLODE;
	AudioManager::GetInstance()->PlaySfx("explosion");

	// Disable collision with player
	for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext())
	{
		b2Filter filter = fixture->GetFilterData();
		filter.maskBits = BOUNDARY;
		fixture->SetFilterData(filter);
	}
}

void Rock::FallWhenPlayerInRange()
{
	float xAxisDistance = physicsBody->GetPosition().x - playerBody->GetPosition().x;
	float yAxisDistance = physicsBody->GetPosition().y - playerBody->GetPosition().y;
	float distance = sqrt(xAxisDistance * xAxisDistance + yAxisDistance * yAxisDistance);

	if (distance < detectRange && currentState != HazardStates::RockState::EXPLODE)
	{
		physicsBody->SetEnabled(true);
	}
}