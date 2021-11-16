#include "../pch.h"
#include "Golem.h"
#include "../Constants.h"
#include "../Graphics/TextureManager.h"
#include "../Audio/AudioManager.h"

using namespace constants;
using namespace EnemyStates;

Golem::Golem(Properties* properties): Actor(properties)
{
	row = 0;
	frameCount = 18; 
	animationSpeed = 80;
	animation = new Animation();
	animation->SetProperties(textureID, true, row, frameCount, animationSpeed);
	collisionWidth = 25;
	collisionHeight = 40;
	physicsBody = Physics::GetInstance()->AddEnemyRect(
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
	movementBoundaryLeft = 0.0f;
	movementBoundaryRight = 0.0f;
	isCharge = false;
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
	// TextureManager::GetInstance()->DrawRect(physicsBody->GetPosition().x * PIXEL_PER_METER, physicsBody->GetPosition().y * PIXEL_PER_METER, collisionWidth, collisionHeight);
}

void Golem::Update(float dt)
{
	FollowPlayerWhenInRange();
	UpdateAnimationState();
	animation->Update();
	origin->x = physicsBody->GetPosition().x * PIXEL_PER_METER + width / 2;
	origin->y = physicsBody->GetPosition().y * PIXEL_PER_METER + height / 2;
}

void Golem::Clean()
{
}

void Golem::Idle()
{
	currentState = EnemyState::Idle;
	b2Vec2 velocity = b2Vec2(0.0f, physicsBody->GetLinearVelocity().y);
	physicsBody->SetLinearVelocity(velocity);
}

void Golem::MoveRight()
{
	b2Vec2 velocity;
	flipSprite = SDL_FLIP_NONE;
	currentState = EnemyState::Walk;
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
	currentState = EnemyState::Walk;
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

void Golem::Die()
{
	currentState = EnemyState::Die;
	
	AudioManager::GetInstance()->PlaySfx("golem_death");

	for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext())
	{
		b2Filter filter = fixture->GetFilterData();
		filter.maskBits = BOUNDARY;
		fixture->SetFilterData(filter);
	}
}

void Golem::UpdateAnimationState()
{
	switch (currentState)
	{
		case EnemyState::Idle:
			animation->SetProperties("golem_idle", true, 0, 18, 80, flipSprite);
			break;
		case EnemyState::Walk:
			animation->SetProperties("golem_walking", true, 0, 24, 80, flipSprite);
			break;
		case EnemyState::Die:
			animation->SetProperties("golem_dying", false, 0, 10, 2, flipSprite);
			break;
	}
	previousState = currentState;
	previousFlipSprite = flipSprite;
}

void Golem::FollowPlayerWhenInRange()
{
	//std::cout << "enemy: " << physicsBody->GetPosition().x << " w:" << physicsBody->GetWorldCenter().y << std::endl;
	//std::cout << "--- player: " << playerBody->GetPosition().x << " y:" << playerBody->GetWorldCenter().y << std::endl;
	float xAxisDistance = physicsBody->GetPosition().x - playerBody->GetPosition().x;
	float yAxisDistance = physicsBody->GetPosition().y - playerBody->GetPosition().y;
	float distance = sqrt(xAxisDistance * xAxisDistance + yAxisDistance * yAxisDistance);
	//std::cout << "d: " << distance << std::endl;
	int currentX = physicsBody->GetPosition().x;

	if (distance < GOLEM_DETECT_RANGE && 
		currentState != EnemyState::Die && 
		currentX < movementBoundaryRight && 
		currentX > movementBoundaryLeft )
	{
		if (distance < 5.0f)
		{
			isCharge = true;
		}
		else
		{
			isCharge = false;
		}

		std::cout << xAxisDistance << " " << yAxisDistance << std::endl;

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