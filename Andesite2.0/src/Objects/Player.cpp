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
}

Player::~Player() {
	delete animation;
}

void Player::Draw() {
	animation->Draw(position.x, position.y, width, height);
	// TextureManager::GetInstance()->DrawFrame(textureID, position.x, position.y, width, height, row, frame);
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
}

void Player::MoveLeft() {
	animation->SetProperties("player_run", 0, 8, 80, SDL_FLIP_HORIZONTAL);
}

void Player::Idle() {
	animation->SetProperties("player_idle", 0, 8, 80, SDL_FLIP_HORIZONTAL);
}

void Player::Escape() {
	Game::GetInstance()->SetIsRunning(false);
}