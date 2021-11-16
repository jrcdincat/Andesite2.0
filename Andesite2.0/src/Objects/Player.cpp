#include "../pch.h"
#include "Player.h"
#include "../Input/InputManager.h"
#include "../Graphics/TextureManager.h"
#include "../Time/Timer.h"
#include "../Audio/AudioManager.h"
#include "../Constants.h"
using namespace constants;

Player::Player(Properties* properties): Actor(properties) 
{
	// Set initial animation properties
	row = 0;
	frameCount = 8;
	animationSpeed = 80;
	animation = new Animation();
	animation->SetProperties(textureID, true, row, frameCount, animationSpeed);
	currentState = PlayerState::IDLE;
	flipSprite = SDL_FLIP_NONE;

	// Set physics body
	collisionWidth = 45;
	collisionHeight = 80;
	physicsBody = Physics::GetInstance()->AddPlayerRect(
		properties->position.x, 
		properties->position.y, 
		collisionWidth, 
		collisionHeight, 
		this
	);
	physicsBody->SetGravityScale(0.1f);
	physicsBody->SetLinearDamping(1.3f);
}

Player::~Player() 
{
	Clean();
}

void Player::Draw()
{
	animation->Draw(
		physicsBody->GetPosition().x * PIXEL_PER_METER - PLAYER_X_OFFSET_ANIMATION,
		physicsBody->GetPosition().y * PIXEL_PER_METER - PLAYER_Y_OFFSET_ANIMATION,
		width,
		height
	);
}

void Player::Update(float dt)
{
	UpdateMovement();
	UpdateAnimationState();
	animation->Update();

	if (physicsBody->GetPosition().x  > X_AXIS_WIN_LOCATION)
	{
		Win();
	}

	origin->x = physicsBody->GetPosition().x * PIXEL_PER_METER + width / 2;
	origin->y = physicsBody->GetPosition().y * PIXEL_PER_METER + height / 2;
}

void Player::Clean()
{
	physicsBody->GetWorld()->DestroyBody(physicsBody);
	delete animation;
}

void Player::UpdateAnimationState()
{
	switch (currentState)
	{
		case PlayerState::IDLE:
			animation->SetProperties("player_idle", true, 0, 8, 80, flipSprite);
			break;

		case PlayerState::RUN:
			animation->SetProperties("player_run", true, 0, 8, 80, flipSprite);
			break;

		case PlayerState::JUMP:
			animation->SetProperties("player_jump", true, 0, 2, 80, flipSprite);
			break;

		case PlayerState::FALL:
			animation->SetProperties("player_fall", true, 0, 2, 80, flipSprite);
			break;

		case PlayerState::DIE:
			animation->SetProperties("player_death", false, 0, 6, 100, flipSprite);
			if (animation->IsEnded())
			{
				Game::GetInstance()->currentGameState = IN_MAIN_MENU;
			}
			break;

		case PlayerState::WIN:
			animation->SetProperties("player_run", true, 0, 8, 80, flipSprite);
			AudioManager::GetInstance()->PlaySfx("player_win");
			if (physicsBody->GetPosition().x > X_AXIS_END_OF_LEVEL_LOCATION)
			{
				Game::GetInstance()->currentGameState = IN_MAIN_MENU;
			}
			break;
	}
}

void Player::Idle() 
{
	currentState = PlayerState::IDLE;
	b2Vec2 velocity = b2Vec2(0.0f, physicsBody->GetLinearVelocity().y);
	physicsBody->SetLinearVelocity(velocity);
}

void Player::Die()
{
	currentState = PlayerState::DIE;
	AudioManager::GetInstance()->PlaySfx("player_death");

	// Disable collision with other objects except boundaries
	for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext())
	{
		b2Filter filter = fixture->GetFilterData();
		filter.maskBits = BOUNDARY;
		fixture->SetFilterData(filter);
	}
}

void Player::Win() 
{
	currentState = PlayerState::WIN;
	SetVelocityMoveRight();
}

void Player::UpdateMovement() 
{
	b2Vec2 velocity = physicsBody->GetLinearVelocity();
	bool isMovingOnYAxis = (velocity.y * (physicsBody->GetGravityScale() * 3.8f) > 0);

	if (physicsBody->GetPosition().y >= LAVA_Y_AXIS)
	{
		Die();
	}

	// Prevent player input from affecting movement when dead or in win state
	if (currentState == PlayerState::DIE || currentState == PlayerState::WIN)
	{
		return;
	}

	// Falling movement
	if (isMovingOnYAxis && Physics::GetInstance()->numFootContacts < 1)
	{
		if (isMoveLeft)
		{
			SetVelocityMoveLeft();
		}
		else if (isMoveRight)
		{
			SetVelocityMoveRight();
		}

		Fall();
	}
	else
	{
		// On ground movement
		if (isMoveLeft | isMoveRight | isJump)
		{

			if (isJump)
			{
				Jump();
			}

			if (isMoveLeft)
			{
				RunLeft();
			}
			
			if (isMoveRight)
			{
				RunRight();
			}
		}
		else if (currentState != PlayerState::DIE && currentState != PlayerState::WIN)
		{
			Idle();
		}
	}
}

void Player::SetVelocityMoveRight() 
{
	flipSprite = SDL_FLIP_NONE;
	b2Vec2 velocity = b2Vec2(PLAYER_SPEED, physicsBody->GetLinearVelocity().y);
	physicsBody->SetLinearVelocity(velocity);
}

void Player::SetVelocityMoveLeft() 
{
	flipSprite = SDL_FLIP_HORIZONTAL;
	b2Vec2 velocity = b2Vec2(-PLAYER_SPEED, physicsBody->GetLinearVelocity().y);
	physicsBody->SetLinearVelocity(velocity);
}

void Player::RunRight()
{
	currentState = PlayerState::RUN;
	SetVelocityMoveRight();
}

void Player::RunLeft()
{
	currentState = PlayerState::RUN;
	SetVelocityMoveLeft();
}

void Player::Jump() 
{
	// Jump once when in contact with floor
	if (Physics::GetInstance()->numFootContacts > 0)
	{
		currentState = PlayerState::JUMP;
		AudioManager::GetInstance()->PlaySfx("jump");
		float impulse = INT_MAX * Timer::GetInstance()->GetDeltaTime();
		physicsBody->ApplyLinearImpulseToCenter(b2Vec2(physicsBody->GetLinearVelocity().x, -impulse), true);
	}
}

void Player::Fall()
{
	isJump = false;
	currentState = PlayerState::FALL;
}
