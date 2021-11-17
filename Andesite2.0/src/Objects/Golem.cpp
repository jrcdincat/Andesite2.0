#include "../pch.h"
#include "Golem.h"
#include "../Constants.h"
#include "../Graphics/TextureManager.h"
#include "../Audio/AudioManager.h"

using namespace constants;
using namespace EnemyStates;

Golem::Golem(Properties* properties): Actor(properties)
{
	// Set initial animation properties
	row = 0;
	frameCount = 18; 
	animationSpeed = 80;
	animation = new Animation();
	animation->SetProperties(textureID, true, row, frameCount, animationSpeed);

	// Set physics body
	collisionWidth = 25;
	collisionHeight = 40;
	physicsBody = Physics::GetInstance()->AddEnemyRect(
		(int)properties->position.x, 
		(int)properties->position.y, 
		collisionWidth, 
		collisionHeight, 
		USER_TYPE_GOLEM,
		this
	);
	physicsBody->SetGravityScale(1.0f);
	physicsBody->SetLinearDamping(1.3f);
	movementBoundaryLeft = 0.0f;
	movementBoundaryRight = 0.0f;
	isCharge = false;

	// Get player physics body for detection
	playerInstance = Game::GetInstance()->GetPlayer();
	playerBody = playerInstance->GetPlayerBody();
}

Golem::~Golem()
{
	physicsBody->GetWorld()->DestroyBody(physicsBody);
	delete animation;
}

void Golem::Draw()
{
	animation->Draw(
		physicsBody->GetPosition().x * PIXEL_PER_METER - GOLEM_X_OFFSET_ANIMATION, 
		physicsBody->GetPosition().y * PIXEL_PER_METER - GOLEM_Y_OFFSET_ANIMATION,
		width, 
		height
	);
}

void Golem::Update(float dt)
{
	FollowPlayerWhenInRange();
	UpdateAnimationState();
	animation->Update();
}

void Golem::UpdateAnimationState()
{
	switch (currentState)
	{
		case EnemyState::IDLE:
			animation->SetProperties("golem_idle", true, 0, 18, 80, flipSprite);
			break;

		case EnemyState::MOVE:
			animation->SetProperties("golem_walking", true, 0, 24, 80, flipSprite);
			break;

		case EnemyState::DIE:
			animation->SetProperties("golem_dying", false, 0, 10, 2, flipSprite);
			break;
	}
}

void Golem::Idle()
{
	currentState = EnemyState::IDLE;
	b2Vec2 velocity = b2Vec2(0.0f, physicsBody->GetLinearVelocity().y);
	physicsBody->SetLinearVelocity(velocity);
}

void Golem::Die()
{
	currentState = EnemyState::DIE;
	AudioManager::GetInstance()->PlaySfx("golem_death");

	// Disable collision with player
	for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext())
	{
		b2Filter filter = fixture->GetFilterData();
		filter.maskBits = BOUNDARY;
		fixture->SetFilterData(filter);
	}
}

void Golem::MoveRight()
{
	b2Vec2 velocity;
	flipSprite = SDL_FLIP_NONE;
	currentState = EnemyState::MOVE;

	if (isCharge)
	{
		velocity = b2Vec2(GOLEM_CHARGE_SPEED, physicsBody->GetLinearVelocity().y);
	}
	else
	{
		velocity = b2Vec2(GOLEM_DEFAULT_SPEED, physicsBody->GetLinearVelocity().y);
	}

	physicsBody->SetLinearVelocity(velocity);
}

void Golem::MoveLeft()
{
	b2Vec2 velocity;
	currentState = EnemyState::MOVE;
	flipSprite = SDL_FLIP_HORIZONTAL;

	if (isCharge)
	{
		velocity = b2Vec2(-GOLEM_CHARGE_SPEED, physicsBody->GetLinearVelocity().y);
	}
	else
	{
		velocity = b2Vec2(-GOLEM_DEFAULT_SPEED, physicsBody->GetLinearVelocity().y);
	}

	physicsBody->SetLinearVelocity(velocity);
}

void Golem::FollowPlayerWhenInRange()
{
	float xAxisDistance = physicsBody->GetPosition().x - playerBody->GetPosition().x;
	float yAxisDistance = physicsBody->GetPosition().y - playerBody->GetPosition().y;
	float distance = sqrt(xAxisDistance * xAxisDistance + yAxisDistance * yAxisDistance);
	int currentX = (int)physicsBody->GetPosition().x;

	if (distance < GOLEM_DETECT_RANGE && 
		currentState != EnemyState::DIE && 
		currentX < movementBoundaryRight && 
		currentX > movementBoundaryLeft)
	{
		if (distance < 5.0f)
		{
			isCharge = true;
		}
		else
		{
			isCharge = false;
		}

		// Continue charging in the same direction if the player is directly above
		if ((xAxisDistance < 2.0f && xAxisDistance > -2.0f && yAxisDistance > 1.0f))
		{
			switch (flipSprite)
			{
			case SDL_FLIP_NONE:
				MoveRight();
				break;
			case SDL_FLIP_HORIZONTAL:
				MoveLeft();
				break;
			}
		}
		else
		{
			if (xAxisDistance < 0)
			{
				MoveRight();
			}
			else
			{
				MoveLeft();
			}
		}
	}
}
