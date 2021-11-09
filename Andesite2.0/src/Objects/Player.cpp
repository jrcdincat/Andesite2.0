#include "../pch.h"
#include "Player.h"
#include "../Input/InputManager.h"
#include "../Graphics/TextureManager.h"
#include "../Time/Timer.h"
#include "../Constants.h"
using namespace constants;

Player::Player(Properties* properties): Actor(properties) {
	health = PLAYER_HEALTH;
	row = 0;
	frameCount = 8;
	animationSpeed = 80;
	animation = new Animation();
	animation->SetProperties(textureID, row, frameCount, animationSpeed);
	collisionWidth = 45;
	collisionHeight = 80;
	physicsBody = Physics::GetInstance()->AddPlayerRect(properties->position.x, properties->position.y, collisionWidth, collisionHeight, this);
	physicsBody->SetGravityScale(0.1f);
	physicsBody->SetLinearDamping(1.3f);
}

Player::~Player() {
	delete animation;
}

void Player::Draw() {
	animation->Draw(physicsBody->GetPosition().x * PIXEL_PER_METER - PLAYER_X_OFFSET_ANIMATION, physicsBody->GetPosition().y * PIXEL_PER_METER - PLAYER_Y_OFFSET_ANIMATION, width, height);
	// TextureManager::GetInstance()->DrawRect(physicsBody->GetPosition().x * PIXEL_PER_METER, physicsBody->GetPosition().y * PIXEL_PER_METER, collisionWidth, collisionHeight);
	// std::cout << "x: " << physicsBody->GetPosition().x * PIXEL_PER_METER << "y: " << physicsBody->GetPosition().y * PIXEL_PER_METER << std::endl;
}

void Player::Update(float dt) {
	animation->Update();
	UpdateMovement();
	origin->x = physicsBody->GetPosition().x * PIXEL_PER_METER + width / 2;
	origin->y = physicsBody->GetPosition().y * PIXEL_PER_METER + height / 2;
	// frame = (SDL_GetTicks() / animationSpeed) % frameCount; 
}

void Player::Clean() {
	TextureManager::GetInstance()->Clean();
}

// Movement
void Player::MoveRight() {
	animation->SetProperties("player_run", 0, 8, 80);
	b2Vec2 velocity = b2Vec2(1.0f, physicsBody->GetLinearVelocity().y);
	physicsBody->SetLinearVelocity(velocity);
}

void Player::MoveLeft() {
	animation->SetProperties("player_run", 0, 8, 80, SDL_FLIP_HORIZONTAL);
	b2Vec2 velocity = b2Vec2(-1.0f, physicsBody->GetLinearVelocity().y);
	physicsBody->SetLinearVelocity(velocity);
}

void Player::Jump() {

	std::cout << "User Data: " << reinterpret_cast<Physics::FixtureUserData*>(physicsBody->GetFixtureList()->GetUserData().pointer)->type<< std::endl;
	std::cout << "Feet " << Physics::GetInstance()->numFootContacts << std::endl;
	// Jump once when in contact
	if (Physics::GetInstance()->numFootContacts > 0)
	{
		animation->SetProperties("player_jump", 0, 2, 80);
		float impulse = INT_MAX * Timer::GetInstance()->GetDeltaTime();
		physicsBody->ApplyLinearImpulseToCenter(b2Vec2(physicsBody->GetLinearVelocity().x, -impulse), true);
	}
	
	isJump = false;
	//else
	//{
	//	// If no contact on floor, then fall animation.
	//	b2Vec2 velocity = physicsBody->GetLinearVelocity();
	//	bool isMovingOnAxis = (velocity.x * 0 + velocity.y * (physicsBody->GetGravityScale() * 3.8f) > 0);
	//	if (isMovingOnAxis)
	//	{
	//		Fall();
	//	}
	//	else
	//	{
	//		isJump = false;
	//	}
	//	std::cout << "axis " << isMovingOnAxis << std::endl;
	//}
	// Additional Jump allowed in air 
	// Reset Additional Jump allowed in air when landing on floor
}

void Player::Fall() {
	animation->SetProperties("player_fall", 0, 2, 80);
}


void Player::Idle() {
	animation->SetProperties("player_idle", 0, 8, 80, SDL_FLIP_HORIZONTAL);
	b2Vec2 velocity = b2Vec2(0.0f, physicsBody->GetLinearVelocity().y);
	physicsBody->SetLinearVelocity(velocity);
}

void Player::Hit() {
	health--;

	if (health < 1)
	{
		Die();
	}

	std::cout << "**** Player Hit ****" << std::endl;
	animation->SetProperties("player_hit", 0, 4, 80, SDL_FLIP_HORIZONTAL);
}

void Player::Die() {
	std::cout << "*** Player Die ****" << std::endl;
	animation->SetProperties("player_death", 0, 6, 80, SDL_FLIP_HORIZONTAL);
}

void Player::Escape() {
	Game::GetInstance()->SetIsRunning(false);
}

void Player::UpdateMovement() {
	if (isMoveLeft)
	{
		MoveLeft();
	}
	if (isMoveRight)
	{
		MoveRight();
	}
	if (isJump)
	{
		Jump();
	}
	if (isEscape)
	{
		Escape();
	}

	b2Vec2 velocity = physicsBody->GetLinearVelocity();
	bool isMovingOnAxis = (velocity.x * 0 + velocity.y * (physicsBody->GetGravityScale() * 3.8f) > 0);
	/*if (isMovingOnAxis && Physics::GetInstance()->numFootContacts == 0)
	{
		Fall();
	}
	else
	{
		isJump = false;
	}*/
}