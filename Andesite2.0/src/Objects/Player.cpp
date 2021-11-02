#include "../pch.h"
#include "Player.h"
#include "../Input/InputManager.h"
#include "../Graphics/TextureManager.h"
#include "../Time/Timer.h"
#include "../Constants.h"
using namespace constants;

Player::Player(Properties* properties): Actor(properties) {
	row = 0;
	frameCount = 8;
	animationSpeed = 80;
	animation = new Animation();
	animation->SetProperties(textureID, 0, 8, 80);
	collisionWidth = 50;
	collisionHeight = 74;
	physicsBody = Physics::GetInstance()->AddRect(properties->position.x, properties->position.y, collisionWidth, collisionHeight, true);
	physicsBody->SetGravityScale(0.1f);
	physicsBody->SetLinearDamping(1.0f);
}

Player::~Player() {
	delete animation;
}

void Player::Draw() {
	animation->Draw(physicsBody->GetPosition().x * PIXEL_PER_METER - (width / 2) , physicsBody->GetPosition().y * PIXEL_PER_METER - (height / 3), width, height);

}

void Player::Update(float dt) {
	animation->Update();
	// frame = (SDL_GetTicks() / animationSpeed) % frameCount; 
}

void Player::Clean() {
	TextureManager::GetInstance()->Clean();
}

// Movement
void Player::MoveRight() {
	animation->SetProperties("player_run", 0, 8, 80);
	b2Vec2 velocity = b2Vec2(2.0f, physicsBody->GetLinearVelocity().y);
	physicsBody->ApplyForce(velocity, physicsBody->GetWorldCenter(), true);
}

void Player::MoveLeft() {
	animation->SetProperties("player_run", 0, 8, 80, SDL_FLIP_HORIZONTAL);
	b2Vec2 velocity = b2Vec2(-5.0f, physicsBody->GetLinearVelocity().y);
	physicsBody->ApplyForce(velocity, physicsBody->GetWorldCenter(), true);
}

void Player::Idle() {
	animation->SetProperties("player_idle", 0, 8, 80, SDL_FLIP_HORIZONTAL);
	b2Vec2 velocity = b2Vec2(0.0f, physicsBody->GetLinearVelocity().y);
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
	else
	{
		// If no contact on floor, then fall animation.
		Fall();
	}
	// Additional Jump allowed in air 
	// Reset Additional Jump allowed in air when landing on floor



}

void Player::Fall() {
	animation->SetProperties("player_fall", 0, 2, 80);
}

void Player::Escape() {
	Game::GetInstance()->SetIsRunning(false);
}