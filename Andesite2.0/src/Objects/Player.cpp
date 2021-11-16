#include "../pch.h"
#include "Player.h"
#include "../Input/InputManager.h"
#include "../Graphics/TextureManager.h"
#include "../Time/Timer.h"
#include "../Audio/AudioManager.h"
#include "../Constants.h"
using namespace constants;

Player::Player(Properties* properties): Actor(properties) {
	row = 0;
	frameCount = 8;
	animationSpeed = 80;
	animation = new Animation();
	animation->SetProperties(textureID, true, row, frameCount, animationSpeed);
	collisionWidth = 45;
	collisionHeight = 80;
	physicsBody = Physics::GetInstance()->AddPlayerRect(properties->position.x, properties->position.y, collisionWidth, collisionHeight, this);
	physicsBody->SetGravityScale(0.1f);
	physicsBody->SetLinearDamping(1.3f);
	currentState = PlayerState::Idle;
	previousState = PlayerState::Idle;
	flipSprite = SDL_FLIP_NONE;
	previousFlipSprite = SDL_FLIP_NONE;
}

Player::~Player() {
	physicsBody->GetWorld()->DestroyBody(physicsBody);
	delete animation;
}

void Player::Draw() {
	animation->Draw(physicsBody->GetPosition().x * PIXEL_PER_METER - PLAYER_X_OFFSET_ANIMATION, physicsBody->GetPosition().y * PIXEL_PER_METER - PLAYER_Y_OFFSET_ANIMATION, width, height);
	// TextureManager::GetInstance()->DrawRect(physicsBody->GetPosition().x * PIXEL_PER_METER, physicsBody->GetPosition().y * PIXEL_PER_METER, collisionWidth, collisionHeight);
	// std::cout << "x: " << physicsBody->GetPosition().x << "y: " << physicsBody->GetPosition().y << std::endl;
}

void Player::Update(float dt) {
	UpdateMovement();
	UpdateAnimationState();
	animation->Update();

	if (physicsBody->GetPosition().x  > X_AXIS_WIN_LOCATION)
	{
		Win();
	}

	origin->x = physicsBody->GetPosition().x * PIXEL_PER_METER + width / 2;
	origin->y = physicsBody->GetPosition().y * PIXEL_PER_METER + height / 2;
	// frame = (SDL_GetTicks() / animationSpeed) % frameCount; 
}

void Player::Clean() {
	TextureManager::GetInstance()->Clean();
}

// Movement
void Player::RunRight() {
	currentState = PlayerState::Run;
	SetVelocityMoveRight();
}

void Player::RunLeft() {
	currentState = PlayerState::Run;
	SetVelocityMoveLeft();
}

void Player::SetVelocityMoveRight() {
	flipSprite = SDL_FLIP_NONE;
	b2Vec2 velocity = b2Vec2(1.0f, physicsBody->GetLinearVelocity().y);
	physicsBody->SetLinearVelocity(velocity);
}

void Player::SetVelocityMoveLeft() {
	flipSprite = SDL_FLIP_HORIZONTAL;
	b2Vec2 velocity = b2Vec2(-1.0f, physicsBody->GetLinearVelocity().y);
	physicsBody->SetLinearVelocity(velocity);
}

void Player::Jump() {
	// Jump once when in contact
	if (Physics::GetInstance()->numFootContacts > 0)
	{
		currentState = PlayerState::Jump;
		AudioManager::GetInstance()->PlaySfx("jump");
		float impulse = INT_MAX * Timer::GetInstance()->GetDeltaTime();
		physicsBody->ApplyLinearImpulseToCenter(b2Vec2(physicsBody->GetLinearVelocity().x, -impulse), true);
	}
}

void Player::Fall() {
	isJump = false;
	currentState = PlayerState::Fall;
}

void Player::Idle() {
	currentState = PlayerState::Idle;
	b2Vec2 velocity = b2Vec2(0.0f, physicsBody->GetLinearVelocity().y);
	physicsBody->SetLinearVelocity(velocity);
}

void Player::Die() {
	currentState = PlayerState::Die;
	AudioManager::GetInstance()->PlaySfx("player_death");
}

void Player::Win() {
	currentState = PlayerState::Win;
	flipSprite = SDL_FLIP_NONE;
	SetVelocityMoveRight();
}

void Player::Escape() {
	Game::GetInstance()->SetIsRunning(false);
}

void Player::UpdateMovement() {
	b2Vec2 velocity = physicsBody->GetLinearVelocity();
	bool isMovingOnAxis = (velocity.x * 0 + velocity.y * (physicsBody->GetGravityScale() * 3.8f) > 0);
	// Exit
	if (isEscape)
	{
		Escape();
	}

	if (physicsBody->GetPosition().y >= LAVA_Y_AXIS)
	{
		Die();
	}

	if (currentState == PlayerState::Die || currentState == PlayerState::Win)
	{
		return;
	}

	// Falling movement
	if (isMovingOnAxis && Physics::GetInstance()->numFootContacts < 1)
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
		else if (currentState != PlayerState::Die && currentState != PlayerState::Win)
		{
			Idle();
		}

	}
}


void Player::UpdateAnimationState()
{
	switch (currentState)
	{
		case PlayerState::Idle:
			animation->SetProperties("player_idle", true, 0, 8, 80, flipSprite);
			break;
		case PlayerState::Run:
			animation->SetProperties("player_run", true, 0, 8, 80, flipSprite);
			break;
		case PlayerState::Jump:
			animation->SetProperties("player_jump", true, 0, 2, 80, flipSprite);
			break;
		case PlayerState::Fall:
			animation->SetProperties("player_fall", true, 0, 2, 80, flipSprite);
			break;
		case PlayerState::Die:
			for (b2Fixture* fixture = physicsBody->GetFixtureList(); fixture; fixture = fixture->GetNext())
			{
				b2Filter filter = fixture->GetFilterData();
				filter.maskBits = BOUNDARY;
				fixture->SetFilterData(filter);
			}
			animation->SetProperties("player_death", false, 0, 6, 100, flipSprite);
			if (animation->IsEnded())
			{
				Game::GetInstance()->currentGameState = IN_MAIN_MENU;
			}
			break;
		case PlayerState::Win:
			animation->SetProperties("player_run", true, 0, 8, 80, flipSprite);
			AudioManager::GetInstance()->PlaySfx("player_win");
			if (physicsBody->GetPosition().x > X_AXIS_END_OF_LEVEL_LOCATION)
			{
				Game::GetInstance()->currentGameState = IN_MAIN_MENU;
			}
			break;
	}
		previousState = currentState;
		previousFlipSprite = flipSprite;
}
