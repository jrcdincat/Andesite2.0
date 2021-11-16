#include "../pch.h"
#include "Bat.h"
#include "../Constants.h"
#include "../Graphics/TextureManager.h"
#include "../Audio/AudioManager.h"

using namespace constants;
using namespace EnemyStates;

Bat::Bat(Properties* properties) : Actor(properties)
{
	row = 0;
	frameCount = 12;
	animationSpeed = 80;
	animation = new Animation();
	animation->SetProperties(textureID, true, row, frameCount, animationSpeed);
	collisionWidth = 80;
	collisionHeight = 30;
	physicsBody = Physics::GetInstance()->AddEnemyRect(
		properties->position.x,
		properties->position.y,
		collisionWidth,
		collisionHeight,
		USER_TYPE_BAT,
		this
	);
	physicsBody->SetGravityScale(0.0f);
	physicsBody->SetLinearDamping(1.3f);
	playerInstance = Game::GetInstance()->GetPlayer();
	playerBody = playerInstance->GetPlayerBody();
	movementBoundaryLeft = 0.0f;
	movementBoundaryRight = 20.0f;
	isCharge = false;
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
	//TextureManager::GetInstance()->DrawRect(physicsBody->GetPosition().x * PIXEL_PER_METER, physicsBody->GetPosition().y * PIXEL_PER_METER, collisionWidth, collisionHeight);
}

void Bat::Update(float dt)
{
	FollowPlayerWhenInRange();
	UpdateAnimationState();
	animation->Update();
	origin->x = physicsBody->GetPosition().x * PIXEL_PER_METER + width / 2;
	origin->y = physicsBody->GetPosition().y * PIXEL_PER_METER + height / 2;
}

void Bat::Clean()
{
}

void Bat::Idle()
{
	currentState = EnemyState::IDLE;
	b2Vec2 velocity = b2Vec2(0.0f, physicsBody->GetLinearVelocity().y);
	physicsBody->SetLinearVelocity(velocity);
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

void Bat::Die()
{
	currentState = EnemyState::DIE;

	AudioManager::GetInstance()->PlaySfx("bat_death");

	for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext())
	{
		b2Filter filter = fixture->GetFilterData();
		filter.maskBits = BOUNDARY;
		fixture->SetFilterData(filter);
	}
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
	previousState = currentState;
	previousFlipSprite = flipSprite;
}

void Bat::FollowPlayerWhenInRange()
{
	//std::cout << "enemy: " << physicsBody->GetPosition().x << " w:" << physicsBody->GetWorldCenter().y << std::endl;
	//std::cout << "--- player: " << playerBody->GetPosition().x << " y:" << playerBody->GetWorldCenter().y << std::endl;
	float xAxisDistance = physicsBody->GetPosition().x - playerBody->GetPosition().x;
	float yAxisDistance = physicsBody->GetPosition().y - playerBody->GetPosition().y;
	float distance = sqrt(xAxisDistance * xAxisDistance + yAxisDistance * yAxisDistance);
	//std::cout << "d: " << distance << std::endl;
	int currentX = physicsBody->GetPosition().x;

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