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
	physicsBody->SetLinearDamping(2.0f);
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
	animation->SetProperties("player_jump", 0, 2, 80);
	std::cout << "mass" << physicsBody->GetMass() << std::endl;
	float impulse = INT_MAX * Timer::GetInstance()->GetDeltaTime();
	b2Vec2 velocity = physicsBody->GetLinearVelocity();
	velocity.y = 10;
	std::cout << "impulse: " << impulse << std::endl;
	physicsBody->ApplyLinearImpulseToCenter(b2Vec2(physicsBody->GetLinearVelocity().x, -impulse), true);
	std::cout << "impulse:" << impulse << std::endl;
	std::cout << "velocity" << physicsBody->GetLinearVelocity().y << std::endl;
	std::cout << physicsBody->GetPosition().y << std::endl;
	std::cout << "Timer: " << Timer::GetInstance()->GetDeltaTime() << std::endl;
	
	
	// Apply force for specfied number of seconds for jump, use impulse to apply instantly 
	for (b2ContactEdge* edge = physicsBody->GetContactList(); edge; edge = edge->next)
	{
		std::cout << edge->contact->IsTouching() << std::endl;
		std::cout << "edge: " << edge->contact << std::endl;
		if (edge->contact->IsTouching())
		{
			b2FixtureUserData userData = edge->contact->GetFixtureA()->GetUserData();
		}
	}
}

void Player::Fall() {
	animation->SetProperties("player_fall", 0, 2, 80);
}

void Player::Escape() {
	Game::GetInstance()->SetIsRunning(false);
}