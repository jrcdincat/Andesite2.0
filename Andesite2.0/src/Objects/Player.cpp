#include "../pch.h"
#include "Player.h"
#include "../Input/InputManager.h"
#include "../Graphics/TextureManager.h"

Player::Player(Properties* properties): Actor(properties) {
	row = 0;
	frameCount = 8;
	animationSpeed = 80;
	animation = new Animation();
	animation->SetProperties(textureID, 0, 8, 80);
	collisionWidth = 50;
	collisionHeight = 74;
	physicsBody = Physics::GetInstance()->AddRect(properties->position.x, properties->position.y , collisionWidth, collisionHeight, true);
}

Player::~Player() {
	delete animation;
}

void Player::Draw() {
	animation->Draw(physicsBody->GetPosition().x - (width / 2 - 30) , physicsBody->GetPosition().y - (height / 2  - 40), width, height);

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
	b2Vec2 velocity = b2Vec2(200.0f, 0.0f);
	physicsBody->SetLinearVelocity(velocity);
}

void Player::MoveLeft() {
	animation->SetProperties("player_run", 0, 8, 80, SDL_FLIP_HORIZONTAL);
	b2Vec2 velocity = b2Vec2(-55.0f, 0.0f);
	physicsBody->SetLinearVelocity(velocity);
}

void Player::Idle() {
	animation->SetProperties("player_idle", 0, 8, 80, SDL_FLIP_HORIZONTAL);
}

void Player::Escape() {
	Game::GetInstance()->SetIsRunning(false);
}