#include "../pch.h"
#include "Bat.h"
#include "../Constants.h"
#include "../Graphics/TextureManager.h"
#include "../Audio/AudioManager.h"

using namespace constants;
using namespace EnemyStates;

Bat::Bat(Properties* properties) : Actor(properties)
{
	// Set initial animation properties
	row = 0;
	frameCount = 12;
	animationSpeed = 80;
	animation = new Animation();
	animation->SetProperties(textureID, true, row, frameCount, animationSpeed);

	// Set physics body
	collisionWidth = 80;
	collisionHeight = 30;
	physicsBody = Physics::GetInstance()->AddEnemyRect(
		(int)properties->position.x,
		(int)properties->position.y,
		collisionWidth,
		collisionHeight,
		USER_TYPE_BAT,
		this
	);
	physicsBody->SetGravityScale(0.0f);
	physicsBody->SetLinearDamping(1.3f);
	movementBoundaryLeft = 0.0f;
	movementBoundaryRight = 20.0f;
	isCharge = false;

	// Get player physics body for detection
	playerInstance = Game::GetInstance()->GetPlayer();
	playerBody = playerInstance->GetPlayerBody();
}

Bat::~Bat()
{
	physicsBody->GetWorld()->DestroyBody(physicsBody);
	delete animation;
}

void Bat::Draw()
{
	animation->Draw(
		physicsBody->GetPosition().x * PIXEL_PER_METER - BAT_X_OFFSET_ANIMATION,
		physicsBody->GetPosition().y * PIXEL_PER_METER - BAT_Y_OFFSET_ANIMATION,
		width,
		height
	);
}

void Bat::Update(float dt)
{
	FollowPlayerWhenInRange();
	UpdateAnimationState();
	animation->Update();
}

void Bat::UpdateAnimationState()
{
	switch (currentState)
	{
		case EnemyState::IDLE:
			animation->SetProperties("bat_idle", true, 0, 12, 80, flipSprite);
			break;

		case EnemyState::MOVE:
			animation->SetProperties("bat_flying", true, 0, 8, 80, flipSprite);
			break;

		case EnemyState::DIE:
			animation->SetProperties("bat_dying", false, 0, 8, 2, flipSprite);
			physicsBody->SetGravityScale(0.1f);
			break;
	}
}

void Bat::Idle()
{
	currentState = EnemyState::IDLE;
	b2Vec2 velocity = b2Vec2(0.0f, physicsBody->GetLinearVelocity().y);
	physicsBody->SetLinearVelocity(velocity);
}

void Bat::Die()
{
	currentState = EnemyState::DIE;
	AudioManager::GetInstance()->PlaySfx("bat_death");

	// Disable collision with player;
	for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext())
	{
		b2Filter filter = fixture->GetFilterData();
		filter.maskBits = BOUNDARY;
		fixture->SetFilterData(filter);
	}
}

void Bat::MoveRight()
{
	b2Vec2 velocity;
	flipSprite = SDL_FLIP_NONE;
	currentState = EnemyState::MOVE;

	if (isCharge)
	{
		velocity = b2Vec2(BAT_CHARGE_SPEED, physicsBody->GetLinearVelocity().y);
	}
	else
	{
		velocity = b2Vec2(BAT_DEFAULT_SPEED, physicsBody->GetLinearVelocity().y);
	}

	physicsBody->SetLinearVelocity(velocity);
}

void Bat::MoveLeft()
{
	b2Vec2 velocity;
	currentState = EnemyState::MOVE;
	flipSprite = SDL_FLIP_HORIZONTAL;

	if (isCharge)
	{
		velocity = b2Vec2(-BAT_CHARGE_SPEED, physicsBody->GetLinearVelocity().y);
	}
	else
	{
		velocity = b2Vec2(-BAT_DEFAULT_SPEED, physicsBody->GetLinearVelocity().y);
	}

	physicsBody->SetLinearVelocity(velocity);
}

void Bat::FollowPlayerWhenInRange()
{
	float xAxisDistance = physicsBody->GetPosition().x - playerBody->GetPosition().x;
	float yAxisDistance = physicsBody->GetPosition().y - playerBody->GetPosition().y;
	float distance = sqrt(xAxisDistance * xAxisDistance + yAxisDistance * yAxisDistance);
	int currentX = (int)physicsBody->GetPosition().x;

	if (distance < BAT_DETECT_RANGE &&
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
		else if (xAxisDistance < 1.5f && xAxisDistance > -1.5f && yAxisDistance < -1.0f)
		{
			Idle();
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
